/******************************************************************************
* file    TestModifierConstrain.hpp
*******************************************************************************
* brief   Test of Constrain Modifier
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

#ifndef TEST_MODIFIER_CONSTRAIN_HPP
#define TEST_MODIFIER_CONSTRAIN_HPP

#include "../TestEngine.hpp"

class TestModifierConstrain : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "Constrain";

    static bool test(Engine* engine)
    {
      GeneratorSine* sine = new GeneratorSine(0);
      sine->setParameterInput(3, new Coefficient(1000, 0.5));                 // amplitude
      sine->setParameterInput(4, new Coefficient(1001, 0.5));                 // offset

      ModifierConstrain* constrainModule = new ModifierConstrain(1);
      constrainModule->setParameterInput(0, sine);                            // input
      constrainModule->setParameterInput(1, new Coefficient(1002, 0.3));      // minimum
      constrainModule->setParameterInput(2, new Coefficient(1003, 0.7));      // maximum

      FunctionRect* rect = new FunctionRect(2);
      rect->setParameterInput(0, new Coefficient(1004, 0.0));                 // start
      rect->setParameterInput(1, constrainModule);                            // stop

      bool status = true;
      Module* modules[] = {sine, constrainModule, rect};
      status &= engine->setOutput(rect, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
