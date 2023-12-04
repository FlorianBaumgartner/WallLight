/******************************************************************************
* file    TestExample_Techno2.hpp
*******************************************************************************
* brief   Example of Techno2 (Seraching Bugs in WallLight Engine)
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-11-18
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

#ifndef TEST_EXAMPLE_TECHNO2_HPP
#define TEST_EXAMPLE_TECHNO2_HPP

#include "../TestEngine.hpp"

class TestExample_Techno2 : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "Techno2";
    static const int MAX_DOTS = 4; // Maximum number of dots

    static bool testOld(Engine* engine)
    {
      Module* modules[MAX_DOTS * 4 + 1]; // Adjust the size based on the number of modules
      int moduleIndex = 0;
      int coefficientIndex = 1000; // Starting index for Coefficient

      for(int i = 0; i < MAX_DOTS; ++i) 
      {
        // Sine generator
        GeneratorSine* sine = new GeneratorSine(i);
        sine->setParameterInput(1, new Coefficient(coefficientIndex++, 0.03 * (i + 1))); // Frequency
        sine->setParameterInput(3, new Coefficient(coefficientIndex++, 0.5)); // Amplitude
        sine->setParameterInput(4, new Coefficient(coefficientIndex++, 0.5)); // Offset
        sine->setParameterInput(5, new Coefficient(coefficientIndex++, 0.0)); // Phase
        modules[moduleIndex++] = sine;

        // Dot function
        FunctionPdf* dot = new FunctionPdf(i + 4);
        dot->setParameterInput(0, sine);
        dot->setParameterInput(1, new Coefficient(coefficientIndex++, 0.0015));
        modules[moduleIndex++] = dot;

        // HSV to RGB modifier
        ModifierHsvToRgb* hsvToRgb = new ModifierHsvToRgb(i + 8);
        hsvToRgb->setParameterInput(0, new Coefficient(coefficientIndex++, float(i) * 0.08 + 0.7));
        hsvToRgb->setParameterInput(2, new Coefficient(coefficientIndex++, 1.0));
        modules[moduleIndex++] = hsvToRgb;

        // Color gain
        FunctionColorGain* color = new FunctionColorGain(i + 12);
        color->setInput(0, dot);
        color->setParameterInput(0, hsvToRgb, 0);
        color->setParameterInput(1, hsvToRgb, 1);
        color->setParameterInput(2, hsvToRgb, 2);
        modules[moduleIndex++] = color;
      }

      // Color blend function
      FunctionColorBlend* merge = new FunctionColorBlend(MAX_DOTS * 4);
      for (int i = 0; i < MAX_DOTS; ++i) 
      {
        merge->setInput(i, modules[i * 4 + 3]); // Assuming the last module in each group is the color gain
      }
      modules[moduleIndex++] = merge;

      bool status = true;
      status &= engine->setOutput(merge, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }

    static bool test(Engine* engine)
    {
      static const int MODULE_COUNT = MAX_DOTS * 4 + 1; // Adjust the size based on the number of modules
      Module* modules[MODULE_COUNT]; // Adjust the size based on the number of modules
      for(int i = 0; i < MODULE_COUNT; ++i) modules[i] = nullptr;
      int moduleIndex = 0;
      int coefficientIndex = 1000; // Starting index for Coefficient

      // Color blend function
      FunctionColorBlend* merge = new FunctionColorBlend(MAX_DOTS * 4);
      modules[moduleIndex++] = merge;

      for(int i = 0; i < MAX_DOTS; ++i) 
      {
        // Sine generator
        GeneratorSine* sine = new GeneratorSine(i);
        sine->setParameterInput(1, new Coefficient(coefficientIndex++, 0.03 * (i + 1))); // Frequency
        sine->setParameterInput(3, new Coefficient(coefficientIndex++, 0.5)); // Amplitude
        sine->setParameterInput(4, new Coefficient(coefficientIndex++, 0.5)); // Offset
        sine->setParameterInput(5, new Coefficient(coefficientIndex++, 0.0)); // Phase
        modules[moduleIndex++] = sine;

        // Dot function
        FunctionPdf* dot = new FunctionPdf(i + 4);
        dot->setParameterInput(0, sine);
        dot->setParameterInput(1, new Coefficient(coefficientIndex++, 0.0015));
        modules[moduleIndex++] = dot;

        // HSV to RGB modifier
        ModifierHsvToRgb* hsvToRgb = new ModifierHsvToRgb(i + 8);
        hsvToRgb->setParameterInput(0, new Coefficient(coefficientIndex++, float(i) * 0.08 + 0.7));
        hsvToRgb->setParameterInput(2, new Coefficient(coefficientIndex++, 1.0));
        modules[moduleIndex++] = hsvToRgb;

        // Color gain
        FunctionColorGain* color = new FunctionColorGain(i + 12);
        color->setInput(0, dot);
        color->setParameterInput(0, hsvToRgb, 0);
        color->setParameterInput(1, hsvToRgb, 1);
        color->setParameterInput(2, hsvToRgb, 2);
        modules[moduleIndex++] = color;

        merge->setInput(i, color);
        // merge->setInput(i, dot);

      }

      bool status = true;
      status &= engine->setOutput(merge, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }

};

#endif
