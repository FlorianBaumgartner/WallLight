/******************************************************************************
* file    DataTypes.hpp
*******************************************************************************
* brief   Common Data Types for Modules
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-19
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

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <Arduino.h>
#include "WallLightConfig.hpp"
#include "Module.hpp"

class Module;   // Forward declaration of Module

class LedVector: public WallLightConfig
{
  private:
    bool allocated = false;

  public:
    float** value = nullptr;
    LedVector(bool allocateMemory = false, float defaultValue = 0.0)
    {
      if(allocateMemory)
      {
        allocate(defaultValue);
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
    bool allocate(float defaultValue = 0.0)
    {
      bool status = true;
      if(!allocated)
      {
        allocated = true;
        value = new float*[COLORCOUNT];
        status &= (value != nullptr);
        for(int i = 0; i < COLORCOUNT; i++) 
        {
          value[i] = new float[PIXELCOUNT];
          status &= (value[i] != nullptr);
          for(int j = 0; j < PIXELCOUNT; j++)
          {
            value[i][j] = defaultValue;
          }
        }
      }
      return status;
    }
    void fill(float vectorValue = 0.0)
    {
      for(int i = 0; i < COLORCOUNT; i++) 
      {
        for(int j = 0; j < PIXELCOUNT; j++)
        {
          value[i][j] = vectorValue;
        }
      }
    }
    bool fillPixel(uint16_t pos, float val)
    {
      if(pos < PIXELCOUNT)
      {
        for(int i = 0; i < COLORCOUNT; i++)
        {
          value[i][pos] = val;
        }
        return true;
      }
      return false;
    }
    bool copy(const LedVector* vector)
    {
      if(vector)
      {
        for(int i = 0; i < COLORCOUNT; i++) 
        {
          for(int j = 0; j < PIXELCOUNT; j++)
          {
            value[i][j] = vector->value[i][j];      // TODO: Maybe replace with memcopy?
          }
        }
        return true;
      }
      return false;
    }

    static bool checkValid(const LedVector* vector)
    {
      if(vector)
      {
        return vector->value != nullptr;
      }
      return false;
    }
};

class Parameter
{
  public:
    Parameter(const char* name, float defaultValue = 0.0, Module* module = nullptr, uint32_t sourceIndex = 0): name(name), value(defaultValue), module(module), sourceIndex(sourceIndex) {}
    const char* name;
    float value;
    Module* module;
    uint32_t sourceIndex;
};

class Vector
{
  public:
    Vector(const char* name, LedVector* defaultValue = nullptr, Module* module = nullptr, uint32_t sourceIndex = 0): name(name), value(defaultValue), module(module), sourceIndex(sourceIndex), allocated(false) {}
    const char* name;
    LedVector* value;
    Module* module;
    uint32_t sourceIndex;
    bool allocated;

    bool allocate(float defaultValue = 0.0)
    {
      allocated = value->allocate(defaultValue);
      return allocated;
    }
};

#endif