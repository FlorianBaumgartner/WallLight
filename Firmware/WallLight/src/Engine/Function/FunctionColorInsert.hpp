/******************************************************************************
* file    FunctionColorInsert.hpp
*******************************************************************************
* brief   Color Insert Function
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

#ifndef FUNCTION_COLOR_INSERT_HPP
#define FUNCTION_COLOR_INSERT_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class FunctionColorInsert: public virtual Function
{
  private:
    Parameter parameterInputs[7] = {Parameter("pos", 0.5),
                                    Parameter("r", 0.0),
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
    static constexpr const char* MODULE_NAME = "ColorInsert";
    FunctionColorInsert(int32_t id): Function(id, MODULE_NAME) {}
    ~FunctionColorInsert() {}
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
        console.warning.println("[FUNCTION_COLOR_INSERT] No input connected");
      }
      if(deepCopy)
      {
        getOutput(0)->allocate(-1.0);
        setOutput(0, getOutputValue(0));
        console.log.println("[FUNCTION_COLOR_INSERT] Allocate local output buffer");
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

      float pos = getParameterValue(0);
      float r = getParameterValue(1);
      float g = getParameterValue(2);
      float b = getParameterValue(3);
      float ww = getParameterValue(4);
      float cw = getParameterValue(5);
      float am = getParameterValue(6);

      LedVector* output = getOutputValue(0);
      if(LedVector::checkValid(output))     
      {
        if(LedVector::checkValid(input))      // Only process data if a valid input is connected (otherwise a -1.0 vector is directly connected to the output)
        {
          output->copy(getInput(0)->value);
        }
        else
        {
          output->fill(-1.0);                  // Make sure to overwrite output vector when no input is connected (acts as constant -1.0 vector source)
        }
        if(0.0 <= pos <= 1.0)
        {
          uint16_t discretePos = (uint16_t)(pos * (pixelcount() - 1) + 0.5);
          output->value[LED_R][discretePos] = r;
          output->value[LED_G][discretePos] = g;
          output->value[LED_B][discretePos] = b;
          output->value[LED_WW][discretePos] = ww;
          output->value[LED_CW][discretePos] = cw;
          output->value[LED_AM][discretePos] = am;
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
