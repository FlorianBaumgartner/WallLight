/******************************************************************************
* file    ModifierSampler.hpp
*******************************************************************************
* brief   Sampler Modifier
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-04-10
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

#ifndef MODIFIER_SAMPLER_HPP
#define MODIFIER_SAMPLER_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class ModifierSampler: public virtual Modifier
{
  private:
    Parameter parameterInputs[3] = {Parameter("input", 0.0),
                                    Parameter("trigger", 0.0),
                                    Parameter("mode", 0.0)};
    
    Parameter parameterOutputs[1] = {Parameter("output")};
    enum Mode {MODE_RISING, MODE_FALLING, MODE_BOTH};
    float output = 0.0;
    bool trigger = false;

  public:
    static constexpr const char* MODULE_NAME = "Sampler";
    ModifierSampler(int32_t id): Modifier(id, MODULE_NAME) {}
    ~ModifierSampler() {}
    inline Parameter* getParameterInput(uint16_t index) {return (index < (sizeof(parameterInputs) / sizeof(Parameter)))? &parameterInputs[index] : nullptr;}
    inline Parameter* getParameterOutput(uint16_t index) {return (index < (sizeof(parameterOutputs) / sizeof(Parameter)))? &parameterOutputs[index] : nullptr;}
    inline uint32_t getParameterInputCount() {return (sizeof(parameterInputs) / sizeof(Parameter));}
    inline uint32_t getParameterOutputCount() {return (sizeof(parameterOutputs) / sizeof(Parameter));}
    bool init(bool deepCopy = false)
    {
      checkParameterInputs();         // Iterate over all parameter inputs to check if they are valid
      return initDone();
    }

    bool update(float time)
    {
      if(t == time)                 // Only calculate module content if time has updated
      {
        return true;
      }
      if(!Modifier::update(t))     // Check if all sources are available (modules that are connected have output value ready)
      {
        return false;
      }
      t = time;

      float input = getParameterValue(0);
      bool triggerInput = getParameterValue(1) >= 0.5;
      Mode mode = (Mode)(constrain((int)(getParameterValue(2) + 0.5), 0, 2));

      switch(mode)
      {
        case MODE_RISING:
          if(!trigger && triggerInput)
          {
            output = input;
          }
          break;
        case MODE_FALLING:
          if(trigger && !triggerInput)
          {
            output = input;
          }
          break;
        case MODE_BOTH:
          if(trigger != triggerInput)
          {
            output = input;
          }
          break;
        default:
          break;
      }
      trigger = triggerInput;
      setParameterOutput(0, output);
      return true;
    }
};

#ifdef log
  #undef log
#endif
#endif
