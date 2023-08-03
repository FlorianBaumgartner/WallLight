/******************************************************************************
* file    ambientSensor.cpp
*******************************************************************************
* brief   High Level Driver for VEML6040 color and ambient light sensor
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-07-31
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

#include <Wire.h>
#include "ambientSensor.hpp"

#include "console.hpp"


AmbientSensor::AmbientSensor(int scl, int sda, int freq) : pin_scl(scl), pin_sda(sda), frequency(freq)
{
}

bool AmbientSensor::begin(uint8_t config)
{
  Wire.begin(pin_sda, pin_scl, frequency);
  if(!sensor.begin())
  {
    return false;
  }
  sensor.setConfiguration(config);

  initialized = true;
  i2cMutex = xSemaphoreCreateMutex();
  xTaskCreate(update, "task_updateAmbientSensor", 4096, this, 1, nullptr);
  return true;
}

void AmbientSensor::update(void *pvParameter)
{
  AmbientSensor* ref = (AmbientSensor*)pvParameter;

  while(ref->initialized)
  {
    TickType_t task_last_tick = xTaskGetTickCount();

    if(xSemaphoreTake(ref->i2cMutex, portMAX_DELAY) == pdTRUE)
    {
      uint16_t red = ref->sensor.getRed();
      uint16_t green = ref->sensor.getGreen();
      uint16_t blue = ref->sensor.getBlue();
      uint16_t white = ref->sensor.getWhite();
      uint16_t ambient = ref->sensor.getAmbientLight();

      ref->color.red = (float)red / 65535.0;
      ref->color.green = (float)green / 65535.0;
      ref->color.blue = (float)blue / 65535.0;
      ref->color.white = (float)white / 65535.0;
      ref->color.ambient = (float)ambient / 2061.0;
      ref->color.cct = ref->getCCT(red, green, blue) / 10144.0;
    }
    xSemaphoreGive(ref->i2cMutex);
    
    vTaskDelayUntil(&task_last_tick, (const TickType_t) 1000 / AMBIENT_SENSOR_UPDATE_RATE);
  }
  vTaskDelete(NULL);
}

