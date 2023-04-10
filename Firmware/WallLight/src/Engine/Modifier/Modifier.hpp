/******************************************************************************
* file    Modifier.hpp
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

#ifndef MODIFIER_HPP
#define MODIFIER_HPP

#include <Arduino.h>
#include "../Module.hpp"
#include "../../console.hpp"

#include "ModifierAdder.hpp"
#include "ModifierSubtractor.hpp"
#include "ModifierMultiplier.hpp"
#include "ModifierSum.hpp"
#include "ModifierProduct.hpp"
#include "ModifierMixer.hpp"
#include "ModifierSwitch.hpp"
#include "ModifierAbs.hpp"
#include "ModifierConstrain.hpp"
#include "ModifierEaseIn.hpp"
#include "ModifierEaseOut.hpp"
#include "ModifierEaseInOut.hpp"
#include "ModifierIntegrator.hpp"
#include "ModifierDifferentiator.hpp"
#include "ModifierExpSmoothing.hpp"
#include "ModifierSampler.hpp"
#include "ModifierMonoflop.hpp"


static Modifier* allocateModifier(const char* name, int32_t id)
{
  Modifier* module = nullptr;
  if(false){}
  
  if(strcmp(name, ModifierAdder::MODULE_NAME) == 0)
  {
    module = new ModifierAdder(id);
  }
  else if(strcmp(name, ModifierSubtractor::MODULE_NAME) == 0)
  {
    module = new ModifierSubtractor(id);
  }
  else if(strcmp(name, ModifierMultiplier::MODULE_NAME) == 0)
  {
    module = new ModifierMultiplier(id);
  }
  else if(strcmp(name, ModifierSum::MODULE_NAME) == 0)
  {
    module = new ModifierSum(id);
  }
  else if(strcmp(name, ModifierProduct::MODULE_NAME) == 0)
  {
    module = new ModifierProduct(id);
  }
  else if(strcmp(name, ModifierMixer::MODULE_NAME) == 0)
  {
    module = new ModifierMixer(id);
  }
  else if(strcmp(name, ModifierSwitch::MODULE_NAME) == 0)
  {
    module = new ModifierSwitch(id);
  }
  else if(strcmp(name, ModifierAbs::MODULE_NAME) == 0)
  {
    module = new ModifierAbs(id);
  }
  else if(strcmp(name, ModifierConstrain::MODULE_NAME) == 0)
  {
    module = new ModifierConstrain(id);
  }
  else if(strcmp(name, ModifierEaseIn::MODULE_NAME) == 0)
  {
    module = new ModifierEaseIn(id);
  }
  else if(strcmp(name, ModifierEaseOut::MODULE_NAME) == 0)
  {
    module = new ModifierEaseOut(id);
  }
  else if(strcmp(name, ModifierEaseInOut::MODULE_NAME) == 0)
  {
    module = new ModifierEaseInOut(id);
  }
  else if(strcmp(name, ModifierIntegrator::MODULE_NAME) == 0)
  {
    module = new ModifierIntegrator(id);
  }
  else if(strcmp(name, ModifierDifferentiator::MODULE_NAME) == 0)
  {
    module = new ModifierDifferentiator(id);
  }
  else if(strcmp(name, ModifierExpSmoothing::MODULE_NAME) == 0)
  {
    module = new ModifierExpSmoothing(id);
  }
  else if(strcmp(name, ModifierSampler::MODULE_NAME) == 0)
  {
    module = new ModifierSampler(id);
  }
  else if(strcmp(name, ModifierMonoflop::MODULE_NAME) == 0)
  {
    module = new ModifierMonoflop(id);
  }
  else
  {
    console.error.printf("[MODIFIER] Module '%s' is not supported!\n", name);
  }
  return module;
}

#endif
