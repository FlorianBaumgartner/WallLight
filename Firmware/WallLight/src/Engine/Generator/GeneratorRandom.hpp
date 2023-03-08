/******************************************************************************
* file    GeneratorRandom.hpp
*******************************************************************************
* brief   Random Generator
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

#ifndef GENERATOR_RANDOM_HPP
#define GENERATOR_RANDOM_HPP

#include <Arduino.h>
#include "../Module.hpp"
#include "bootloader_random.h"

#define log   DISABLE_MODULE_LEVEL

class GeneratorRandom: public virtual Generator
{
  private:
    Parameter parameterInputs[3] = {Parameter("enable", 1.0),
                                    Parameter("min", 0.0),
                                    Parameter("max", 1.0)};
    
    Parameter parameterOutputs[1] = {Parameter("output")};

  public:
    static constexpr const char* MODULE_NAME = "Random";
    GeneratorRandom(int32_t id): Generator(id, MODULE_NAME) {}
    ~GeneratorRandom() {}
    inline Parameter* getParameterInput(uint16_t index) {return (index < (sizeof(parameterInputs) / sizeof(Parameter)))? &parameterInputs[index] : nullptr;}
    inline Parameter* getParameterOutput(uint16_t index) {return (index < (sizeof(parameterOutputs) / sizeof(Parameter)))? &parameterOutputs[index] : nullptr;}
    inline uint32_t getParameterInputCount() {return (sizeof(parameterInputs) / sizeof(Parameter));}
    inline uint32_t getParameterOutputCount() {return (sizeof(parameterOutputs) / sizeof(Parameter));}
    bool init(bool deepCopy = false)
    {
      checkParameterInputs();         // Iterate over all parameter inputs to check if they are valid
      bootloader_random_enable();     // Enable True Random Number Generator of ESP32 SoC
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
      float minimum = getParameterValue(1);
      float maximum = getParameterValue(2);

      float output = ((float)esp_random() / (float)UINT32_MAX) * fabs(maximum - minimum) + minimum;
      if(!enable)
      {
        output = 0.0;
      }
      setParameterOutput(0, output);
      return true;
    }
};

#ifdef log
  #undef log
#endif
#endif
