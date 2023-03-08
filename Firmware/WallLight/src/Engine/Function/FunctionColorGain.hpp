/******************************************************************************
* file    FunctionColorGain.hpp
*******************************************************************************
* brief   Color Gain Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-23
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

#ifndef FUNCTION_COLOR_GAIN_HPP
#define FUNCTION_COLOR_GAIN_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class FunctionColorGain: public virtual Function
{
  private:
    Parameter parameterInputs[6] = {Parameter("r", 0.0),
                                    Parameter("g", 0.0),
                                    Parameter("b", 0.0),
                                    Parameter("ww", 0.0),
                                    Parameter("cw", 0.0),
                                    Parameter("am", 0.0)};

    Parameter parameterOutputs [0] = {};   

    Vector inputs[1] = {Vector("input")};
    LedVector outputVectors[1] = {LedVector()};
    Vector outputs[1] = {Vector("output", &outputVectors[0])};
    LedVector* input = nullptr;

  public:
    static constexpr const char* MODULE_NAME = "ColorGain";
    FunctionColorGain(int32_t id): Function(id, MODULE_NAME) {}
    ~FunctionColorGain() {}
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
        console.warning.println("[FUNCTION_COLOR_GAIN] No input connected");
      }
      if(deepCopy)
      {
        getOutput(0)->allocate(0.0);
        setOutput(0, getOutputValue(0));
        console.log.println("[FUNCTION_COLOR_GAIN] Allocate local output buffer");
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

      float r = getParameterValue(0);
      float g = getParameterValue(1);
      float b = getParameterValue(2);
      float ww = getParameterValue(3);
      float cw = getParameterValue(4);
      float am = getParameterValue(5);

      LedVector* output = getOutputValue(0);
      if(LedVector::checkValid(output))     
      {
        if(LedVector::checkValid(input))      // Only process data if a valid input is connected (otherwise a 0-vector is directly connected to the output)
        {
          for(int i = 0; i < PIXELCOUNT; i++)
          {
            output->value[LED_R][i] = input->value[LED_R][i] * r;
            output->value[LED_G][i] = input->value[LED_G][i] * g;
            output->value[LED_B][i] = input->value[LED_B][i] * b;
            output->value[LED_WW][i] = input->value[LED_WW][i] * ww;
            output->value[LED_CW][i] = input->value[LED_CW][i] * cw;
            output->value[LED_AM][i] = input->value[LED_AM][i] * am;
          }
        }
        else
        {
          output->fill(0.0);                  // Make sure to overwrite output vector when no input is connected (acts as constant 0-vector source)
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
