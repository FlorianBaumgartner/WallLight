/******************************************************************************
* file    TestFunction.hpp
*******************************************************************************
* brief   Virtual base class of all function tests
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-04-07
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

#ifndef TEST_FUNCTION_HPP
#define TEST_FUNCTION_HPP

#include "../TestEngine.hpp"

#include "TestFunctionRect.hpp"
#include "TestFunctionPulse.hpp"
#include "TestFunctionFire.hpp"
#include "TestFunctionAdder.hpp"
#include "TestFunctionSwitch.hpp"
#include "TestFunctionBrightness.hpp"
#include "TestFunctionPdf.hpp"


static bool testFunction(Engine* engine, const char* name)
{
  if(strcmp(name, TestFunctionRect::TEST_NAME) == 0)
  {
    return TestFunctionRect::test(engine);
  }
  else if(strcmp(name, TestFunctionPulse::TEST_NAME) == 0)
  {
    return TestFunctionPulse::test(engine);
  }
  else if(strcmp(name, TestFunctionFire::TEST_NAME) == 0)
  {
    return TestFunctionFire::test(engine);
  }
  else if(strcmp(name, TestFunctionAdder::TEST_NAME) == 0)
  {
    return TestFunctionAdder::test(engine);
  }
  else if(strcmp(name, TestFunctionSwitch::TEST_NAME) == 0)
  {
    return TestFunctionSwitch::test(engine);
  }
  else if(strcmp(name, TestFunctionBrightness::TEST_NAME) == 0)
  {
    return TestFunctionBrightness::test(engine);
  }
  else if(strcmp(name, TestFunctionPdf::TEST_NAME) == 0)
  {
    return TestFunctionPdf::test(engine);
  }
  else
  {
    console.error.printf("[TEST_FUNCTION] Function '%s' is not supported!\n", name);
  }
  return false;
}

#endif
