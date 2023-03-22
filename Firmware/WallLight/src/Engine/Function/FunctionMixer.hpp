/******************************************************************************
* file    FunctionMixer.hpp
*******************************************************************************
* brief   Mixer Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-03-15
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

#ifndef FUNCTION_MIXER_HPP
#define FUNCTION_MIXER_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class FunctionMixer: public virtual Function
{
  private:
    Parameter parameterInputs[1] = {Parameter("mix", 0.0)};
    Parameter parameterOutputs [0] = {};   
    Vector inputs[2] = {Vector("input 0"), Vector("input 1")};
    LedVector outputVectors[1] = {LedVector()};
    Vector outputs[1] = {Vector("output", &outputVectors[0])};
    LedVector* input0 = nullptr;
    LedVector* input1 = nullptr;

  public:
    static constexpr const char* MODULE_NAME = "Mixer";
    FunctionMixer(int32_t id): Function(id, MODULE_NAME) {}
    ~FunctionMixer() {}
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
      input0 = getInputValue(0);
      input1 = getInputValue(1);
      if(!deepCopy)
      {
        if(LedVector::checkValid(input0))
        {
          setOutput(0, input0);
          console.log.printf("[FUNCTION_MIXER] [ID: %d] Connect module output to input 0\n", id);
        }
        else if(LedVector::checkValid(input1))
        {
          setOutput(0, input1);
          console.log.printf("[FUNCTION_MIXER] [ID: %d] Connect module output to input 1\n", id);
        }
        else        // No input is connected to module, allocate local output vector
        {
          deepCopy = true;      
          console.warning.printf("[FUNCTION_MIXER] [ID: %d] No input connected\n", id);
        }
      }
      if(deepCopy)
      {
        getOutput(0)->allocate(0.0);
        setOutput(0, getOutputValue(0));
        console.log.printf("[FUNCTION_MIXER] [ID: %d] Allocate local output buffer\n", id);
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

      float mix = getParameterValue(0);
      LedVector* output = getOutputValue(0);
      if(LedVector::checkValid(output))
      {
        LedVector* mirrorInput = nullptr;
        if(getOutput(0)->allocated && LedVector::checkValid(input0) && LedVector::checkValid(input1))     // Check if output is a deep copy and all inputs are available
        {
          if(mix >= 1.0)
          {
            mirrorInput = input1;
          }
          else if(mix <= 0.0)
          {
            mirrorInput = input0;
          }
        }
        mix = constrain(mix, 0.0, 1.0);
        if(mirrorInput)
        {
          output->copy(mirrorInput);                                                // Make deep copy of one of the inputs
        }
        else if(LedVector::checkValid(input0) && LedVector::checkValid(input1))     // Check if both inputs available
        {
          for(int i = 0; i < pixelcount(); i++)
          {
            for(int c = 0; c < COLORCOUNT; c++)
            {
              output->value[c][i] = input1->value[c][i] * mix + input0->value[c][i] * (1.0 - mix);
            }
          }
        }
        else if(LedVector::checkValid(input0))                                      // Only input 0 is available
        {
          for(int i = 0; i < pixelcount(); i++)
          {
            for(int c = 0; c < COLORCOUNT; c++)
            {
              output->value[c][i] = input0->value[c][i] * (1.0 - mix);
            }
          }
        }
        else if(LedVector::checkValid(input1))                                      // Only input 1 is available
        {
          for(int i = 0; i < pixelcount(); i++)
          {
            for(int c = 0; c < COLORCOUNT; c++)
            {
              output->value[c][i] = input1->value[c][i] * mix;
            }
          }
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
