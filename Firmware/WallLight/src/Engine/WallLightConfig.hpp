/******************************************************************************
* file    Config.hpp
*******************************************************************************
* brief   Configuration of WallLight acts as base class
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-16
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

#ifndef WALLLIGHT_CONFIG_HPP
#define WALLLIGHT_CONFIG_HPP

#include <Arduino.h>

class WallLightConfig
{
  public:
    WallLightConfig(uint16_t count = 0, float rate = 0.0)
    {
      if(count > 0)
      {
        pixelcount(count);
        framerate(rate);
      }
    }

    enum LED_COLOR {LED_R, LED_G, LED_B, LED_WW, LED_CW, LED_AM, COLORCOUNT};
    inline static uint16_t pixelcount(uint16_t c = 0)
    {
      static uint16_t count = 0;
      if(c > 0)
      {
        count = c;
      }
      return count;
    }
    inline static float framerate(float f = 0.0)
    {
      static float framerate = 0.0;
      if(f > 0.0)
      {
        framerate = f;
      }
      return framerate;
    }
};

#endif
