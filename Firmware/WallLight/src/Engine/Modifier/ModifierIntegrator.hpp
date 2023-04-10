/******************************************************************************
* file    ModifierIntegrator.hpp
*******************************************************************************
* brief   Integrator Modifier
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

#ifndef MODIFIER_INTEGRATOR_HPP
#define MODIFIER_INTEGRATOR_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class ModifierIntegrator: public virtual Modifier
{
  private:
    Parameter parameterInputs[5] = {Parameter("input", 0.0),
                                    Parameter("gain", 1.0),
                                    Parameter("min", 0.0),
                                    Parameter("max", 0.0),
                                    Parameter("reset", 0.0)};
    
    Parameter parameterOutputs[1] = {Parameter("output")};
    float output = 0.0;

  public:
    static constexpr const char* MODULE_NAME = "Integrator";
    ModifierIntegrator(int32_t id): Modifier(id, MODULE_NAME) {}
    ~ModifierIntegrator() {}
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
      if(t < 0.0)
      {
        t = -1.0 / (float)framerate();
      }
      
      float input = getParameterValue(0);
      float gain = getParameterValue(1);
      float minValue = getParameterValue(2);
      float maxValue = getParameterValue(3);
      bool reset = getParameterValue(4) >= 0.5;

      if(Parameter::checkValid(getParameterInput(2)))   // Check if minimum value has been set
      {
        output = max(output, minValue);
      } 
      if(Parameter::checkValid(getParameterInput(3)))   // Check if maximum value has been set
      {
        output = min(output, maxValue);
      }
      if(reset)
      {
        output = 0.0;
      }
      setParameterOutput(0, output);
      output += input * (gain * (time - t));
      t = time;
      return true;
    }
};

#ifdef log
  #undef log
#endif
#endif
