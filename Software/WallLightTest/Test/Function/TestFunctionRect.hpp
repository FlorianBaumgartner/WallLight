/******************************************************************************
* file    TestFunctionRect.hpp
*******************************************************************************
* brief   Test of Rect Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-03-29
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

#ifndef TEST_FUNCTION_RECT_HPP
#define TEST_FUNCTION_RECT_HPP

#include "../TestEngine.hpp"

void test(void)
{
  float enable = 1.0;
  float freq = 0.1;
  float rep = -1.0;
  float amplitude = 0.5;
  float offset = 0.5;
  float phase = 0.0;

  float start = 0.0;
  float low = 0.0;
  float high = 1.0;
  float smooth = 1.0;

  // triangle = Generator.Triangle(0)
  // triangle.setParameterInput(0, Coefficient(4, enable))
  // triangle.setParameterInput(1, Coefficient(5, freq))
  // triangle.setParameterInput(2, Coefficient(6, rep))
  // triangle.setParameterInput(3, Coefficient(7, amplitude))
  // triangle.setParameterInput(4, Coefficient(8, offset))
  // triangle.setParameterInput(5, Coefficient(9, phase))


}






// rect = Function.Rect(1)
// rect.setParameterInput(0, Coefficient(10, start))
// rect.setParameterInput(1, triangle)
// rect.setParameterInput(2, Coefficient(11, low))
// rect.setParameterInput(3, Coefficient(12, high))
// rect.setParameterInput(4, Coefficient(13, smooth))

// parameterPlotter = Analyzer.ParameterPlotter(2)
// parameterPlotter.setParameterInput(0, triangle)

// inputPlotter = Analyzer.InputPlotter(3)
// inputPlotter.setParameterInput(0, Coefficient(14, plotChannel))
// inputPlotter.setInput(0, rect)



#endif