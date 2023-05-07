/******************************************************************************
* file    TestExample_HeartBeat.hpp
*******************************************************************************
* brief   Example of Heart Beat
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-05-07
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

#ifndef TEST_EXAMPLE_HEART_BEAT_HPP
#define TEST_EXAMPLE_HEART_BEAT_HPP

#include "../TestEngine.hpp"

class TestExample_HeartBeat : public TestEngine
{
  public:
    static constexpr const char* TEST_NAME = "HeartBeat";

    static bool test(Engine* engine)
    {
      GeneratorRect* mainHeartBeat = new GeneratorRect(0);
      mainHeartBeat->setParameterInput(1, new Coefficient(1000, 0.6));              // Frequency
      mainHeartBeat->setParameterInput(3, new Coefficient(1001, 0.5));              // Amplitude
      mainHeartBeat->setParameterInput(4, new Coefficient(1002, 0.5));              // Offset
      mainHeartBeat->setParameterInput(6, new Coefficient(1003, 0.15));             // Duty Cycle

      GeneratorRamp* genRamp0 = new GeneratorRamp(1);
      genRamp0->setParameterInput(0, mainHeartBeat);                                // Enable
      genRamp0->setParameterInput(1, new Coefficient(1004, 3.0));                   // Frequency
      genRamp0->setParameterInput(3, new Coefficient(1005, 0.0));                   // Start
      genRamp0->setParameterInput(4, new Coefficient(1006, 1.0));                   // Stop
      genRamp0->setParameterInput(5, new Coefficient(1007, 0.0));                   // Phase

      ModifierMonoflop* mainHeartBeatDelay = new ModifierMonoflop(2);
      mainHeartBeatDelay->setParameterInput(0, mainHeartBeat);                      // Input
      mainHeartBeatDelay->setParameterInput(1, new Coefficient(1008, 0.5));         // Trigger Level
      mainHeartBeatDelay->setParameterInput(2, new Coefficient(1009, 0.3));         // Pulse Length
      mainHeartBeatDelay->setParameterInput(3, new Coefficient(1010, 0.0));         // Retrigger
      mainHeartBeatDelay->setParameterInput(4, new Coefficient(1011, 1.0));         // Nagtive Edge

      GeneratorRamp* genRamp1 = new GeneratorRamp(3);
      genRamp1->setParameterInput(0, mainHeartBeatDelay);                           // Enable
      genRamp1->setParameterInput(1, new Coefficient(1012, 3.0));                   // Frequency
      genRamp1->setParameterInput(3, new Coefficient(1013, 0.0));                   // Start
      genRamp1->setParameterInput(4, new Coefficient(1014, 1.2));                   // Stop
      genRamp1->setParameterInput(5, new Coefficient(1015, 0.0));                   // Phase

      ModifierAdder* genTriangleCombined = new ModifierAdder(4);
      genTriangleCombined->setParameterInput(0, genRamp0);
      genTriangleCombined->setParameterInput(1, genRamp1);

      ModifierExpSmoothing* expSmoothing = new ModifierExpSmoothing(5);
      expSmoothing->setParameterInput(0, genTriangleCombined);
      expSmoothing->setParameterInput(1, new Coefficient(1016, 0.15));              // Alpha

      ModifierMultiplier* variance = new ModifierMultiplier(6);
      variance->setParameterInput(0, expSmoothing);
      variance->setParameterInput(1, new Coefficient(1017, 0.05));

      FunctionPdf* pdf = new FunctionPdf(7);
      pdf->setParameterInput(0, new Coefficient(1018, 0.5));                        // Mean
      pdf->setParameterInput(1, variance);                                          // Variance

      FunctionRect* background = new FunctionRect(8);
      FunctionColorGain* backgroundColor = new FunctionColorGain(9);
      backgroundColor->setParameterInput(0, new Coefficient(1018, 0.4));
      backgroundColor->setParameterInput(2, new Coefficient(1019, 0.4));

      // backgroundColor->setParameterInput(3, new Coefficient(1020, 0.4));            // Amber
      backgroundColor->setInput(0, background);

      ModifierMultiplier* hue = new ModifierMultiplier(10);
      hue->setParameterInput(0, expSmoothing);
      hue->setParameterInput(1, new Coefficient(1021, 0.15/2));

      ModifierAdder* hueOffset = new ModifierAdder(11);
      hueOffset->setParameterInput(0, hue);
      hueOffset->setParameterInput(1, new Coefficient(1022, -0.05));

      ModifierHsvToRgb* hsvToRgb = new ModifierHsvToRgb(12);
      hsvToRgb->setParameterInput(0, hue);

      FunctionColorGain* colorGain = new FunctionColorGain(13);
      colorGain->setParameterInput(0, hsvToRgb, 0);
      colorGain->setParameterInput(1, hsvToRgb, 1);
      colorGain->setParameterInput(2, hsvToRgb, 2);
      colorGain->setInput(0, pdf);

      ModifierAdder* brightness = new ModifierAdder(14);
      brightness->setParameterInput(0, expSmoothing);
      brightness->setParameterInput(1, new Coefficient(1023, 0.5));

      FunctionBrightness* foreground = new FunctionBrightness(15);
      foreground->setParameterInput(0, brightness);
      foreground->setInput(0, colorGain);

      FunctionAdder* combinedOutput = new FunctionAdder(16);
      combinedOutput->setInput(0, foreground);
      // combinedOutput->setInput(1, backgroundColor);

      bool status = true;
      Module* modules[] = {mainHeartBeat, genRamp0, mainHeartBeatDelay, genRamp1, genTriangleCombined, expSmoothing, variance, pdf, background, backgroundColor, hue, hueOffset, hsvToRgb, colorGain, brightness, foreground, combinedOutput};
      status &= engine->setOutput(combinedOutput, 0);
      status &= engine->loadGraph(modules, sizeof(modules) / sizeof(Module*));
      return status;
    }
};

#endif
