/******************************************************************************
* file    FunctionRect.cpp
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

#ifndef FUNCTION_RECT_H
#define FUNCTION_RECT_H

#include <Arduino.h>
#include "../Module.h"

class FunctionRect: public virtual Function
{
  public:
    static constexpr const char* MODULE_NAME = "Rect";
    FunctionRect(int32_t id): Function(id, MODULE_NAME) {}
    bool update(float t);
  
  private:
    Parameter parameterInputs[5] = {Parameter("position", 0.5),
                                    Parameter("width", 1.0),
                                    Parameter("low", 0.0),
                                    Parameter("high", 1.0),
                                    Parameter("clip", 1.0)};
                                    
    const uint32_t parameterInputCount = sizeof(parameterInputs) / sizeof(Parameter);
    LedVector outputs[1] = {LedVector(true)};           // Always allocate output vector
    const uint32_t outputCount = sizeof(outputs) / sizeof(LedVector);
};


#endif
