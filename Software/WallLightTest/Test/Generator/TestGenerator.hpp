/******************************************************************************
* file    TestGenerator.hpp
*******************************************************************************
* brief   Virtual base class of all generator tests
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

#ifndef TEST_GENERATOR_HPP
#define TEST_GENERATOR_HPP

#include "../TestEngine.hpp"

#include "TestGeneratorSine.hpp"
#include "TestGeneratorRect.hpp"
#include "TestGeneratorRamp.hpp"
#include "TestGeneratorTriangle.hpp"
#include "TestGeneratorDirac.hpp"
#include "TestGeneratorRandom.hpp"


static bool testGenerator(Engine* engine, const char* name)
{
  if(strcmp(name, TestGeneratorSine::TEST_NAME) == 0)
  {
    return TestGeneratorSine::test(engine);
  }
  else if(strcmp(name, TestGeneratorRect::TEST_NAME) == 0)
  {
    return TestGeneratorRect::test(engine);
  }
  else if(strcmp(name, TestGeneratorRamp::TEST_NAME) == 0)
  {
    return TestGeneratorRamp::test(engine);
  }
  else if(strcmp(name, TestGeneratorTriangle::TEST_NAME) == 0)
  {
    return TestGeneratorTriangle::test(engine);
  }
  else if(strcmp(name, TestGeneratorDirac::TEST_NAME) == 0)
  {
    return TestGeneratorDirac::test(engine);
  }
  else if(strcmp(name, TestGeneratorRandom::TEST_NAME) == 0)
  {
    return TestGeneratorRandom::test(engine);
  }
  else
  {
    console.error.printf("[TEST_GENERATOR] Generator '%s' is not supported!\n", name);
  }
  return false;
}

#endif
