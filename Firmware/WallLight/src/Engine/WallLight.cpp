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

#include "WallLight.hpp"
#include "console.hpp"
#include "utils.hpp"
#include "freertos/task.h"

#define DEBUG_PIN       16


WallLight::WallLight(int8_t rgbPin, int8_t wwaPin, bool rgbw): rgbPin(rgbPin), wwaPin(wwaPin)
{
  if(rgbw)
  {
    console.log.println("[WALLLIGHT] RGBW Mode activated.");
  }
  leds = new Adafruit_NeoPXL8(PIXELCOUNT, pins, rgbw? NEO_GRBW : NEO_GRB);
}

WallLight::~WallLight()
{
  delete leds;
}

bool WallLight::begin(void)
{
  if (!leds->begin())
  {
    console.error.println("[WALLLIGHT] Could not initialize LEDs");
  }
  leds->setBrightness(10);
  leds->show();

  xTaskCreate(update, "task_walllight", 16384, this, 1, NULL);       // TODO: Check if stack size is enough
  console.ok.println("[WALLLIGHT] Initialization successfull.");
  return true;
}

bool WallLight::loadGraph(const char* path, uint8_t engineIndex)
{
  engineIndex &= 0x01;
  console.log.printf("[WALLLIGHT] Loading graph from file: '%s' to Engine %d\n", path, engineIndex);
  return engine[engineIndex].loadGraph(path);
}

void WallLight::unloadGraph(uint8_t engineIndex)
{
  engineIndex &= 0x01;
  console.log.printf("[WALLLIGHT] Unloading graph from Engine %d pending...\n", engineIndex);
  unloadingGraphPending[engineIndex] = true;
}

void WallLight::update(void* pvParameter)
{
  WallLight* ref = (WallLight*)pvParameter;
  bool updateStatus = true;
  bool printOnce = true;

  while(true)
  {
    TickType_t task_last_tick = xTaskGetTickCount();

    if(ref->unloadingGraphPending[0])
    {
      ref->unloadingGraphPending[0] = false;
      console.log.printf("[WALLLIGHT] Unloading graph from Engine %d\n", 0);
      ref->engine[0].unloadGraph();
    }
    if(ref->engine[0].isReady())                                                  // Check if graph is loaded 
    {
      bool updateStatus = ref->engine[0].update((float)millis() / 1000.0);        // TODO: Get time from dedicated time source
      if(updateStatus)
      {
        LedVector* pixels = ref->engine[0].getPixelData();      // TODO: Make dynamic
        if(pixels && updateStatus)                              // Check if output is connected to module
        {
          if(pixels->value)                                     // Check if led vector of output module is allocated
          {
            for(int i = 0; i < ref->PIXELCOUNT; i++)
            {
              uint_fast8_t r = uint_fast8_t(constrain(pixels->value[0][i] * 255.0, 0.0, 255.0));
              uint_fast8_t g = uint_fast8_t(constrain(pixels->value[1][i] * 255.0, 0.0, 255.0));
              uint_fast8_t b = uint_fast8_t(constrain(pixels->value[2][i] * 255.0, 0.0, 255.0));
              uint_fast8_t w = uint_fast8_t(constrain(pixels->value[3][i] * 255.0, 0.0, 255.0));
              uint_fast8_t c = uint_fast8_t(constrain(pixels->value[4][i] * 255.0, 0.0, 255.0));
              uint_fast8_t a = uint_fast8_t(constrain(pixels->value[5][i] * 255.0, 0.0, 255.0));
              ref->leds->setPixelColor(i, ref->leds->Color(r, g, b));
              ref->leds->setPixelColor(i + ref->PIXELCOUNT, ref->leds->Color(w, c, a));
            }
          }
        }
      }
      else
      {
        console.error.println("[WALLLIGHT] Error occured while updating graph");
        ref->engine[0].unloadGraph();
      }
    }
    else
    {
      ref->leds->fill();
    }
    
    ref->leds->show();
    vTaskDelayUntil(&task_last_tick, (const TickType_t) 1000 / ref->FRAMERATE);
  }
  vTaskDelete(NULL);
}
