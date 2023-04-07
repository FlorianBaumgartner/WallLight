/******************************************************************************
* file    TestFunctionBrightness.hpp
*******************************************************************************
* brief   Test of Brightness Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-04-07
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

#ifndef TEST_FUNCTION_BRIGHTNESS_HPP
#define TEST_FUNCTION_BRIGHTNESS_HPP

#include "../TestEngine.hpp"

class TestFunctionBrightness : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "Brightness";

    static bool test(Engine* engine)
    {
      FunctionRect* rect = new FunctionRect(0);

      FunctionBrightness* brightness = new FunctionBrightness(1);
      brightness->setInput(0, rect);
      brightness->setParameterInput(0, new Coefficient(1000, 0.5));     // brightness

      bool status = true;
      Module* modules[] = {rect, brightness};
      status &= engine->setOutput(brightness, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
