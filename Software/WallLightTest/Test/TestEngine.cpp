/******************************************************************************
* file    TestEngine.cpp
*******************************************************************************
* brief   Graph Test Engine
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-03-29
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

#include "TestEngine.hpp"

#include "Generator/TestGenerator.hpp"
#include "Modifier/TestModifier.hpp"
#include "Function/TestFunction.hpp"
#include "Example/TestExample.hpp"

bool TestEngine::loadtest(const char* name)
{
  char* separator = strchr(name, '.');      // Find the separator symbol '.'
  if (separator == nullptr)
  {
    console.error.printf("[TEST_ENGINE] The desired test name is not correctly formated: %s\n", name);
    return false;
  }
  int prefixLength = separator - name;
  char* moduleType = separator + 1;

  if(strncmp(name, "Generator", prefixLength) == 0)
  {
    return testGenerator(engine, moduleType);
  }
  else if(strncmp(name, "Modifier", prefixLength) == 0)
  {
    return testModifier(engine, moduleType);
  }
  else if(strncmp(name, "Function", prefixLength) == 0)
  {
    return testFunction(engine, moduleType);
  }
  else if(strncmp(name, "Example", prefixLength) == 0)
  {
    return testExample(engine, moduleType);
  }
  else
  {
    console.warning.printf("[TEST_ENGINE] Module class type '%s' is not supported\n", name);
  }
  return false;
}