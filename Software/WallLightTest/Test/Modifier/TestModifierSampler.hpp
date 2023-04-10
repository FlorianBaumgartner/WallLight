/******************************************************************************
* file    TestModifierSampler.hpp
*******************************************************************************
* brief   Test of Sampler Modifier
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

#ifndef TEST_MODIFIER_SAMPLER_HPP
#define TEST_MODIFIER_SAMPLER_HPP

#include "../TestEngine.hpp"

class TestModifierSampler : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "Sampler";

    static bool test(Engine* engine)
    {
      float mode = 1.0;       // 0.0 = rising edge, 1.0 = falling edge, 2.0 = rising and falling edge

      GeneratorSine* sine = new GeneratorSine(0);
      GeneratorRect* rect = new GeneratorRect(1);
      rect->setParameterInput(1, new Coefficient(1000, 0.3));                   // freq

      ModifierSampler* sampler = new ModifierSampler(2);
      sampler->setParameterInput(0, sine);                                      // input
      sampler->setParameterInput(1, rect);                                      // trigger
      sampler->setParameterInput(2, new Coefficient(1001, mode));               // mode

      FunctionDirac* dirac0 = new FunctionDirac(3);
      dirac0->setParameterInput(0, sine);                                       // position
      dirac0->setParameterInput(2, new Coefficient(1003, 0.0));                 // smooth

      FunctionDirac* dirac1 = new FunctionDirac(4);
      dirac1->setParameterInput(0, sampler);                                    // position
      dirac1->setParameterInput(2, new Coefficient(1004, 0.0));                 // smooth

      FunctionColorGain* colorGain0 = new FunctionColorGain(5);
      colorGain0->setParameterInput(0, new Coefficient(1005, 1.0));             // red
      colorGain0->setParameterInput(1, new Coefficient(1006, 0.0));             // green
      colorGain0->setParameterInput(2, new Coefficient(1007, 0.0));             // blue
      colorGain0->setInput(0, dirac0);

      FunctionColorGain* colorGain1 = new FunctionColorGain(6);
      colorGain1->setParameterInput(0, new Coefficient(1008, 0.0));             // red
      colorGain1->setParameterInput(1, new Coefficient(1009, 1.0));             // green
      colorGain1->setParameterInput(2, new Coefficient(1010, 0.0));             // blue
      colorGain1->setInput(0, dirac1);

      FunctionAdder* adder = new FunctionAdder(7);
      adder->setInput(0, colorGain0);                                           // input 0
      adder->setInput(1, colorGain1);                                           // input 1

      bool status = true;
      Module* modules[] = {sine, rect, sampler, dirac0, dirac1, colorGain0, colorGain1, adder};
      status &= engine->setOutput(adder, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
