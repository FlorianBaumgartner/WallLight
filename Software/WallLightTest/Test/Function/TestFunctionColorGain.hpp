/******************************************************************************
* file    TestFunctionColorGain.hpp
*******************************************************************************
* brief   Test of Color Gain Function
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

#ifndef TEST_FUNCTION_COLOR_GAIN_HPP
#define TEST_FUNCTION_COLOR_GAIN_HPP

#include "../TestEngine.hpp"

class TestFunctionColorGain : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "ColorGain";

    static bool test(Engine* engine)
    {
      FunctionPdf* pdf0 = new FunctionPdf(0);
      pdf0->setParameterInput(0, new Coefficient(1000, 0.35));            // mean
      pdf0->setParameterInput(1, new Coefficient(1001, 0.01));            // variance

      FunctionPdf* pdf1 = new FunctionPdf(1);
      pdf1->setParameterInput(0, new Coefficient(1002, 0.65));            // mean
      pdf1->setParameterInput(1, new Coefficient(1003, 0.01));            // variance

      FunctionColorGain* colorGain0 = new FunctionColorGain(2);
      colorGain0->setParameterInput(0, new Coefficient(1004, 1.0));       // red
      colorGain0->setParameterInput(1, new Coefficient(1005, 0.0));       // green
      colorGain0->setParameterInput(2, new Coefficient(1006, 1.0));       // blue
      colorGain0->setInput(0, pdf0);

      FunctionColorGain* colorGain1 = new FunctionColorGain(3);
      colorGain1->setParameterInput(0, new Coefficient(1007, 0.0));       // red
      colorGain1->setParameterInput(1, new Coefficient(1008, 1.0));       // green
      colorGain1->setParameterInput(2, new Coefficient(1009, 1.0));       // blue
      colorGain1->setInput(0, pdf1);

      FunctionAdder* adder = new FunctionAdder(4);
      adder->setInput(0, colorGain0);
      adder->setInput(1, colorGain1);

      FunctionAdder* adderDemo = new FunctionAdder(5);              // Use this as a dummy to force a deep copy of the inputs
      adderDemo->setInput(0, colorGain0);
      adderDemo->setInput(1, colorGain1);

      bool status = true;
      Module* modules[] = {pdf0, pdf1, colorGain0, colorGain1, adder, adderDemo};
      status &= engine->setOutput(adder, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
