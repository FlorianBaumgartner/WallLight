/******************************************************************************
* file    GeneratorSine.hpp
*******************************************************************************
* brief   Sine Generator
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

#ifndef GENERATOR_SINE_HPP
#define GENERATOR_SINE_HPP

#include <Arduino.h>
#include "../Module.hpp"
#include "../Utility.hpp"

#define log   DISABLE_MODULE_LEVEL

class GeneratorSine: public virtual Generator
{
  private:
    Parameter parameterInputs[6] = {Parameter("enable", 1.0),
                                    Parameter("freq", 1.0),
                                    Parameter("rep", -1.0),
                                    Parameter("amplitude", 1.0),
                                    Parameter("offset", 0.0),
                                    Parameter("phase", 0.0)};
    
    Parameter parameterOutputs[1] = {Parameter("output")};

  public:
    static constexpr const char* MODULE_NAME = "Sine";
    GeneratorSine(int32_t id): Generator(id, MODULE_NAME) {}
    ~GeneratorSine() {}
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
      if(!Generator::update(t))     // Check if all sources are available (modules that are connected have output value ready)
      {
        return false;
      }
      t = time;

      bool enable = getParameterValue(0) >= 0.5;
      float freq = getParameterValue(1);
      float rep = getParameterValue(2);
      float amplitude = getParameterValue(3);
      float offset = getParameterValue(4);
      float phase = getParameterValue(5);
      phase = Utility::mod((phase + 1.0), 2.0) - 1.0;

      if(enable)
      {
        t -= enableTime;
        if(rep > 0.0 && ((rep / freq) < t))
        {
          t = 0;
        }
      }
      else
      {
        enableTime = t;
        t = 0;
      }

      float x = t * 2.0 * PI * freq - phase * PI;
      float output = sinf(x) * amplitude + offset;
      setParameterOutput(0, output);
      return true;
    }
};

#ifdef log
  #undef log
#endif
#endif
