/******************************************************************************
* file    WallLight.h
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

#ifndef WALLLIGHT_H
#define WALLLIGHT_H

#include <Arduino.h>

#include "Adafruit_NeoPXL8.h"

class WallLight
{
  public:
    WallLight(int8_t rgbPin, int8_t wwaPin, uint16_t pixelCount, uint16_t framerate);
    bool begin(void);

    const uint16_t PIXELCOUNT;
    const uint16_t FRAMERATE;
    
  private:
    const int8_t rgbPin;
    const int8_t wwaPin;

    int8_t pins[8] = {rgbPin, wwaPin, -1, -1, -1, -1, -1, -1};
    Adafruit_NeoPXL8 leds = Adafruit_NeoPXL8(PIXELCOUNT, pins, NEO_GRB);

    static void update(void* pvParameter);
};

#endif
