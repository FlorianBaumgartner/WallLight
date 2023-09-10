/******************************************************************************
* file    mic.hpp
*******************************************************************************
* brief   PDM Microphone Driver
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-09-10
*******************************************************************************
* Library is based on: https://github.com/m5stack
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

#ifndef MIC_HPP
#define MIC_HPP

#include <Arduino.h>
#include <driver/i2s.h>

#define MIC_MAGNIFICATION                   64            // Multiplier for input value
#define MIC_OVERSAMPLING                    4             // Oversampling factor (1, 2, 4, 8)
#define MIC_DMA_BUFFER_SIZE                 512           // I2S Buffer size
#define MIC_DMA_BUFFER_COUNT                2             // I2S Buffer count
#define MIC_TASK_PRIORITY                   2             // Task priority

#define COMM_FORMAT_I2S (I2S_COMM_FORMAT_STAND_I2S)
#define COMM_FORMAT_MSB (I2S_COMM_FORMAT_I2S_MSB)


class Mic 
{
  public:
    Mic(int pdmClk, int pdmData);
    bool begin(uint16_t sampleRate = 16000, float updateRate = 20);
    void end(void);
    float getAmplitude(void) {return amplitude;}
    void setCallback(void(*func)(float)) {callback = func;}

  
  private:
    const int pin_pdmClk;
    const int pin_pdmData;

    uint16_t sampleRate;
    float updateRate;
    volatile float amplitude = 0.0f;
    i2s_port_t i2sPort = i2s_port_t::I2S_NUM_0;

    int16_t dmaInputBuffer[MIC_DMA_BUFFER_SIZE];
    int32_t sumBuffer[MIC_DMA_BUFFER_SIZE];

    volatile bool initialized = false;
    volatile SemaphoreHandle_t taskSemaphore = nullptr;
    TaskHandle_t taskHandle = nullptr;
    void (*callback)(float amplitude) = nullptr;

    bool setupI2s(void);
    static void update(void *pvParameter);
};

#endif
