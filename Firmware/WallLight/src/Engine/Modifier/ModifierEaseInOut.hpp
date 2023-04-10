/******************************************************************************
* file    ModifierEaseInOut.hpp
*******************************************************************************
* brief   EaseInOut Modifier
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

#ifndef MODIFIER_EASE_IN_OUT_HPP
#define MODIFIER_EASE_IN_OUT_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class ModifierEaseInOut: public virtual Modifier
{
  private:
    Parameter parameterInputs[2] = {Parameter("input", 0.0),
                                    Parameter("type", 0.0)};
    
    Parameter parameterOutputs[1] = {Parameter("output")};

  public:
    static constexpr const char* MODULE_NAME = "EaseInOut";
    ModifierEaseInOut(int32_t id): Modifier(id, MODULE_NAME) {}
    ~ModifierEaseInOut() {}
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
            output = -(cosf(x * PI) - 1.0) * 0.5;
            break;
          case 1:   // Quad
            output = (x < 0.5)? 2.0 * x * x : x * (4.0 - 2.0 * x) - 1.0;
            break;
          case 2:   // Cubic
            output = (x < 0.5)? 4.0 * x * x * x : 1.0 - (-2.0 * x + 2.0) * (-2.0 * x + 2.0) * (-2.0 * x + 2.0) * 0.5;
            break;
          case 3:   // Quart
            output = (x < 0.5)? 8.0 * x * x * x * x: 1.0 - (-2.0 * x + 2.0) * (-2.0 * x + 2.0) * (-2.0 * x + 2.0) * (-2.0 * x + 2.0) * 0.5;
            break;
          case 4:   // Quint
            output = (x < 0.5)? 16.0 * x * x * x * x * x: 1.0 - (-2.0 * x + 2.0) * (-2.0 * x + 2.0) * (-2.0 * x + 2.0) * (-2.0 * x + 2.0) * (-2.0 * x + 2.0) * 0.5;
            break;
          case 5:   // Expo
            output = (x < 0.5)? powf(2.0, 20.0 * x - 10.0) * 0.5 : (2.0 - powf(2.0, -20.0 * x + 10.0)) * 0.5;
            break;
          case 6:   // Circ
            output = (x < 0.5)? (1.0 - sqrtf(1.0 - (2.0 * x) * (2.0 * x))) * 0.5 : (sqrtf(1.0 - (-2.0 * x + 2.0) * (-2.0 * x + 2.0)) + 1.0) * 0.5;
            break;
          case 7:   // Back
            output = (x < 0.5)? ((2.0 * x) * (2.0 * x) * (7.1898 * x - 2.5949)) * 0.5 : ((2.0 * x - 2.0) * (2.0 * x - 2.0) * (3.5949 * (x * 2.0 - 2.0) + 2.5949) + 2.0) * 0.5;
            break;
          case 8:   // Elastic
            output = (x < 0.5)? -(powf(2.0, (20.0 * x - 10.0))) * sinf((20.0 * x - 11.125) * ((2.0 * PI) / 4.5)) * 0.5 : (powf(2.0, (-20.0 * x + 10.0)) * sinf((20.0 * x - 11.125) * ((2.0 * PI) / 4.5))) * 0.5 + 1.0;
            break;
          case 9:   // Bounce
            output = (x < 0.5)? 8.0 * powf(2.0, 8.0 * (x - 1.0)) * fabs(sinf(x * PI * 7.0)) : 1.0 - 8.0 * powf(2.0, -8.0 * x) * fabs(sinf(x * PI * 7.0));
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
