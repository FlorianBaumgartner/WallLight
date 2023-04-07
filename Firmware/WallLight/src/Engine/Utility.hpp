/******************************************************************************
* file    Utility.hpp
*******************************************************************************
* brief   General Utilities for WallLight Engine
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-03-21
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

#ifndef UTILITY_H
#define UTILITY_H

#include "Arduino.h"

class Utility
{
  public:
    static void hsvToRgb(float h, float s, float v, float* r, float* g, float* b);
    static void rgbToHsv(float r, float g, float b, float* h, float* s, float* v);
    static void interpolate(float* input, float* output, uint16_t count);
    static float mod(float a, float b) {return fmod((fmod(a, b) + b), b);}        // Module function that always returns the positive reminder (bahaves equal to the python % operator)
    static float rand(void);
};



#endif