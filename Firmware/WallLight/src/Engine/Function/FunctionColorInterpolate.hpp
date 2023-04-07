/******************************************************************************
* file    FunctionColorInterpolate.hpp
*******************************************************************************
* brief   Color Interpolate Function
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

#ifndef FUNCTION_COLOR_INTERPOLATE_HPP
#define FUNCTION_COLOR_INTERPOLATE_HPP

#include <Arduino.h>
#include "../Module.hpp"
#include "../Utility.hpp"

#define log   DISABLE_MODULE_LEVEL

class FunctionColorInterpolate: public virtual Function
{
  private:
    Parameter parameterInputs[1] = {Parameter("mode", 0.0)};
    Parameter parameterOutputs [0] = {};   

    Vector inputs[1] = {Vector("input")};
    LedVector outputVectors[1] = {LedVector()};
    Vector outputs[1] = {Vector("output", &outputVectors[0])};
    LedVector* input = nullptr;

  public:
    static constexpr const char* MODULE_NAME = "ColorInterpolate";
    FunctionColorInterpolate(int32_t id): Function(id, MODULE_NAME) {}
    ~FunctionColorInterpolate() {}
    inline Parameter* getParameterInput(uint16_t index) {return (index < (sizeof(parameterInputs) / sizeof(Parameter)))? &parameterInputs[index] : nullptr;}
    inline Parameter* getParameterOutput(uint16_t index) {return (index < (sizeof(parameterOutputs) / sizeof(Parameter)))? &parameterOutputs[index] : nullptr;}
    inline uint32_t getParameterInputCount() {return (sizeof(parameterInputs) / sizeof(Parameter));}
    inline uint32_t getParameterOutputCount() {return (sizeof(parameterOutputs) / sizeof(Parameter));}
    inline Vector* getInput(uint16_t index) {return (index < (sizeof(inputs) / sizeof(Vector)))? &inputs[index] : nullptr;}
    inline Vector* getOutput(uint16_t index) {return (index < (sizeof(outputs) / sizeof(Vector)))? &outputs[index] : nullptr;}
    inline uint32_t getInputCount() {return (sizeof(inputs) / sizeof(Vector));}
    inline uint32_t getOutputCount() {return (sizeof(outputs) / sizeof(Vector));}

    bool init(bool deepCopy = false)
    {
      checkParameterInputs();               // Iterate over all parameter inputs to check if they are valid
      input = getInputValue(0);
      if(!LedVector::checkValid(input))     // If no input is connected, allocate output vector
      {
        deepCopy = true;
        console.warning.println("[FUNCTION_COLOR_INTERPOLATE] No input connected");
      }
      if(deepCopy)
      {
        getOutput(0)->allocate(0.0);
        setOutput(0, getOutputValue(0));
        console.log.println("[FUNCTION_COLOR_INTERPOLATE] Allocate local output buffer");
      }
      else
      {
        setOutput(0, input);
      }
      return initDone();
    }

    bool update(float time)
    {
      if(t == time)                // Only calculate module content if time has updated
      {
        return true;
      }
      if(!Function::update(t))     // Check if all sources are available (modules that are connected have output value ready)
      {
        return false;
      }
      t = time;

      bool mode = getParameterValue(0) >= 0.5;

      LedVector* output = getOutputValue(0);
      if(LedVector::checkValid(output))     
      {
        if(LedVector::checkValid(input))      // Only process data if a valid input is connected (otherwise a -1.0 vector is directly connected to the output)
        {
          if(mode)
          {
            float* h = new float[pixelcount()];
            float* s = new float[pixelcount()];
            float* v = new float[pixelcount()];
            for(int i = 0; i < pixelcount(); i++)
            {
              float mask = constrain(input->value[LED_R][i] + 1.0, 0.0, 1.0);
              float r = constrain(input->value[LED_R][i], 0.0, 1.0);
              float g = constrain(input->value[LED_G][i], 0.0, 1.0);
              float b = constrain(input->value[LED_B][i], 0.0, 1.0);
              Utility::rgbToHsv(r, g, b, &h[i], &s[i], &v[i]);
              h[i] = (h[i] + 1.0) * mask - 1.0;
              s[i] = (s[i] + 1.0) * mask - 1.0;
              v[i] = (v[i] + 1.0) * mask - 1.0;
            }
            Utility::interpolate(h, h, pixelcount());
            Utility::interpolate(s, s, pixelcount());
            Utility::interpolate(v, v, pixelcount());
            for(int i = 0; i < pixelcount(); i++)
            {
              float r, g, b;
              Utility::hsvToRgb(h[i], s[i], v[i], &r, &g, &b);
              output->value[LED_R][i] = r;
              output->value[LED_G][i] = g;
              output->value[LED_B][i] = b;
            }
            delete h;
            delete s;
            delete v;
          }
          else
          {
            Utility::interpolate(input->value[LED_R], output->value[LED_R], pixelcount());
            Utility::interpolate(input->value[LED_G], output->value[LED_G], pixelcount());
            Utility::interpolate(input->value[LED_B], output->value[LED_B], pixelcount());
          }
          Utility::interpolate(input->value[LED_WW], output->value[LED_WW], pixelcount());
          Utility::interpolate(input->value[LED_CW], output->value[LED_CW], pixelcount());
          Utility::interpolate(input->value[LED_AM], output->value[LED_AM], pixelcount());
        }
        else
        {
          output->fill(0.0);                  // Make sure to overwrite output vector when no input is connected (acts as constant -1.0 vector source)
        }
      }
      else error = true;
      return true;
    }
};

#ifdef log
  #undef log
#endif
#endif
