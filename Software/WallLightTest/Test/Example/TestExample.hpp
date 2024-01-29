/******************************************************************************
* file    TestExample.hpp
*******************************************************************************
* brief   Virtual base class of all example tests
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-05-07
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

#ifndef TEST_EXAMPLE_HPP
#define TEST_EXAMPLE_HPP

#include "../TestEngine.hpp"

#include "TestExample_HeartBeat.hpp"
#include "TestExample_Techno2.hpp"

static bool testExample(Engine* engine, const char* name)
{
  if(strcmp(name, TestExample_HeartBeat::TEST_NAME) == 0)
  {
    return TestExample_HeartBeat::test(engine);
  }
  else if(strcmp(name, TestExample_Techno2::TEST_NAME) == 0)
  {
    return TestExample_Techno2::test(engine);
  }
  else
  {
    console.error.printf("[TEST_EXAMPLE] Example '%s' is not supported!\n", name);
  }
  return false;
}

#endif
