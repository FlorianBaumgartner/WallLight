/******************************************************************************
* file    ModifierProduct.hpp
*******************************************************************************
* brief   Product Modifier
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-04-09
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

#ifndef MODIFIER_PRODUCT_HPP
#define MODIFIER_PRODUCT_HPP

#include <Arduino.h>
#include "../Module.hpp"

#define log   DISABLE_MODULE_LEVEL

class ModifierProduct: public virtual Modifier
{
  private:
    Parameter parameterInputs[8] = {Parameter("input 0", 1.0),
                                    Parameter("input 1", 1.0),
                                    Parameter("input 2", 1.0),
                                    Parameter("input 3", 1.0),
                                    Parameter("input 4", 1.0),
                                    Parameter("input 5", 1.0),
                                    Parameter("input 6", 1.0),
                                    Parameter("input 7", 1.0)};
    
    Parameter parameterOutputs[1] = {Parameter("output")};

  public:
    static constexpr const char* MODULE_NAME = "Product";
    ModifierProduct(int32_t id): Modifier(id, MODULE_NAME) {}
    ~ModifierProduct() {}
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

      uint16_t connectedParameterCount = 0;
      float output = 1.0;
      for(int i = 0; i < getParameterInputCount(); i++)
      {
        output *= getParameterValue(i);
        if(Parameter::checkValid(getParameterInput(i)))
        {
          connectedParameterCount++;
        }
      }
      if(connectedParameterCount == 0)      // If no parameter input is connected -> set output to zero
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
