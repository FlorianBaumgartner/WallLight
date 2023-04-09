/******************************************************************************
* file    FunctionSubtractor.hpp
*******************************************************************************
* brief   Subtractor Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-03-08
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

#ifndef FUNCTION_SUBTRACTOR_HPP
#define FUNCTION_SUBTRACTOR_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class FunctionSubtractor: public virtual Function
{
  private:
    Parameter parameterInputs[0] = {};
    Parameter parameterOutputs [0] = {};   
    Vector inputs[2] = {Vector("minuend"), Vector("subtrahend")};
    LedVector outputVectors[1] = {LedVector()};
    Vector outputs[1] = {Vector("output", &outputVectors[0])};
    LedVector* minuend = nullptr;
    LedVector* subtrahend = nullptr;

  public:
    static constexpr const char* MODULE_NAME = "Subtractor";
    FunctionSubtractor(int32_t id): Function(id, MODULE_NAME) {}
    ~FunctionSubtractor() {}
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
      minuend = getInputValue(0);
      subtrahend = getInputValue(1);
      if(!deepCopy)
      {
        if(LedVector::checkValid(minuend))
        {
          setOutput(0, minuend);
          console.log.printf("[FUNCTION_SUBTRACTOR] [ID: %d] Connect module output to minuend\n", id);
        }
        else        // No input is connected to module, allocate local output vector
        {
          deepCopy = true;      
          console.warning.printf("[FUNCTION_SUBTRACTOR] [ID: %d] No input connected\n", id);
        }
      }
      if(deepCopy)
      {
        getOutput(0)->allocate(0.0);
        setOutput(0, getOutputValue(0));
        console.log.printf("[FUNCTION_SUBTRACTOR] [ID: %d] Allocate local output buffer\n", id);
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

      LedVector* output = getOutputValue(0);
      if(LedVector::checkValid(output))
      {
        LedVector* mirrorInput = nullptr;
        if(getOutput(0)->allocated)
        {
          if(LedVector::checkValid(minuend) && !LedVector::checkValid(subtrahend))          // Only minuend is available and needs to be copied
          {
            mirrorInput = minuend;
          }
          else if(!LedVector::checkValid(minuend) && !LedVector::checkValid(subtrahend))    // No Input is connected -> set output to zero
          {
            output->fill(0.0);
          }
        }
        if(mirrorInput)
        {
          output->copy(mirrorInput);                                                        // Make deep copy of one of the inputs
        }
        else if(!LedVector::checkValid(minuend) && LedVector::checkValid(subtrahend))       // Only subtrahend available menas output = 0 - subtrahend
        {
          for(int i = 0; i < pixelcount(); i++)
          {
            for(int c = 0; c < COLORCOUNT; c++)
            {
              output->value[c][i] = -subtrahend->value[c][i];
            }
          }
        }
        else if(LedVector::checkValid(minuend) && LedVector::checkValid(subtrahend))        // Check if both inputs available
        {
          for(int i = 0; i < pixelcount(); i++)
          {
            for(int c = 0; c < COLORCOUNT; c++)
            {
              output->value[c][i] = minuend->value[c][i] - subtrahend->value[c][i];
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
