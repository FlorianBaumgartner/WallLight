/******************************************************************************
* file    FunctionSum.hpp
*******************************************************************************
* brief   Sum Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-03-03
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

#ifndef FUNCTION_SUM_H
#define FUNCTION_SUM_H

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class FunctionSum: public virtual Function
{
  private:
    Parameter parameterInputs[0] = {};
    Parameter parameterOutputs [0] = {};   
    Vector inputs[8] = {Vector("input 0"), Vector("input 1"), Vector("input 2"), Vector("input 3"), Vector("input 4"), Vector("input 5"), Vector("input 6"), Vector("input 7")};
    LedVector outputVectors[1] = {LedVector()};
    Vector outputs[1] = {Vector("output", &outputVectors[0])};
    LedVector* input[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

  public:
    static constexpr const char* MODULE_NAME = "Sum";
    FunctionSum(int32_t id): Function(id, MODULE_NAME) {}

    inline Parameter* getParameterInput(uint16_t index) {return (index < (sizeof(parameterInputs) / sizeof(Parameter)))? &parameterInputs[index] : nullptr;}
    inline Parameter* getParameterOutput(uint16_t index) {return (index < (sizeof(parameterOutputs) / sizeof(Parameter)))? &parameterOutputs[index] : nullptr;}
    inline uint32_t getParameterInputCount() {return (sizeof(parameterInputs) / sizeof(Parameter));}
    inline uint32_t getParameterOutputCount() {return (sizeof(parameterOutputs) / sizeof(Parameter));}
    inline Vector* getInput(uint16_t index) {return (index < (sizeof(inputs) / sizeof(Vector)))? &inputs[index] : nullptr;}
    inline Vector* getOutput(uint16_t index) {return (index < (sizeof(outputs) / sizeof(Vector)))? &outputs[index] : nullptr;}
    inline uint32_t getInputCount() {return (sizeof(inputs) / sizeof(Vector));}
    inline uint32_t getOutputCount() {return (sizeof(outputs) / sizeof(Vector));}

    bool init(bool allocateVector = false)
    {
      for(int i = 0; i < getInputCount(); i++)
      {
        input[i] = getInputValue(i);
      }
      if(!allocateVector)
      {
        bool allUnconnected = true;
        for(int i = 0; i < getInputCount(); i++)
        {
          if(LedVector::checkValid(input[i]))
          {
            allUnconnected = false;
            setOutput(0, input[i]);
            console.log.printf("[FUNCTION_SUM] Connect module output to input %d\n", i);
            break;
          }
        }
        if(allUnconnected)
        {
          setOutput(0, getOutputValue(0));
          allocateVector = true;      
          console.warning.println("[FUNCTION_SUM] No input connected");
        }
      }
      if(allocateVector)
      {
        getOutput(0)->allocate(0.0);
        setOutput(0, getOutputValue(0));
        console.log.println("[FUNCTION_SUM] Allocate local output buffer");
      }
      return initialized = true;
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
          for(int i = 0; i < getInputCount(); i++)
          {
            bool otherInputs = false;
            for(int j = 0; j < getInputCount(); j++)
            {
              otherInputs |= LedVector::checkValid(input[j]) && (j != i);
            }
            if(LedVector::checkValid(input[i]) && !otherInputs)
            {
              mirrorInput = input[i];
              break;
            }
          }
        }

        if(mirrorInput)
        {
          output->copy(mirrorInput);                          // Make deep copy of one of the inputs
        }
        else                                                  // Add valid inputs together
        {
          bool firstSummand = true;
          for(int inp = 0; inp < getInputCount(); inp++)
          {
            if(LedVector::checkValid(input[inp]))             // If no input is connected, the output vector stays 0.0
            {
              if(firstSummand)
              {
                firstSummand = false;
                output->copy(input[inp]);                     // Use first valid input as base for addition
              }
              else
              {
                for(int i = 0; i < PIXELCOUNT; i++)
                {
                  for(int c = 0; c < COLORCOUNT; c++)
                  {
                    output->value[c][i] += input[inp]->value[c][i];
                  }
                }
              }
            }
          }
        }
      }
      else error = true;
      return done();
    }
};

#ifdef log
  #undef log
#endif
#endif
