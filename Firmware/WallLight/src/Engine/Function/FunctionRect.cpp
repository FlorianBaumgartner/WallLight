/******************************************************************************
* file    FunctionRect.cpp
*******************************************************************************
* brief   Rectangle Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-19
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

#include "FunctionRect.h"

bool FunctionRect::update(float time)
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

  float start = getParameterValue(0);
  float stop = getParameterValue(1);
  float low = getParameterValue(2);
  float high = getParameterValue(3);
  bool smooth = getParameterValue(4) >= 0.5;

  if(start > stop)
  {
    float temp = start;
    start = stop;
    stop = temp;
  }

  outputs[0].value.fill(low);
  for(int i = 0; i < Module::PIXELCOUNT; i++)
  {
    if(((i + 0.5) > start * Module::PIXELCOUNT) && ((i + 0.5) <= stop * Module::PIXELCOUNT))
    {
      outputs[0].value.fillPixel(i, high);
    }
    if(smooth)
    {
      float startDif = start * Module::PIXELCOUNT - (float)i;
      float stopDif = stop * Module::PIXELCOUNT - (float)i;
      if((startDif > 0.0) && (startDif < 1.0))
      {
        outputs[0].value.fillPixel(i, low + (high - low) * startDif);
      }
      if((stopDif >= 0.0) && (stopDif < 1.0))
      {
        outputs[0].value.fillPixel(i, low + (high - low) * stopDif);
      }
    }
  }

  return true;
}