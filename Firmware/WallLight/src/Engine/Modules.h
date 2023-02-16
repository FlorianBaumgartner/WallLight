/******************************************************************************
* file    Module.h
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

class LedVector
{
  private:
    const uint16_t PIXELCOUNT;
    const uint16_t COLORCOUNT;
    bool allocated = false;

  public:
    float** value = nullptr;
    LedVector(uint16_t pixelCount, uint16_t colorCount, bool allocateMemory = false): PIXELCOUNT(pixelCount), COLORCOUNT(colorCount), allocated(allocateMemory)
    {
      if(allocated)
      {
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
};

class Module
{
  public:
    Module(int32_t id);

    const char* revision = "0.1";       // TODO: Make dynamic
    const int32_t id;
    bool ready = false;
    float* parameterInputs = nullptr;
    float* parameterOutputs = nullptr;
    uint32_t parameterInputCount = 0;
    uint32_t parameterOutputCount = 0;

//   protected:

    bool update(float t);
    bool setParameterInput(int32_t id, Module &source, uint32_t sourceIndex = 0);


  private:
    

};

class Coefficient: public Module
{
  public:
    Coefficient();
    void setValue(float value) {value = value;};
    float getValue(void) {return value;};

  private:
    float value = 0.0;
};


#endif