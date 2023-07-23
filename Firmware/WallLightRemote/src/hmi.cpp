/******************************************************************************
* file    hmi.cpp
*******************************************************************************
* brief   Humanmachine interface
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

  // hspi.begin(clk, data, -1, ld);
  // hspi.setFrequency(1000000);

  // hspi.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  // for(int i = 0; i < 4; i++)
  // {
  //   console.log.printf("SPI: %d\n", hspi.transfer(0x00));
  // }
  // hspi.endTransaction();

  return true;
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
