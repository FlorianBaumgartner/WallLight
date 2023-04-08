/******************************************************************************
* file    TestFunctionMixer.hpp
*******************************************************************************
* brief   Test of Mixer Function
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

#ifndef TEST_FUNCTION_MIXER_HPP
#define TEST_FUNCTION_MIXER_HPP

#include "../TestEngine.hpp"

class TestFunctionMixer : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "Mixer";

    static bool test(Engine* engine)
    {
      FunctionRect* rect0 = new FunctionRect(0);
      rect0->setParameterInput(0, new Coefficient(1000, 0.0));          // start
      rect0->setParameterInput(1, new Coefficient(1001, 0.25));         // stop

      FunctionRect* rect1 = new FunctionRect(1);
      rect1->setParameterInput(0, new Coefficient(1002, 0.75));         // start
      rect1->setParameterInput(1, new Coefficient(1003, 1.0));          // stop

      FunctionMixer* mixer = new FunctionMixer(2);
      mixer->setInput(0, rect0);
      mixer->setInput(1, rect1);
      mixer->setParameterInput(0, new Coefficient(1004, 0.3));          // mix

      FunctionMixer* mixerDemo = new FunctionMixer(3);                  // Use this as a dummy to force a deep copy of the inputs
      mixerDemo->setInput(0, rect0);
      mixerDemo->setInput(1, rect1);

      bool status = true;
      Module* modules[] = {rect0, rect1, mixer, mixerDemo};
      status &= engine->setOutput(mixer, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
