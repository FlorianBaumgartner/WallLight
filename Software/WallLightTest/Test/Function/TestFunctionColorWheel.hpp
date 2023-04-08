/******************************************************************************
* file    TestFunctionColorWheel.hpp
*******************************************************************************
* brief   Test of Color Wheel Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-04-08
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

#ifndef TEST_FUNCTION_COLOR_WHEEL_HPP
#define TEST_FUNCTION_COLOR_WHEEL_HPP

#include "../TestEngine.hpp"

class TestFunctionColorWheel : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "ColorWheel";

    static bool test(Engine* engine)
    {
      GeneratorRamp* ramp = new GeneratorRamp(0);
      ramp->setParameterInput(1, new Coefficient(1000, 0.1));           // freq
      ramp->setParameterInput(3, new Coefficient(1001, 0.0));           // amplitude
      ramp->setParameterInput(4, new Coefficient(1002, 1.0));           // offset

      FunctionColorWheel* colorWheel = new FunctionColorWheel(1);
      colorWheel->setParameterInput(0, new Coefficient(1003, 1.0));     // freq
      colorWheel->setParameterInput(1, ramp);
      colorWheel->setParameterInput(2, new Coefficient(1004, 1.0));     // saturation
      colorWheel->setParameterInput(3, new Coefficient(1005, 1.0));     // brightness

      bool status = true;
      Module* modules[] = {ramp, colorWheel};
      status &= engine->setOutput(colorWheel, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
