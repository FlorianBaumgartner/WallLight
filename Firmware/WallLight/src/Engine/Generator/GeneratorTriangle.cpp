/******************************************************************************
* file    GeneratorTrangle.cpp
*******************************************************************************
* brief   Triangle Generator
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-18
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

#include "GeneratorTriangle.h"

bool GeneratorTriangle::update(float t)
{
  // TODO: Check if all sources are avilable (modules that are connect have output state ready)

  bool enable = getParameterValue(0) >= 0.5;
  float freq =  getParameterValue(1);
  float rep =  getParameterValue(2);
  float amplitude =  getParameterValue(3);
  float offset =  getParameterValue(4);
  float phase =  getParameterValue(5);
  phase = fmod((phase + 1.0), 2.0) - 1.0;

  if(enable)
  {
    t -= enableTime;
    if(rep > 0.0 && ((rep / freq) < t))
    {
      t = 0;
    }
  }
  else
  {
    enableTime = t;
    t = 0;
  }

  float x = 1.0 - fabs(fmod((t * freq - (phase / freq)), 1.0) * 2.0 - 1.0);
  float output = (x - 0.5) * 2 * amplitude + offset;
  setParameterOutput(0, output);
  return true;
}