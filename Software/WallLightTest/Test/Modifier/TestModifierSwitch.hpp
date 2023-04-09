/******************************************************************************
* file    TestModifierSwitch.hpp
*******************************************************************************
* brief   Test of Switch Modifier
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-04-09
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

#ifndef TEST_MODIFIER_SWITCH_HPP
#define TEST_MODIFIER_SWITCH_HPP

#include "../TestEngine.hpp"

class TestModifierSwitch : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "Switch";

    static bool test(Engine* engine)
    {
      ModifierSwitch* switchModule = new ModifierSwitch(0);
      switchModule->setParameterInput(0, new Coefficient(1000, 0.2));     // input 0
      switchModule->setParameterInput(1, new Coefficient(1001, 0.8));     // input 1
      switchModule->setParameterInput(2, new Coefficient(1002, 0.0));     // switch

      FunctionRect* rect = new FunctionRect(1);
      rect->setParameterInput(0, new Coefficient(1003, 0.0));             // start
      rect->setParameterInput(1, switchModule);                           // stop

      bool status = true;
      Module* modules[] = {switchModule, rect};
      status &= engine->setOutput(rect, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
