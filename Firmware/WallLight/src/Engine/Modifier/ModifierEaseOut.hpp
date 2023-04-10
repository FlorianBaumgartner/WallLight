/******************************************************************************
* file    ModifierEaseOut.hpp
*******************************************************************************
* brief   EaseOut Modifier
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

#ifndef MODIFIER_EASE_OUT_HPP
#define MODIFIER_EASE_OUT_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class ModifierEaseOut: public virtual Modifier
{
  private:
    Parameter parameterInputs[2] = {Parameter("input", 0.0),
                                    Parameter("type", 0.0)};
    
    Parameter parameterOutputs[1] = {Parameter("output")};

  public:
    static constexpr const char* MODULE_NAME = "EaseOut";
    ModifierEaseOut(int32_t id): Modifier(id, MODULE_NAME) {}
    ~ModifierEaseOut() {}
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

      float x = getParameterValue(0);
      uint8_t type = (uint8_t)(getParameterValue(1) + 0.5);
      float output = 0.0;
      if(x >= 1.0)
      {
        output = 1.0;
      }
      else if(x <= 0.0)
      {
        output = 0.0;
      }
      else
      {
        switch(type)
        {
          case 0:   // Sine
            output = sinf((x * PI) * 0.5);
            break;
          case 1:   // Quad
            output = 1.0 - (1.0 - x) * (1.0 - x);
            break;
          case 2:   // Cubic
            output = 1.0 - (1.0 - x) * (1.0 - x) * (1.0 - x);
            break;
          case 3:   // Quart
            output = 1.0 - (1.0 - x) * (1.0 - x) * (1.0 - x) * (1.0 - x);
            break;
          case 4:   // Quint
            output = 1.0 - (1.0 - x) * (1.0 - x) * (1.0 - x) * (1.0 - x) * (1.0 - x);
            break;
          case 5:   // Expo
            output = 1.0 - powf(2.0, -10.0 * x);
            break;
          case 6:   // Circ
            output = sqrtf(1.0 - (x - 1.0) * (x - 1.0));
            break;
          case 7:   // Back
            output = 1.0 + 2.70158 * (x - 1.0) * (x - 1.0) * (x - 1.0) + 1.70158 * (x - 1.0) * (x - 1.0);
            break;
          case 8:   // Elastic
            output = 1.0 - (1.0 - x) * (1.0 - x) * (1.0 - x) * (1.0 - x) * cosf(x * PI * 4.586) * 1.0377;     // Original: 4.5
            break;
          case 9:   // Bounce
            output = 1.0 - powf(2.0, -6.0 * x) * fabs(cosf(x * PI * 3.612)) * 1.065;                          // Original: 3.5
            break;
          default:
            output = 0.0;
            break;
        }
      }
      setParameterOutput(0, output);
      return true;
    }
};

#ifdef log
  #undef log
#endif
#endif
