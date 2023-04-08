/******************************************************************************
* file    TestFunctionPdf.hpp
*******************************************************************************
* brief   Test of the Power Density Function
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

#ifndef TEST_FUNCTION_PDF_HPP
#define TEST_FUNCTION_PDF_HPP

#include "../TestEngine.hpp"

class TestFunctionPdf : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "Pdf";

    static bool test(Engine* engine)
    {
      FunctionPdf* pdf = new FunctionPdf(0);
      pdf->setParameterInput(0, new Coefficient(1000, 0.5));           // mean
      pdf->setParameterInput(1, new Coefficient(1001, 0.01));          // variance

      bool status = true;
      Module* modules[] = {pdf};
      status &= engine->setOutput(pdf, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif