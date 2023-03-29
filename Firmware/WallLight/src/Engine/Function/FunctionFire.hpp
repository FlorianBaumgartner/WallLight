/******************************************************************************
* file    FunctionFire.hpp
*******************************************************************************
* brief   Fire Function
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-03-28
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

#ifndef FUNCTION_FIRE_HPP
#define FUNCTION_FIRE_HPP

#include <Arduino.h>
#include "../Module.hpp"
#include "../Utility.hpp"

#define log   DISABLE_MODULE_LEVEL

class FunctionFire: public virtual Function
{
  private:
    Parameter parameterInputs[5] = {Parameter("intensity", 0.5),
                                    Parameter("ignition", 0.85),
                                    Parameter("cooling", 0.5),
                                    Parameter("speed", 0.5),
                                    Parameter("acceleration", 0.2)};

    Parameter parameterOutputs [0] = {};                                

    Vector inputs[0] = {};
    LedVector outputVectors[1] = {LedVector()};     
    Vector outputs[1] = {Vector("output", &outputVectors[0])};  
    const int32_t OFFSET = 3;
    const float BLEND_SELF = 1.0;
    const float BLEND_NEIGHBOR_1 = 0.4;
    const float BLEND_NEIGHBOR_2 = 0.2;
    const float BLEND_TOTAL = BLEND_SELF + BLEND_NEIGHBOR_1 * 2.0 + BLEND_NEIGHBOR_2 * 2.0;
    float* heat = nullptr;
    float* temp = nullptr;
    uint16_t arraySize = 0;

    void heatMap(float temperature, float& r, float& g, float& b)
    {
      temperature = constrain(temperature, 0.0, 1.0) * 3.0;
      if(temperature > 2.0)
      {
        r = 1.0;
        g = 0.4;
        b = Utility::mod(temperature, 1.0) * 0.4;
      }
      else if(temperature > 1.0)
      {
        r = 1.0;
        g = 0.2 + Utility::mod(temperature, 1.0) * 0.2;
        b = 0.0;
      }
      else
      {
        r = Utility::mod(temperature, 1.0);
        g = Utility::mod(temperature, 1.0) * 0.2;
        b = 0.0;
      }
    }

    void addHeat(float* arr, float pos, float val)
    {
      int16_t posLower = (int16_t)pos;
      int16_t posUpper = posLower + 1;
      float posOffset = Utility::mod(pos, 1.0);
      if(posLower >= 0 && posLower < arraySize)
      {
        arr[posLower] += val * (1.0 - posOffset);
      }
      if(posUpper >= 0 && posUpper < arraySize)
      {
        arr[posUpper] += val * posOffset;
      }
    }

  public:
    static constexpr const char* MODULE_NAME = "Fire";
    FunctionFire(int32_t id): Function(id, MODULE_NAME) {}
    ~FunctionFire()
    {
      if(heat) delete heat;
      if(temp) delete temp;
    }
    inline Parameter* getParameterInput(uint16_t index) {return (index < (sizeof(parameterInputs) / sizeof(Parameter)))? &parameterInputs[index] : nullptr;}
    inline Parameter* getParameterOutput(uint16_t index) {return (index < (sizeof(parameterOutputs) / sizeof(Parameter)))? &parameterOutputs[index] : nullptr;}
    inline uint32_t getParameterInputCount() {return (sizeof(parameterInputs) / sizeof(Parameter));}
    inline uint32_t getParameterOutputCount() {return (sizeof(parameterOutputs) / sizeof(Parameter));}
    inline Vector* getInput(uint16_t index) {return (index < (sizeof(inputs) / sizeof(Vector)))? &inputs[index] : nullptr;}
    inline Vector* getOutput(uint16_t index) {return (index < (sizeof(outputs) / sizeof(Vector)))? &outputs[index] : nullptr;}
    inline uint32_t getInputCount() {return (sizeof(inputs) / sizeof(Vector));}
    inline uint32_t getOutputCount() {return (sizeof(outputs) / sizeof(Vector));}
    bool init(bool deepCopy = false)
    {
      checkParameterInputs();         // Iterate over all parameter inputs to check if they are valid
      getOutput(0)->allocate(0.0);    // Always allocate an output vector for this module 
      arraySize = pixelcount() + OFFSET;
      heat = new float[arraySize];
      temp = new float[arraySize];
      return initDone();
    }

    bool update(float time)
    {
      if(t == time)                // Only calculate module content if time has updated
      {
        return true;
      }
      if(!Function::update(t))     // Check if all sources are available (modules that are connected have output value ready)
      {
        return false;
      }
      t = time;

      float intensity = getParameterValue(0);
      float ignition = getParameterValue(1);
      float cooling = getParameterValue(2);
      float speed = getParameterValue(3);
      bool acceleration = getParameterValue(4);

      LedVector* output = getOutputValue(0);
      if(LedVector::checkValid(output))
      {
        for(int i = 0; i < pixelcount(); i++)
        {
          heat[i] = max(0.0, heat[i] - Utility::rand() * ((cooling * 10.0) / (float)pixelcount()));
        }
        for(int i = 0; i < arraySize; i++)
        {
          temp[i] = 0.0;
        }
        for(int i = 0; i < pixelcount(); i++)
        {
          addHeat(temp, i * (acceleration * 0.1 + 1.0) + 100.0 * (speed / (float)framerate()), heat[i]);
        }
        heat[0] = temp[0];
        heat[1] = temp[1];
        heat[arraySize - 1] = temp[arraySize - 1];
        heat[arraySize - 2] = temp[arraySize - 2];
        for(int i = 2; i < arraySize - 2; i++)
        {
          heat[i] = (temp[i] * BLEND_SELF +
                     temp[i + 1] * BLEND_NEIGHBOR_1 + 
                     temp[i - 1] * BLEND_NEIGHBOR_1 + 
                     temp[i + 2] * BLEND_NEIGHBOR_2 + 
                     temp[i - 2] * BLEND_NEIGHBOR_2) / BLEND_TOTAL;
        }
        if(Utility::rand() < ignition)
        {
          heat[0] += Utility::rand() * 10.0 * intensity;
        }
        for(int i = 0; i < pixelcount(); i++)
        {
          float r, g, b;
          heatMap(heat[i + OFFSET], r, g, b);
          output->value[LED_R][i] = r;
          output->value[LED_G][i] = g;
          output->value[LED_B][i] = b;
        }
      }
      else error = true;
      return true;
    }
};

#ifdef log
  #undef log
#endif
#endif
