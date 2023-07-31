/******************************************************************************
* file    ambientSensor.hpp
*******************************************************************************
* brief   High Level Driver for VEML6040 color and ambient light sensor
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-07-31
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

#ifndef AMBIENT_SENSOR_HPP
#define AMBIENT_SENSOR_HPP

#include <Arduino.h>
// #include <Wire.h>
#include "veml6040.h"

#define AMBIENT_SENSOR_UPDATE_RATE      5             // [hz]

class AmbientColor
{
  public:
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;
    float white = 0.0;
    float cct = 0.0;
    float ambient = 0.0;
};

class AmbientSensor
{
  public:
    AmbientSensor(int scl, int sda, int freq = 1000000);
    bool begin(uint8_t config = VEML6040_IT_320MS + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);
    void getColor(AmbientColor* color) {*color = this->color;}
  
  private:
    const int pin_scl;
    const int pin_sda;
    const int frequency;

    VEML6040 sensor;
    AmbientColor color;

    volatile bool initialized = false;

    uint16_t getCCT(int red, int green, int blue, float offset = 0.5)
    {
      float ccti = ((float)red-(float)blue) / ((float)green + 1);
      ccti = ccti + offset; 
      float cct = 4278.6 * pow(ccti,-1.2455);
      return((uint16_t)cct);
    }
    static void update(void *pvParameter);
};

#endif
