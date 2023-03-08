/******************************************************************************
* file    Modules.hpp
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

#ifndef MODULES_HPP
#define MODULES_HPP

#include <Arduino.h>
#include "../console.hpp"
#include "WallLightConfig.hpp"
#include "DataTypes.hpp"

class Module: public WallLightConfig
{
  public:
    enum ModuleClass {MODULE_COEFFICIENT, MODULE_GENERATOR, MODULE_MODIFIER, MODULE_FUNCTION};
    struct Revision{int16_t major; int16_t minor;};
    static constexpr const Revision revision = {.major = 0, .minor = 1};     // TODO: Get from global configuration

    Module(int32_t id, ModuleClass moduleClass, const char* moduleName = nullptr, bool printInfo = true): id(id), moduleClass(moduleClass), moduleName(moduleName), printInfo(printInfo)
    {
      switch(moduleClass)
      {
        case MODULE_COEFFICIENT: className = "Coefficient"; break;
        case MODULE_GENERATOR:   className = "Generator"; break;
        case MODULE_MODIFIER:    className = "Modifier"; break;
        case MODULE_FUNCTION:    className = "Function"; break;
        default:                 className = "Unknown"; break;
      }
    }
    virtual ~Module() {}
    virtual bool update(float time) = 0;
    virtual Parameter* getParameterInput(uint16_t index) = 0;
    virtual Parameter* getParameterOutput(uint16_t index) = 0;
    virtual uint32_t getParameterInputCount() = 0;
    virtual uint32_t getParameterOutputCount() = 0;
    virtual bool init(bool deepCopy = false) = 0;
    virtual bool deinit(void) = 0;

    void printName() const
    {
      ConsoleColor color;
      switch(moduleClass)
      {
        case MODULE_COEFFICIENT: color = COLOR_YELLOW_BOLD; break;
        case MODULE_GENERATOR:   color = COLOR_BLUE_BOLD; break;
        case MODULE_MODIFIER:    color = COLOR_GREEN_BOLD; break;
        case MODULE_FUNCTION:    color = COLOR_MAGENTA_BOLD; break;
        default:                 color = COLOR_DEFAULT_BOLD; break;
      }
      console[color].print(className);
      if(moduleClass != MODULE_COEFFICIENT)
      {
        console.print('.'); console[COLOR_YELLOW_BOLD].print(moduleName);
      }
    }

    bool setParameterInput(uint16_t index, Module* source, uint32_t sourceIndex = 0)
    {
      if(index < getParameterInputCount())
      {
        Parameter* parameter = getParameterInput(index);
        if(parameter)
        {
          parameter->module = source;
          parameter->sourceIndex = sourceIndex;
          return true;
        }
      }
      console.error.printf("[MODULE] Could not set parameter input of '%s.%s' [sourceIndex: %d] (Module not correctly initialized or value out of bound)\n", className, moduleName, sourceIndex);
      return false;
    }

    const ModuleClass moduleClass;
    const char* className;
    const char* moduleName;
    const int32_t id;
    bool ready = false;
    bool error = false;
    bool initialized = false;
    bool printInfo;
    float t = NAN;

  protected:
    float getParameterValue(uint32_t index)
    {
      Parameter* parameter = getParameterInput(index);
      if(parameter)
      {
        if(parameter->module)   // Check if module exits
        {
          uint32_t outputCount = parameter->module->getParameterOutputCount();
          uint32_t sourceIndex = parameter->sourceIndex;    // Get parameterOutput index of source module
          if(outputCount > sourceIndex)
          {
            return parameter->module->getParameterOutput(sourceIndex)->value;
          }
          else
          {
            error = true;
            console.error.printf("[MODULE] Could not get parameter value of '%s.%s' (source index out of bound: %d of max: %d)\n", className, moduleName, sourceIndex, outputCount);                   
          }
        }
        if(printInfo)
        {
          console.printf("[MODULE] INFO: Parameter input \"%s\" '%d' of ", parameter->name, index); 
          printName();     
          console.printf(" [ID: %d] uses default value: %.2f\n", id, parameter->value);
        }
        return parameter->value;    // Default value of input parameter itself
      }
      error = true;
      console.error.printf("[MODULE] Could not access parameter of '%s.%s' (index out of bound: %d of max: %d)\n", className, moduleName, index, getParameterInputCount());
      return 0.0;
    }

    bool setParameterOutput(uint32_t index, float value)
    {
      if(getParameterOutputCount() > index)
      {
        getParameterOutput(index)->value = value;
        return true;
      }
      console.error.printf("[MODULE] Could not set parameter output value of Module '%s.%s' (output parameter index [%d] is out of bound [Max Size: %d])\n",
                           className, moduleName, index, getParameterOutputCount());
      return false;
    }

    bool getParameterStatus(void)
    {
      bool status = true;
      for(int i = 0; i < getParameterInputCount(); i++)
      {
        if(getParameterInput(i)->module)
        {
          status &= getParameterInput(i)->module->ready;
        }
      }
      return status;
    }

    void checkParameterInputs(void)
    {
      for(int i = 0; i < getParameterInputCount(); i++)
      {
        getParameterValue(i);   // This forces a message to be printed if a parameter input is not connected and uses default value
      }
    }

    bool initDone(void)
    {
      initialized = true;
      printInfo = false;
      return true;
    }
};

class Coefficient: public Module
{
  public:
    Coefficient(int32_t id, float defaultValue = 0.0): Module(id, MODULE_COEFFICIENT, "Coefficient") {value[0].value = defaultValue;}
    void setValue(float v) {value[0].value = v;};
    float getValue(void) {return value[0].value;};
    bool update(float time) {return ready = true;}
    Parameter* getParameterInput(uint16_t index) {return nullptr;}
    Parameter* getParameterOutput(uint16_t index) {return &value[0];}
    uint32_t getParameterInputCount() {return 0;}
    uint32_t getParameterOutputCount() {return 1;}
    bool init(bool deepCopy = false) {return initialized = true;}
    bool deinit(void) {return initialized = false;}
  private:
    Parameter value[1] = {Parameter("output")};
};

class Generator: public Module
{
  public:
    Generator(int32_t id, const char* moduleName): Module(id, MODULE_GENERATOR, moduleName) {}
    bool deinit(void) {return initialized = false;}
    bool update(float time)
    {
      ready = getParameterStatus();
      return ready;
    }
  protected:
    float enableTime = 0.0;
};

class Modifier: public Module
{
  public:
    Modifier(int32_t id, const char* moduleName): Module(id, MODULE_MODIFIER, moduleName) {}
    bool deinit(void) {return initialized = false;}
    bool update(float time)
    {
      ready = getParameterStatus();
      return ready;
    }
  private:
    
};

class Function: public Module
{
  public:
    Function(int32_t id, const char* moduleName): Module(id, MODULE_FUNCTION, moduleName) {}
    virtual Vector* getInput(uint16_t index) = 0;
    virtual Vector* getOutput(uint16_t index) = 0;
    virtual uint32_t getInputCount() = 0;
    virtual uint32_t getOutputCount() = 0;

    bool deinit(void)
    {
      for(int i = 0; i < getOutputCount(); i++)
      {
        getOutput(i)->free();       // Free all allocated output vectors (unallocated vactors are ignored)
      }
      return initialized = false;
    }

    bool getInputStatus(void)
    {
      bool status = true;
      for(int i = 0; i < getInputCount(); i++)
      {
        if(getInput(i)->module)
        {
          status &= getInput(i)->module->ready;
        }
      }
      return status;
    }

    bool update(float time)
    {
      ready = getParameterStatus() && getInputStatus();
      return ready && initialized;
    }

    bool setInput(uint16_t index, Module* source, uint32_t sourceIndex = 0)
    {
      if(index < getInputCount())
      {
        Vector* input = getInput(index);
        if(input)
        {
          input->module = source;
          input->sourceIndex = sourceIndex;
          return true;
        }
      }
      console.error.printf("[MODULE] Could not set input of '%s.%s' [sourceIndex: %d] (Module not correctly initialized or value out of bound)\n", className, moduleName, sourceIndex);
      return false;
    } 

    
  protected:
    LedVector* getInputValue(uint32_t index)
    {
      Vector* input = getInput(index);
      if(input)
      {
        if(input->module)   // Check if module exits
        {
          if(input->module->moduleClass == MODULE_FUNCTION)
          {
            uint32_t sourceIndex = input->sourceIndex;    // Get output index of source module
            if(((Function*)(input->module))->getOutputCount() > sourceIndex)
            {
              return ((Function*)(input->module))->getOutput(sourceIndex)->value;
            }
            else
            {
              error = true;
              console.error.printf("[MODULE] Could not get input value of '%s.%s' [sourceIndex: %d] (Module not correctly initialized or value out of bound)\n", className, moduleName, sourceIndex);                   
            }
          }
          else
          {
            error = true;
            console.error.printf("[MODULE] Requested Module '%s.%s' is not of type 'Function' and has no Vector Outputs\n", className, moduleName);
          }
        }
        if(printInfo)
        {
          console.printf("[MODULE] INFO: Input \"%s\" '%d' of ", input->name, index); 
          printName();     
          console.printf(" [ID: %d] uses default value\n", id);
        }
        return input->value;    // Default value of input parameter itself
      }
      error = true;
      console.error.printf("[MODULE] Could not access input of '%s.%s' (index out of bound: %d of max: %d)\n", className, moduleName, index, getInputCount());
      return nullptr;
    }

    LedVector* getOutputValue(uint32_t index)
    {
      if(index < getOutputCount())
      {
        return getOutput(index)->value;
      }
      console.error.printf("[MODULE] Could not get output value of '%s.%s' (output parameter index [%d] is out of bound [Max Size: %d])\n",
                           className, moduleName, index, getOutputCount());
      return nullptr;
    }

    bool setOutput(uint32_t index, LedVector* value)
    {
      if(getOutputCount() > index)
      {
        getOutput(index)->value = value;
        return true;
      }
      console.error.printf("[MODULE] Could not set output value of '%s.%s' (output parameter index [%d] is out of bound [Max Size: %d])\n",
                           className, moduleName, index, getOutputCount());
      return false;
    }
  private:
    
};

#endif
