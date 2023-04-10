/******************************************************************************
* file    TestModifier.hpp
*******************************************************************************
* brief   Virtual base class of all modifier tests
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

#ifndef TEST_MODIFIER_HPP
#define TEST_MODIFIER_HPP

#include "../TestEngine.hpp"

#include "TestModifierAdder.hpp"
#include "TestModifierSubtractor.hpp"
#include "TestModifierMultiplier.hpp"
#include "TestModifierSum.hpp"
#include "TestModifierProduct.hpp"
#include "TestModifierMixer.hpp"
#include "TestModifierSwitch.hpp"
#include "TestModifierAbs.hpp"
#include "TestModifierConstrain.hpp"
#include "TestModifierEaseIn.hpp"


static bool testModifier(Engine* engine, const char* name)
{
  if(strcmp(name, TestModifierAdder::TEST_NAME) == 0)
  {
    return TestModifierAdder::test(engine);
  }
  else if(strcmp(name, TestModifierSubtractor::TEST_NAME) == 0)
  {
    return TestModifierSubtractor::test(engine);
  }
  else if(strcmp(name, TestModifierMultiplier::TEST_NAME) == 0)
  {
    return TestModifierMultiplier::test(engine);
  }
  else if(strcmp(name, TestModifierSum::TEST_NAME) == 0)
  {
    return TestModifierSum::test(engine);
  }
  else if(strcmp(name, TestModifierProduct::TEST_NAME) == 0)
  {
    return TestModifierProduct::test(engine);
  }
  else if(strcmp(name, TestModifierMixer::TEST_NAME) == 0)
  {
    return TestModifierMixer::test(engine);
  }
  else if(strcmp(name, TestModifierSwitch::TEST_NAME) == 0)
  {
    return TestModifierSwitch::test(engine);
  }
  else if(strcmp(name, TestModifierAbs::TEST_NAME) == 0)
  {
    return TestModifierAbs::test(engine);
  }
  else if(strcmp(name, TestModifierConstrain::TEST_NAME) == 0)
  {
    return TestModifierConstrain::test(engine);
  }
  else if(strcmp(name, TestModifierEaseIn::TEST_NAME) == 0)
  {
    return TestModifierEaseIn::test(engine);
  }
  else
  {
    console.error.printf("[TEST_MODIFIER] Modifier '%s' is not supported!\n", name);
  }
  return false;
}

#endif
