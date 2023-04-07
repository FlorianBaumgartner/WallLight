/******************************************************************************
* file    TestFunctionColorInsert.hpp
*******************************************************************************
* brief   Test of Color Insert Function
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

#ifndef TEST_FUNCTION_COLOR_INSERT_HPP
#define TEST_FUNCTION_COLOR_INSERT_HPP

#include "../TestEngine.hpp"

class TestFunctionColorInsert : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "ColorInsert";

    static bool test(Engine* engine)
    {
      float pos0 = 0.2;
      float pos1 = 0.5;
      float pos2 = 0.7;

      FunctionColorInsert* colorInsert0 = new FunctionColorInsert(0);
      colorInsert0->setParameterInput(0, new Coefficient(1000, pos0));         // position
      colorInsert0->setParameterInput(1, new Coefficient(1001, 1.0));          // Red
      colorInsert0->setParameterInput(2, new Coefficient(1002, 0.0));          // Green
      colorInsert0->setParameterInput(3, new Coefficient(1003, 1.0));          // Blue

      FunctionColorInsert* colorInsert1 = new FunctionColorInsert(1);
      colorInsert1->setParameterInput(0, new Coefficient(1004, pos1));         // position
      colorInsert1->setParameterInput(1, new Coefficient(1005, 0.0));          // Red
      colorInsert1->setParameterInput(2, new Coefficient(1006, 1.0));          // Green
      colorInsert1->setParameterInput(3, new Coefficient(1007, 1.0));          // Blue
      colorInsert1->setInput(0, colorInsert0);

      FunctionColorInsert* colorInsert2 = new FunctionColorInsert(2);
      colorInsert2->setParameterInput(0, new Coefficient(1008, pos2));         // position
      colorInsert2->setParameterInput(1, new Coefficient(1009, 1.0));          // Red
      colorInsert2->setParameterInput(2, new Coefficient(1010, 1.0));          // Green
      colorInsert2->setParameterInput(3, new Coefficient(1011, 0.0));          // Blue
      colorInsert2->setInput(0, colorInsert1);

      bool status = true;
      Module* modules[] = {colorInsert0, colorInsert1, colorInsert2};
      status &= engine->setOutput(colorInsert2, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
