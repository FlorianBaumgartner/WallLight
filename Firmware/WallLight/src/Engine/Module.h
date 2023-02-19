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
#include "WallLightConfig.h"

class LedVector: public WallLightConfig
{
  private:
    bool allocated = false;

  public:
    float** value = nullptr;
    LedVector(bool allocateMemory = false)
    {
      if(allocateMemory)
      {
        allocate();
      }
    }
    ~LedVector()
    {
      if(allocated)
      {
        for(int i = 0; i < COLORCOUNT; i++)
        {
          delete[] value[i];
        }
        delete[] value;
        value = nullptr;
      }
    }
    void allocate(void)
    {
      if(!allocated)
      {
        allocated = true;
        value = new float*[COLORCOUNT];
        for(int i = 0; i < COLORCOUNT; i++) 
        {
          value[i] = new float[PIXELCOUNT];
          for(int j = 0; j < PIXELCOUNT; j++)
          {
            value[i][j] = 0.0;
          }
        }
      }
    }
};

class Module: public WallLightConfig
{
  public:
    enum ModuleClass {MODULE_COEFFICIENT, MODULE_GENERATOR, MODULE_MODIFIER, MODULE_FUNCTION};

    Module(int32_t id, ModuleClass moduleClass, const char* moduleName = nullptr): id(id), moduleClass(moduleClass), moduleName(moduleName) {}

    const char* revision = "0.1";       // TODO: Make dynamic
    const ModuleClass moduleClass;
    const char* moduleName;
    const int32_t id;
    bool ready = false;
    float* parameterInputs = nullptr;
    float* parameterOutputs = nullptr;
    uint32_t parameterInputCount = 0;
    uint32_t parameterOutputCount = 0;

//   protected:

    virtual bool update(float t) = 0;
    bool setParameterInput(int32_t id, Module &source, uint32_t sourceIndex = 0);


  private:
    

};

class Coefficient: public Module
{
  public:
    Coefficient(int32_t id, float value = 0.0): Module(id, MODULE_COEFFICIENT, "Coefficient"), value(value) {}
    void setValue(float value) {value = value;};
    float getValue(void) {return value;};
    bool update(float t) {return true;}
  private:
    float value = 0.0;
};

class Generator: public Module
{
  public:
    Generator(int32_t id, const char* moduleName): Module(id, MODULE_GENERATOR, moduleName) {}
  private:
    float enabledTime = 0.0;
};

class Modifier: public Module
{
  public:
    Modifier(int32_t id, const char* moduleName): Module(id, MODULE_MODIFIER, moduleName) {}
  private:
    
};

class Function: public Module
{
  public:
    Function(int32_t id, const char* moduleName): Module(id, MODULE_FUNCTION, moduleName) {}
    LedVector** inputs = nullptr;
    LedVector** outputs = nullptr;
    uint32_t inputCount = 0;
    uint32_t outputCount = 0;
    
  private:
    
};

#endif
