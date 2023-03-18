/******************************************************************************
* file    Function.hpp
*******************************************************************************
* brief   List of all Function Modules
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-18
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

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <Arduino.h>
#include "../Module.hpp"
#include "../../console.hpp"

#include "FunctionRect.hpp"
#include "FunctionTriangle.hpp"
#include "FunctionColorGain.hpp"
#include "FunctionAdder.hpp"
#include "FunctionSubtractor.hpp"
#include "FunctionMultiplier.hpp"
#include "FunctionSum.hpp"
#include "FunctionProduct.hpp"


static Function* allocateFunction(const char* name, int32_t id)
{
  Function* module = nullptr;
  if(strcmp(name, FunctionRect::MODULE_NAME) == 0)
  {
    module = new FunctionRect(id);
  }
  else if(strcmp(name, FunctionTriangle::MODULE_NAME) == 0)
  {
    module = new FunctionTriangle(id);
  }
  else if(strcmp(name, FunctionColorGain::MODULE_NAME) == 0)
  {
    module = new FunctionColorGain(id);
  }
  else if(strcmp(name, FunctionAdder::MODULE_NAME) == 0)
  {
    module = new FunctionAdder(id);
  }
  else if(strcmp(name, FunctionSubtractor::MODULE_NAME) == 0)
  {
    module = new FunctionSubtractor(id);
  }
  else if(strcmp(name, FunctionMultiplier::MODULE_NAME) == 0)
  {
    module = new FunctionMultiplier(id);
  }
  else if(strcmp(name, FunctionSum::MODULE_NAME) == 0)
  {
    module = new FunctionSum(id);
  }
  else if(strcmp(name, FunctionProduct::MODULE_NAME) == 0)
  {
    module = new FunctionProduct(id);
  }
  else
  {
    console.error.printf("[FUNCTION] Module '%s' is not supported!\n", name);
  }
  return module;
}

#endif