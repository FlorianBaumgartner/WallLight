/******************************************************************************
* file    FunctionProduct.hpp
*******************************************************************************
* brief   Product Function
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

#ifndef FUNCTION_PRODUCT_HPP
#define FUNCTION_PRODUCT_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class FunctionProduct: public virtual Function
{
  private:
    Parameter parameterInputs[0] = {};
    Parameter parameterOutputs [0] = {};   
    Vector inputs[8] = {Vector("input 0"), Vector("input 1"), Vector("input 2"), Vector("input 3"), Vector("input 4"), Vector("input 5"), Vector("input 6"), Vector("input 7")};
    LedVector outputVectors[1] = {LedVector()};
    Vector outputs[1] = {Vector("output", &outputVectors[0])};
    LedVector* input[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

  public:
    static constexpr const char* MODULE_NAME = "Product";
    FunctionProduct(int32_t id): Function(id, MODULE_NAME) {}
    ~FunctionProduct() {}
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
      checkParameterInputs();         // Iterate over all parameter inputs to check if they are valid
      for(int i = 0; i < getInputCount(); i++)
      {
        input[i] = getInputValue(i);
      }
      if(!deepCopy)
      {
        bool allUnconnected = true;
        for(int i = 0; i < getInputCount(); i++)
        {
          if(LedVector::checkValid(input[i]))
          {
            allUnconnected = false;
            setOutput(0, input[i]);
            console.log.printf("[FUNCTION_PRODUCT] Connect module output to input %d\n", i);
            break;
          }
        }
        if(allUnconnected)
        {
          deepCopy = true;      
          console.warning.println("[FUNCTION_PRODUCT] No input connected");
        }
      }
      if(deepCopy)
      {
        getOutput(0)->allocate(0.0);
        setOutput(0, getOutputValue(0));
        console.log.println("[FUNCTION_PRODUCT] Allocate local output buffer");
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
          int numConnectedInputs = 0;
          for(int i = 0; i < getInputCount(); i++)
          {
            if(LedVector::checkValid(input[i]))
            {
              mirrorInput = input[i];
              numConnectedInputs++;
            }
          }
          if(numConnectedInputs > 1)                          // If more then one input is connected, the output cannot be directly mirrored
          {
            mirrorInput = nullptr;
          }
          else if(numConnectedInputs == 0)                    // If no input is connected -> set output to zero
          {
            output->fill(0.0);
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
                output->copy(input[inp]);                     // Use first valid input as base for multiplication
              }
              else
              {
                for(int i = 0; i < pixelcount(); i++)
                {
                  for(int c = 0; c < COLORCOUNT; c++)
                  {
                    output->value[c][i] *= input[inp]->value[c][i];
                  }
                }
              }
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
