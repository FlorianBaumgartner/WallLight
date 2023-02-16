/******************************************************************************
* file    WallLight.cpp
*******************************************************************************
* brief   Main class for handling all WallLight related tasks
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-14
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

#include "WallLight.h"
#include "console.h"
#include "utils.h"
#include "freertos/task.h"

#define DEBUG_PIN       16


WallLight::WallLight(int8_t rgbPin, int8_t wwaPin, uint16_t pixelCount, uint16_t framerate): rgbPin(rgbPin), wwaPin(wwaPin), PIXELCOUNT(pixelCount), FRAMERATE(framerate)
{
    
}

bool WallLight::begin(void)
{
  if (!leds.begin())
  {
    console.error.println("[WALLLIGHT] Could not initialize LEDs");
  }
  leds.setBrightness(30);
  leds.show();

  xTaskCreate(update, "task_walllight", 2048, this, 1, NULL);
  console.ok.println("[WALLLIGHT] Initialization successfull!");
  return true;
}

void WallLight::update(void* pvParameter)
{
  WallLight* ref = (WallLight*)pvParameter;

  pinMode(DEBUG_PIN, OUTPUT);

  while(true)
  {
    TickType_t task_last_tick = xTaskGetTickCount();
    digitalWrite(DEBUG_PIN, !digitalRead(DEBUG_PIN));

    ref->leds.fill(ref->leds.Color(100, 0, (millis() / 10) & 0xFF));
    ref->leds.show();

    vTaskDelayUntil(&task_last_tick, (const TickType_t) 1000 / ref->FRAMERATE);
  }
  vTaskDelete(NULL);
}
