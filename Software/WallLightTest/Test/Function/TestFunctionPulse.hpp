/******************************************************************************
* file    TestFunctionPulse.hpp
*******************************************************************************
* brief   Test of Pulse Function
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

#ifndef TEST_FUNCTION_PULSE_HPP
#define TEST_FUNCTION_PULSE_HPP

#include "../TestEngine.hpp"

class TestFunctionPulse : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "Pulse";

    static bool test(Engine* engine)
    {
      GeneratorTriangle* triangle = new GeneratorTriangle(0);
      triangle->setParameterInput(0, new Coefficient(1000, 1.0));       // enable
      triangle->setParameterInput(1, new Coefficient(1001, 0.1));       // freq
      triangle->setParameterInput(2, new Coefficient(1002, -1.0));      // rep
      triangle->setParameterInput(3, new Coefficient(1003, 0.5));       // amplitude
      triangle->setParameterInput(4, new Coefficient(1004, 0.5));       // offset
      triangle->setParameterInput(5, new Coefficient(1005, 0.0));       // phase

      FunctionPulse* pulse = new FunctionPulse(1);
      pulse->setParameterInput(0, new Coefficient(1006, 0.5));           // position
      pulse->setParameterInput(1, triangle);
      pulse->setParameterInput(2, new Coefficient(1007, 0.0));           // low
      pulse->setParameterInput(3, new Coefficient(1008, 1.0));           // high
      pulse->setParameterInput(4, new Coefficient(1009, 1.0));           // smooth

      bool status = true;
      Module* modules[] = {triangle, pulse};
      status &= engine->setOutput(pulse, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
