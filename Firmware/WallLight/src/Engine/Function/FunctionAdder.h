/******************************************************************************
* file    FunctionAdder.h
*******************************************************************************
* brief   Adder Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-03-01
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

#ifndef FUNCTION_ADDER_H
#define FUNCTION_ADDER_H

#include <Arduino.h>
#include "../Module.h"

#define log   DISABLE_MODULE_LEVEL

class FunctionAdder: public virtual Function
{
  private:
    Parameter parameterInputs[0] = {};
    Parameter parameterOutputs [0] = {};   
    Vector inputs[2] = {Vector("input 0"), Vector("input 1")};
    LedVector outputVectors[1] = {LedVector()};
    Vector outputs[1] = {Vector("output", &outputVectors[0])};
    LedVector* input0 = nullptr;
    LedVector* input1 = nullptr;

  public:
    static constexpr const char* MODULE_NAME = "Adder";
    FunctionAdder(int32_t id): Function(id, MODULE_NAME) {}

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
      input0 = getInputValue(0);
      input1 = getInputValue(1);
      if(!allocateVector)
      {
        if(LedVector::checkValid(input0))
        {
          setOutput(0, input0);
          console.log.println("[FUNCTION_ADDER] Connect module output to input 0");
        }
        else if(LedVector::checkValid(input1))
        {
          setOutput(0, input1);
          console.log.println("[FUNCTION_ADDER] Connect module output to input 1");
        }
        else        // No input is connected to module, allocate local output vector
        {
          setOutput(0, getOutputValue(0));
          allocateVector = true;      
          console.warning.println("[FUNCTION_ADDER] No input connected");
        }
      }
      if(allocateVector)
      {
        getOutput(0)->allocate(0.0);
        setOutput(0, getOutputValue(0));
        console.log.println("[FUNCTION_ADDER] Allocate local output buffer");
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
          if(LedVector::checkValid(input0) && !LedVector::checkValid(input1))       // Only input 0 is available and needs to be copied
          {
            mirrorInput = input0;
          }
          if(LedVector::checkValid(input1) && !LedVector::checkValid(input0))       // Only input 1 is available and needs to be copied
          {
            mirrorInput = input1;
          }
        }

        if(mirrorInput)
        {
          for(int i = 0; i < PIXELCOUNT; i++)
          {
            for(int c = 0; c < COLORCOUNT; c++)
            {
              output->value[c][i] = mirrorInput->value[c][i];                       // Make deep copy of one of the inputs
            }
          }
        }
        else if(LedVector::checkValid(input0) && LedVector::checkValid(input1))     // Check if both inputs available
        {
          for(int i = 0; i < PIXELCOUNT; i++)
          {
            for(int c = 0; c < COLORCOUNT; c++)
            {
              output->value[c][i] = input0->value[c][i] + input1->value[c][i];
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
