/******************************************************************************
* file    FunctionRect.hpp
*******************************************************************************
* brief   Rectangle Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-19
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

#ifndef FUNCTION_RECT_HPP
#define FUNCTION_RECT_HPP

#include <Arduino.h>
#include "../Module.hpp"

// #define log   DISABLE_MODULE_LEVEL

class FunctionRect: public virtual Function
{
  private:
    Parameter parameterInputs[5] = {Parameter("start", 0.0),
                                    Parameter("stop", 1.0),
                                    Parameter("low", 0.0),
                                    Parameter("high", 1.0),
                                    Parameter("smooth", 1.0)};

    Parameter parameterOutputs [0] = {};                                

    Vector inputs[0] = {};
    LedVector outputVectors[1] = {LedVector()};     
    Vector outputs[1] = {Vector("output", &outputVectors[0])};  
    
  public:
    static constexpr const char* MODULE_NAME = "Rect";
    FunctionRect(int32_t id): Function(id, MODULE_NAME) {}

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
      getOutput(0)->allocate(0.0);    // Always allocate an output vector for this module 
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

      float start = getParameterValue(0);
      float stop = getParameterValue(1);
      float low = getParameterValue(2);
      float high = getParameterValue(3);
      bool smooth = getParameterValue(4) >= 0.5;

      if(start > stop)
      {
        float temp = start;
        start = stop;
        stop = temp;
      }
      
      LedVector* output = getOutputValue(0);
      if(LedVector::checkValid(output))
      {
        output->fill(low);
        for(int i = 0; i < Module::PIXELCOUNT; i++)
        {
          if(((i + 0.5) > start * Module::PIXELCOUNT) && ((i + 0.5) <= stop * Module::PIXELCOUNT))
          {
            output->fillPixel(i, high);
          }
          if(smooth)
          {
            float startDif = start * Module::PIXELCOUNT - (float)i;
            float stopDif = stop * Module::PIXELCOUNT - (float)i;
            if((startDif > 0.0) && (startDif < 1.0))
            {
              output->fillPixel(i, low + (high - low) * startDif);
            }
            if((stopDif >= 0.0) && (stopDif < 1.0))
            {
              output->fillPixel(i, low + (high - low) * stopDif);
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
