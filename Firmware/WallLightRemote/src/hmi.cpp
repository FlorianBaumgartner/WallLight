/******************************************************************************
* file    hmi.cpp
*******************************************************************************
* brief   Human-Machine interface
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-07-23
*******************************************************************************
* MIT License
*
* Copyright (c) 2023 Crelin - Florian Baumgartner
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell          
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#include "hmi.hpp"
#include "console.hpp"


Hmi::Hmi(int clk, int data, int ld, int ens, int ena, int enb) : pin_clk(clk), pin_data(data), pin_ld(ld), pin_ens(ens), pin_ena(ena), pin_enb(enb)
{ 
}

bool Hmi::begin(void)
{
  pinMode(pin_clk, OUTPUT);
  pinMode(pin_data, INPUT);
  pinMode(pin_ld, OUTPUT);
  pinMode(pin_ens, INPUT_PULLUP);
  pinMode(pin_ena, INPUT_PULLUP);
  pinMode(pin_enb, INPUT_PULLUP);

  initialized = true;
  xTaskCreate(update, "task_updateHmi", 4096, this, 1, &updateTaskHandle);
  return true;
}

void Hmi::update(void *pvParameter)
{
  Hmi* ref = (Hmi*)pvParameter;

  while(ref->initialized)
  {
    TickType_t task_last_tick = xTaskGetTickCount();
    ref->readOut = ref->readSerial();
    
    // Handle Id Buttons
    uint8_t btnUp = 0x00, btnDown = 0x00;
    for(int i = 0; i < 8; i++)
    {
      btnUp |= ((~ref->readOut & ref->ID_BUTTON_UP_POS[i])? 0x01 : 0x00) << i;
      btnDown |= ((~ref->readOut & ref->ID_BUTTON_DOWN_POS[i])? 0x01 : 0x00) << i;
    }
    ref->idButtonUpOld = ref->idButtonUp;
    ref->idButtonDownOld = ref->idButtonDown;
    ref->idButtonUp = btnUp;
    ref->idButtonDown = btnDown;
    for(int i = 0; i < 8; i++)
    {
      ref->idButtonUpEdge |= (!(ref->idButtonUpOld & (0x01 << i)) && (ref->idButtonUp & (0x01 << i))) << i;
      ref->idButtonDownEdge |= (!(ref->idButtonDownOld & (0x01 << i)) && (ref->idButtonDown & (0x01 << i))) << i;
    }

    // Handle Buttons and Switches
    uint8_t btnSwitch = 0x00;
    for(int i = 0; i < 4; i++)
    {
      const uint8_t pos[4] = {2, 3, 6, 7};
      btnSwitch |= ((~ref->readOut & ref->BUTTON_SWITCH_POS[i])? 0x01 : 0x00) << pos[i];
    }
    ref->buttonSwitch = btnSwitch;

    // Handle Roller Encoders
    uint8_t encSwitch = 0x00;
    for(int i = 0; i < 4; i++)
    {
      const uint8_t pos[4] = {0, 1, 4, 5};
      encSwitch |= ((~ref->readOut & ref->ENCODER_SWITCH_POS[i])? 0x01 : 0x00) << pos[i];
    }
    ref->encoderSwitchOld = ref->encoderSwitch;
    ref->encoderSwitch = encSwitch;
    for(int i = 0; i < 8; i++)
    {
      ref->encoderSwitchEdge |= (!(ref->encoderSwitchOld & (0x01 << i)) && (ref->encoderSwitch & (0x01 << i))) << i;
    }
    for(int i = 0; i < 4; i++)
    {
      ref->rollerEncoder[i].tick();
    }

    vTaskDelayUntil(&task_last_tick, (const TickType_t) 1000 / HMI_UPDATE_RATE);
  }
  vTaskDelete(NULL);
}

uint32_t Hmi::readSerial(void)    // Readout frequency: ~ 1 MHz
{
  uint32_t data = 0x00000000;
  digitalWrite(pin_ld, HIGH);
  for(int i = 0; i < 32; i++)
  {
    data <<= 1;
    data |= digitalRead(pin_data);
    digitalWrite(pin_clk, HIGH);
    digitalWrite(pin_clk, LOW);
  }
  digitalWrite(pin_ld, LOW);
  return data;
}
