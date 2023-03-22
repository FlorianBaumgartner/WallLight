/******************************************************************************
* file    Utility.cpp
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

#include "Utility.hpp"


void Utility::hsvToRgb(float h, float s, float v, float* r, float* g, float* b)
{
  h = mod(h, 1.0) * 6.0;
  s = constrain(s, 0.0, 1.0);
  v = constrain(v, 0.0, 1.0);
  *r = 0.0;
  *g = 0.0;
  *b = 0.0;
  
  if(s == 0.0)
  {
    *r = *g = *b = v;
  }
  
  int j = (int)h;
  float f = h - j;
  float p = v * (1.0 - s);
  float q = v * (1.0 - s * f);
  float t = v * (1.0 - s * (1.0 - f));

  switch(j % 6)
  {
    case 0: *r = v; *g = t; *b = p; break;
    case 1: *r = q; *g = v; *b = p; break;
    case 2: *r = p; *g = v; *b = t; break;
    case 3: *r = p; *g = q; *b = v; break;
    case 4: *r = t; *g = p; *b = v; break;
    case 5: *r = v; *g = p; *b = q; break;
  }
}

void Utility::rgbToHsv(float r, float g, float b, float* h, float* s, float* v)
{
  r = constrain(r, 0.0, 1.0);
  g = constrain(g, 0.0, 1.0);
  b = constrain(b, 0.0, 1.0);
  *h = *s = *v = 0.0;

  float mx = max(r, max(g, b));
  float mn = min(r, min(g, b));
  float df = mx - mn;
  if(mx == mn)
  {
    *h = 0.0;
  }
  else if(mx == r)
  {
    *h = mod(60.0 * ((g - b) / df) + 360.0, 360.0);
  }
  else if(mx == g)
  {
    *h = mod(60.0 * ((b - r) / df) + 360.0, 360.0);
  }
  else if(mx == b)
  {
    *h = mod(60.0 * ((r - g) / df) + 360.0, 360.0);
  }
  *v = mx;
  *h /= 360.0;
}

