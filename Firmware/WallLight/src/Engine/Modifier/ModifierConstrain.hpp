/******************************************************************************
* file    ModifierConstrain.hpp
*******************************************************************************
* brief   Constrain Modifier
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

#ifndef MODIFIER_CONSTRAIN_HPP
#define MODIFIER_CONSTRAIN_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class ModifierConstrain: public virtual Modifier
{
  private:
    Parameter parameterInputs[3] = {Parameter("input", 0.0),
                                    Parameter("min", 0.0),
                                    Parameter("max", 0.0)};
    
    Parameter parameterOutputs[1] = {Parameter("output")};

  public:
    static constexpr const char* MODULE_NAME = "Constrain";
    ModifierConstrain(int32_t id): Modifier(id, MODULE_NAME) {}
    ~ModifierConstrain() {}
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

      float output = getParameterValue(0);
      float minimum = getParameterValue(1);
      float maximum = getParameterValue(2);
      if(Parameter::checkValid(getParameterInput(1)) && Parameter::checkValid(getParameterInput(2)))    // When both inputs are availabe swap min and max value if they are reversed
      {
        if(minimum > maximum)
        {
          float temp = minimum;
          minimum = maximum;
          maximum = temp;
        }
      }
      if(Parameter::checkValid(getParameterInput(1)))     // Check if minimum value has been set  
      {
        output = max(output, minimum);
      }
      if(Parameter::checkValid(getParameterInput(2)))     // Check if maximum value has been set  
      {
        output = min(output, maximum);
      }
      setParameterOutput(0, output);
      return true;
    }
};

#ifdef log
  #undef log
#endif
#endif
