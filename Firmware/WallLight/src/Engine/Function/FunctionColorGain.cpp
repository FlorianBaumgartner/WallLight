/******************************************************************************
* file    FunctionColorGain.cpp
*******************************************************************************
* brief   Color Gain Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-23
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

#include "FunctionColorGain.h"

bool FunctionColorGain::update(float time)
{
  if(t == time)                // Only calculate module content if time has updated
  {
    return true;
  }
  if(!Function::update(t))     // Check if all sources are available (modules that are connected have output value ready)
  {
    return false;
  }
  t = time;

  float r = getParameterValue(0);
  float g = getParameterValue(1);
  float b = getParameterValue(2);
  float ww = getParameterValue(3);
  float cw = getParameterValue(4);
  float am = getParameterValue(5);

/*  LedVector* vector = getInputValue(0);
  setOutput(0, vector);

  for(int i = 0; i < PIXELCOUNT; i++)
  {
    vector->value[LED_R][i] *= r;
    vector->value[LED_G][i] *= g;
    vector->value[LED_B][i] *= b;
    vector->value[LED_WW][i] *= ww;
    vector->value[LED_CW][i] *= cw;
    vector->value[LED_AM][i] *= am;
  }
  */
  return true;
}