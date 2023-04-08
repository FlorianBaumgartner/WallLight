/******************************************************************************
* file    TestFunctionFire.hpp
*******************************************************************************
* brief   Test of Fire Function
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

#ifndef TEST_FUNCTION_FIRE_HPP
#define TEST_FUNCTION_FIRE_HPP

#include "../TestEngine.hpp"

class TestFunctionFire : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "Fire";

    static bool test(Engine* engine)
    {
      FunctionFire* fire = new FunctionFire(0);
      fire->setParameterInput(0, new Coefficient(1000, 0.5));           // intensity
      fire->setParameterInput(1, new Coefficient(1001, 0.85));          // ignition
      fire->setParameterInput(2, new Coefficient(1002, 0.5));           // cooling
      fire->setParameterInput(3, new Coefficient(1003, 0.5));           // speed
      fire->setParameterInput(4, new Coefficient(1004, 0.2));           // acceleration

      bool status = true;
      Module* modules[] = {fire};
      status &= engine->setOutput(fire, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif