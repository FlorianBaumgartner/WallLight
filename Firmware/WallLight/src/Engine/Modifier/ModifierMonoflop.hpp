/******************************************************************************
* file    ModifierMonoflop.hpp
*******************************************************************************
* brief   Monoflop Modifier
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

#ifndef MODIFIER_MONOFLOP_HPP
#define MODIFIER_MONOFLOP_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class ModifierMonoflop: public virtual Modifier
{
  private:
    Parameter parameterInputs[5] = {Parameter("input", 0.0),
                                    Parameter("trigger level", 0.0),
                                    Parameter("pulse length", 0.0),
                                    Parameter("retrigger", 1.0),
                                    Parameter("modeh", 0.0)};
    
    Parameter parameterOutputs[1] = {Parameter("output")};
    enum Mode {EDGE_GREATER, EDGE_LESSER, EDGE_EQUAL};
    bool trigger = false;
    float triggerActiveTime = -1.0;
    float input = 0.0;

  public:
    static constexpr const char* MODULE_NAME = "Monoflop";
    ModifierMonoflop(int32_t id): Modifier(id, MODULE_NAME) {}
    ~ModifierMonoflop() {}
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

      float inputValue = getParameterValue(0);
      float triggerLevel = getParameterValue(1);
      float pulseLength = getParameterValue(2);
      bool retrigger = getParameterValue(3) >= 0.5;
      Mode mode = (Mode)(constrain((int)(getParameterValue(4) + 0.5), 0, 2));

      bool trig = false;
      switch(mode)
      {
        case EDGE_GREATER:
          if(inputValue > triggerLevel)
          {
            trig = true;
          }
          break;
        case EDGE_LESSER:
          if(inputValue < triggerLevel)
          {
            trig = true;
          }
          break;
        case EDGE_EQUAL:
          if(inputValue > triggerLevel && input < triggerLevel)
          {
            trig = true;
          }
          if(inputValue < triggerLevel && input > triggerLevel)
          {
            trig = true;
          }
          break;
        default:
          break;
      }
      if(((trig && !trigger) && (triggerActiveTime < t)) || retrigger)
      {
        triggerActiveTime = t + pulseLength;
      }
      trigger = trig;
      input = inputValue;

      float output = (triggerActiveTime >= t)? 1.0 : 0.0;
      setParameterOutput(0, output);
      return true;
    }
};

#ifdef log
  #undef log
#endif
#endif
