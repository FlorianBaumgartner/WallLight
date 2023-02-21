/******************************************************************************
* file    Modules.h
*******************************************************************************
* brief   Graph Modul superclasses
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-16
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

#ifndef MODULES_H
#define MODULES_H

#include <Arduino.h>
#include "../console.h"
#include "WallLightConfig.h"
#include "DataTypes.h"

class Module: public WallLightConfig
{
  public:
    enum ModuleClass {MODULE_COEFFICIENT, MODULE_GENERATOR, MODULE_MODIFIER, MODULE_FUNCTION};
    struct Revision{int16_t major; int16_t minor;};
    static constexpr const Revision revision = {.major = 0, .minor = 1};     // TODO: Get from global configuration

    Module(int32_t id, ModuleClass moduleClass, const char* moduleName = nullptr, bool printInfo = true): id(id), moduleClass(moduleClass), moduleName(moduleName), printInfo(printInfo) {}

    const ModuleClass moduleClass;
    const char* moduleName;
    const int32_t id;
    bool ready = false;
    bool printInfo;
    Parameter* parameterInputs = nullptr;
    Parameter* parameterOutputs = nullptr;
    const uint32_t parameterInputCount = 0;
    const uint32_t parameterOutputCount = 0;

    void printName()
      {
        if(moduleClass == MODULE_GENERATOR) console[COLOR_BLUE_BOLD].print("Generator");
        else if(moduleClass == MODULE_MODIFIER) console[COLOR_GREEN_BOLD].print("Modifier");
        else if(moduleClass == MODULE_FUNCTION) console[COLOR_MAGENTA_BOLD].print("Function");
        else console[COLOR_DEFAULT_BOLD].print("Unknown");
        console.print('.'); console[COLOR_YELLOW_BOLD].print(moduleName);
      }

    virtual bool update(float t) = 0;
    bool setParameterInput(int32_t id, Module &source, uint32_t sourceIndex = 0);

  protected:
    float getParameterValue(uint32_t index)
    {
      if(parameterInputs[index].module)   // Check if module exits
      {
        uint32_t sourceIndex = parameterInputs[index].sourceIndex;    // Get parameterOutput index of source module
        if(parameterInputs[index].module->parameterOutputs && (parameterInputs[index].module->parameterOutputCount > sourceIndex))
        {
          return parameterInputs[index].module->parameterOutputs[sourceIndex].value;
        }
        else
        {
          console.error.printf("[MODULE] Could not get parameter value of Module '%s.%s' [sourceIndex: %d] (Module not correctly initialized or value out of bound)\n",
                               moduleClass, moduleName, sourceIndex);
        }
      }
      if(printInfo)
      {
        console.log.printf("[MODULE] INFO: Input ""%s"" [%d] of ", parameterInputs[index].name, index); 
        printName();     
        console.log.printf(" (ID: %d) uses default value: %.2f\n", id, parameterInputs[index].value);     
      }
      return parameterInputs[index].value;    // Default value of input parameter itself
    }

    bool setParameterOutput(uint32_t index, float value)
    {
      if(parameterOutputCount > index)
      {
        parameterOutputs[index].value = value;
        return true;
      }
      console.error.printf("[MODULE] Could not set parameter output value of Module '%s.%s' (output parameter index [%d] is out of bound [Max Size: %d])\n",
                           moduleClass, moduleName, index, parameterOutputCount);
      return false;
    }

    bool getParameterStatus(void)
    {
      bool status = true;
      for(int i = 0; i < parameterInputCount; i++)
      {
        if(parameterInputs[i].module)
        {
          status &= parameterInputs[i].module->ready;
        }
      }
      return status;
    }

  private:
    
    
};

class Coefficient: public Module
{
  public:
    Coefficient(int32_t id, float value = 0.0): Module(id, MODULE_COEFFICIENT, "Coefficient"), value(value) {}
    void setValue(float value) {value = value;};
    float getValue(void) {return value;};
    bool update(float t) {return ready = true;}
  private:
    float value = 0.0;
};

class Generator: public Module
{
  public:
    Generator(int32_t id, const char* moduleName): Module(id, MODULE_GENERATOR, moduleName) {}
    bool update(float t)
    {
      bool parameterStatus = getParameterStatus();
      printInfo = false;
      return parameterStatus;
    }
  protected:
    float enableTime = 0.0;
};

class Modifier: public Module
{
  public:
    Modifier(int32_t id, const char* moduleName): Module(id, MODULE_MODIFIER, moduleName) {}
    bool update(float t)
    {
      bool parameterStatus = getParameterStatus();
      printInfo = false;
      return parameterStatus;
    }
  private:
    
};

class Function: public Module
{
  public:
    Function(int32_t id, const char* moduleName): Module(id, MODULE_FUNCTION, moduleName) {}
    Vector* inputs = nullptr;
    Vector* outputs = nullptr;
    const uint32_t inputCount = 0;
    const uint32_t outputCount = 0;

    bool getInputStatus(void)
    {
      bool status = true;
      for(int i = 0; i < inputCount; i++)
      {
        if(inputs[i].module)
        {
          status &= inputs[i].module->ready;
        }
      }
      return status;
    }
    bool update(float t)
    {
      bool parameterStatus = getParameterStatus();
      bool inputStatus = getInputStatus();
      printInfo = false;
      return parameterStatus & inputStatus;
    }
    
  private:
    
};

#endif
