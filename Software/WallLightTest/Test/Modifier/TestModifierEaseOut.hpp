/******************************************************************************
* file    TestModifierEaseOut.hpp
*******************************************************************************
* brief   Test of EaseOut Modifier
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-04-10
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

#ifndef TEST_MODIFIER_EASE_OUT_HPP
#define TEST_MODIFIER_EASE_OUT_HPP

#include "../TestEngine.hpp"

class TestModifierEaseOut : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "EaseOut";

    static bool test(Engine* engine)
    {
      float easeType = 9.0;

      GeneratorRamp* ramp = new GeneratorRamp(0);
      ramp->setParameterInput(0, new Coefficient(1000, 1.0));                 // enable
      ramp->setParameterInput(1, new Coefficient(1001, 1.0));                 // freq
      ramp->setParameterInput(2, new Coefficient(1002, 1.0));                 // rep
      ramp->setParameterInput(3, new Coefficient(1003, 0.0));                 // start
      ramp->setParameterInput(4, new Coefficient(1004, 1.0));                 // stop
      ramp->setParameterInput(5, new Coefficient(1005, 0.0));                 // phase

      ModifierEaseOut* easeOut = new ModifierEaseOut(1);
      easeOut->setParameterInput(0, ramp);                                     // input
      easeOut->setParameterInput(1, new Coefficient(1006, easeType));          // type

      FunctionRect* rect = new FunctionRect(2);
      rect->setParameterInput(0, new Coefficient(1006, 0.0));                  // start
      rect->setParameterInput(1, easeOut);                                     // stop

      bool status = true;
      Module* modules[] = {ramp, easeOut, rect};
      status &= engine->setOutput(rect, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
