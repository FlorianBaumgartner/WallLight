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
  console.ok.println("[WALLLIGHT] Initialization successfull.");
  return true;
}

bool WallLight::loadGraph(const char* path, uint8_t engineIndex)
{
  engineIndex &= 0x01;
  console.log.printf("[WALLLIGHT] Loading graph from file: '%s' to Engine %d\n", path, engineIndex);
  return engine[engineIndex].loadGraph(path);
}

void WallLight::update(void* pvParameter)
{
  WallLight* ref = (WallLight*)pvParameter;

  while(true)
  {
    TickType_t task_last_tick = xTaskGetTickCount();

    LedVector* pixels = ref->engine[0].getPixelData();     // TODO: Make dynamic
    if(pixels->value)
    {
      for(int i = 0; i < ref->PIXELCOUNT; i++)
      {
        uint_fast8_t r = uint_fast8_t(constrain(pixels->value[0][i] * 255.0, 0.0, 255.0));
        uint_fast8_t g = uint_fast8_t(constrain(pixels->value[1][i] * 255.0, 0.0, 255.0));
        uint_fast8_t b = uint_fast8_t(constrain(pixels->value[2][i] * 255.0, 0.0, 255.0));
        uint_fast8_t w = uint_fast8_t(constrain(pixels->value[3][i] * 255.0, 0.0, 255.0));
        uint_fast8_t c = uint_fast8_t(constrain(pixels->value[4][i] * 255.0, 0.0, 255.0));
        uint_fast8_t a = uint_fast8_t(constrain(pixels->value[5][i] * 255.0, 0.0, 255.0));
        ref->leds.setPixelColor(i, ref->leds.Color(r, g, b));
        ref->leds.setPixelColor(i + ref->PIXELCOUNT, ref->leds.Color(w, c, a));
      }
    }
    else
    {
      ref->leds.fill();
    }
    ref->leds.show();
    vTaskDelayUntil(&task_last_tick, (const TickType_t) 1000 / ref->FRAMERATE);
  }
  vTaskDelete(NULL);
}
