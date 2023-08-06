/******************************************************************************
* file    powerMgmt.cpp
*******************************************************************************
* brief   Power management abstraction layer
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-08-06
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


#include "powerMgmt.hpp"
#include "console.hpp"


PowerMgmt::PowerMgmt(int pwrSw, int pwrOff, int batMsr) : pin_pwrSw(pwrSw), pin_pwrOff(pwrOff), pin_batMsr(batMsr)
{
}

bool PowerMgmt::begin(void)
{
  digitalWrite(pin_pwrOff, LOW);
  pinMode(pin_pwrOff, OUTPUT);
  pinMode(pin_pwrSw, INPUT_PULLUP);
  pinMode(pin_batMsr, INPUT);

  startTime = millis();
  initialized = true;
  xTaskCreate(update, "task_updatePowerMgmt", 4096, this, 10, nullptr);
  return true;
}

float PowerMgmt::measureBatteryVoltage(void)
{
  float voltage = 0.0;
  for(int i = 0; i < POWER_MGMT_MEASUREMENT_AVR_NUM; i++)
  {
    voltage += analogRead(pin_batMsr);
  }
  voltage /= (float)POWER_MGMT_MEASUREMENT_AVR_NUM;
  voltage /= (float)POWER_MGMT_MEASUREMENT_FACTOR;
  return voltage;
}

uint8_t PowerMgmt::calculateBatteryPercentage(float v)
{
  #ifdef POWER_MGMT_DISABLE_BATTERY_MEASUREMENT
    return 100;
  #endif

  static bool printedWarning = false;
  if(v < 0.2)
  {
    if(!printedWarning)
    {
      console.warning.printf("[POWER_MGMT] No Battery detected (%.2f V)\n", v);
      printedWarning = true;
    }
    return 100;
  }
  
  uint16_t soc = 0;
  for (soc = 0; soc <= 100; soc++)
  {
    if (v >= batteryVoltagePercentage[soc]) break;
  }
  soc = max(100 - soc, 0);
  return soc;
}

void PowerMgmt::update(void *pvParameter)
{
  PowerMgmt* ref = (PowerMgmt*)pvParameter;

  while(ref->initialized)
  {
    TickType_t task_last_tick = xTaskGetTickCount();

    ref->pwrButtonOld = ref->pwrButton;
    ref->pwrButton = !digitalRead(ref->pin_pwrSw);

    if(ref->pwrButton && !ref->pwrButtonOld && (millis() - ref->startTime) > POWER_MGMT_POWERDOWN_VALID_DELAY)
    {
      digitalWrite(ref->pin_pwrOff, HIGH);
      delay(100);   // Wait until System is turned off
    }

    ref->batteryVoltage = ref->measureBatteryVoltage();
    ref->batteryPercentage = ref->calculateBatteryPercentage(ref->batteryVoltage);

    vTaskDelayUntil(&task_last_tick, (const TickType_t) 1000 / POWER_MGMT_UPDATE_RATE);
  }
  vTaskDelete(NULL);
}
