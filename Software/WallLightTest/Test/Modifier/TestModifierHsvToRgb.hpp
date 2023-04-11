/******************************************************************************
* file    TestModifierHsvToRgb.hpp
*******************************************************************************
* brief   Test of HSV-to-RGB Modifier
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-04-11
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

#ifndef TEST_MODIFIER_HSV_TO_RGB_HPP
#define TEST_MODIFIER_HSV_TO_RGB_HPP

#include "../TestEngine.hpp"

class TestModifierHsvToRgb : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "HsvToRgb";

    static bool test(Engine* engine)
    {
      ModifierHsvToRgb* hsvToRgb = new ModifierHsvToRgb(0);
      hsvToRgb->setParameterInput(0, new Coefficient(1000, 0.5));               // hue
      hsvToRgb->setParameterInput(1, new Coefficient(1001, 1.0));               // sat
      hsvToRgb->setParameterInput(2, new Coefficient(1002, 1.0));               // val

      FunctionRect* rect = new FunctionRect(1);

      FunctionColorGain* colorGain = new FunctionColorGain(2);
      colorGain->setParameterInput(0, hsvToRgb, 0);                             // red
      colorGain->setParameterInput(1, hsvToRgb, 1);                             // green
      colorGain->setParameterInput(2, hsvToRgb, 2);                             // blue
      colorGain->setInput(0, rect);

      bool status = true;
      Module* modules[] = {hsvToRgb, rect, colorGain};
      status &= engine->setOutput(colorGain, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
