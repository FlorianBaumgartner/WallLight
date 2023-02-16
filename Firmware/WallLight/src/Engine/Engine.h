/******************************************************************************
* file    Engine.h
*******************************************************************************
* brief   Graph Engine that runs in background
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

#ifndef ENGINE_H
#define ENGINE_H

#include <Arduino.h>
#include "Modules.h"


class Engine
{
  public:
    Engine(uint16_t pixelCount, uint16_t framerate);
    bool loadGraph(const char* path);
    void unloadGraph(void);
    bool updateCoefficient(int32_t id, float value);
    LedVector* getPixelData(void) {return &output;};

  private:
    bool setOutput(const Module* module);
    bool update(float t);

    const uint16_t FRAMERATE;
    const uint16_t PIXELCOUNT;
    const uint16_t COLORCOUNT = 6;
    
    LedVector output = LedVector(PIXELCOUNT, COLORCOUNT, true);     // TODO: Remove allocation
    uint32_t outputIndex = 0;
    float t = 0.0;

    char graphName[30] = "";
    char graphRevisionMajor = 0, graphRevisionMinor = 0;
    bool graphLoaded = false;

    Module* modules = nullptr;
    uint32_t moduleCount = 0;
};


#endif