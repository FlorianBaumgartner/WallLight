/******************************************************************************
* file    mic.cpp
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

#include "mic.hpp"
#include "console.hpp"

Mic::Mic(int pdmClk, int pdmData) : pin_pdmClk(pdmClk), pin_pdmData(pdmData)
{
  taskSemaphore = xSemaphoreCreateBinary();
}

bool Mic::begin(uint16_t sampleRate, float updateRate)
{
  this->sampleRate = sampleRate;
  this->updateRate = updateRate;

  if(!setupI2s())
  {
    return false;
  }
  initialized = true;
  uint16_t stackSize = 2048 + (MIC_DMA_BUFFER_SIZE * sizeof(int16_t));
  xTaskCreate(update, "task_updateMic", stackSize, this, MIC_TASK_PRIORITY, &taskHandle);
  return true;
}

void Mic::end(void)
{
  if(!initialized)
  {
    return;
  }
  initialized = false;
  if(taskHandle)
  {
    if(taskHandle) {xTaskNotifyGive(taskHandle);}
    do {vTaskDelay(1);} while (taskHandle);
  }
  i2s_driver_uninstall(i2sPort);
}

bool Mic::setupI2s(void)
{
  i2s_config_t i2s_config;
  memset(&i2s_config, 0, sizeof(i2s_config_t));
  i2s_config.mode                 = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | (0x1 << 6));  // 0x1<<6 is I2S_MODE_PDM
  i2s_config.sample_rate          = sampleRate * MIC_OVERSAMPLING;
  i2s_config.bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT;
  i2s_config.channel_format       = I2S_CHANNEL_FMT_ONLY_RIGHT;     // (stereo: I2S_CHANNEL_FMT_RIGHT_LEFT)
  i2s_config.communication_format = (i2s_comm_format_t)(COMM_FORMAT_I2S);
  i2s_config.dma_buf_count        = MIC_DMA_BUFFER_COUNT;
  i2s_config.dma_buf_len          = MIC_DMA_BUFFER_SIZE;

  i2s_pin_config_t pin_config;
  memset(&pin_config, ~0u, sizeof(i2s_pin_config_t));   // all pin set to I2S_PIN_NO_CHANGE
  pin_config.mck_io_num     = -1;                       // PDM requires no MCK
  pin_config.bck_io_num     = -1;                       // PDM requires no BCK
  pin_config.ws_io_num      = pin_pdmClk;               // PDM CLK (ca. 2.048 MHz @ 16 kHz, oversampling = 2)
  pin_config.data_in_num    = pin_pdmData;              // PDM DATA

  if((i2s_driver_install(i2sPort, &i2s_config, 0, nullptr)) != ESP_OK)
  {
    console.error.println("[MIC] Could not install I2S driver");
    i2s_driver_uninstall(i2sPort);
    return false;
  }
  if(i2s_set_pin(i2sPort, &pin_config) != ESP_OK)
  {
    console.error.println("[MIC] Could not set I2S pins");
    return false;
  }
  return true;
}

void Mic::update(void *pvParameter)
{
  Mic* ref = (Mic*)pvParameter;

  i2s_start(ref->i2sPort);

  uint32_t dmaLen = 0;
  uint32_t sumSampleIndex = 0;
  uint32_t amplitudeSampleIndex = 0;
  uint32_t amplitudeSampleCount = (uint32_t)(ref->sampleRate / ref->updateRate);
  float amplitudeSum = 0.0f;
  float amplitudeNormalized = 0.0f;
  float offset = 0.0f;
  float offsetFilterFactor = 0.001f;

  while(ref->initialized)
  {
    i2s_read(ref->i2sPort, ref->dmaInputBuffer, MIC_DMA_BUFFER_SIZE, &dmaLen, 100 / portTICK_PERIOD_MS);   // Blocking (100ms timeout)
    dmaLen >>= 1;                                                                     // Mono data is stored in first half of buffer

    for(int i = 0; i < dmaLen; i++)
    {
      ref->sumBuffer[sumSampleIndex + i / MIC_OVERSAMPLING] += ref->dmaInputBuffer[i];
    }
    sumSampleIndex += dmaLen / MIC_OVERSAMPLING;

    if(sumSampleIndex >= MIC_DMA_BUFFER_SIZE)                                         // Check if the sum buffer is full
    {
      float value = 0.0f;
      for(int i = 0; i < MIC_DMA_BUFFER_SIZE; i++)
      {
        value = (float)ref->sumBuffer[i] / (float)INT16_MAX;                          // Convert to float
        offset = offset * (1.0f - offsetFilterFactor) + value * offsetFilterFactor;   // Filter offset
        value -= offset;                                                              // Remove offset              
        amplitudeSum += value * value;                                                // Calculate sum of squares

        amplitudeSampleIndex++;
        if(amplitudeSampleIndex >= amplitudeSampleCount)
        {
          amplitudeSampleIndex = 0;
          amplitudeNormalized = (float)(amplitudeSum * MIC_MAGNIFICATION) / (float)(MIC_DMA_BUFFER_SIZE * MIC_OVERSAMPLING);
          ref->amplitude = sqrtf(amplitudeNormalized);                                // Calculate root of sum of squares (RMS)
          amplitudeSum = 0;                                                           // Reset sum

          if(ref->callback)
          {
            ref->callback(ref->amplitude);
          }
        }
      }
      sumSampleIndex = 0;
      memset(ref->sumBuffer, 0, MIC_DMA_BUFFER_SIZE * sizeof(int32_t));
    }
  }
  i2s_stop(ref->i2sPort);
  ref->taskHandle = nullptr;
  vTaskDelete(NULL);
}
