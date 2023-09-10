/******************************************************************************
* file    powerMgmt.hpp
*******************************************************************************
* brief   Power management abstraction layer
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-08-06
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

#ifndef POWER_MGMT_HPP
#define POWER_MGMT_HPP

#include <Arduino.h>

#define POWER_MGMT_UPDATE_RATE              10            // [hz]
#define POWER_MGMT_POWERDOWN_VALID_DELAY    1000          // [ms]
#define POWER_MGMT_MEASUREMENT_AVR_NUM      10            // [#]
#define POWER_MGMT_MEASUREMENT_FACTOR       605.5         // [#]

class PowerMgmt 
{
  public:
    PowerMgmt(int pwrSw, int pwrOff, int batMsr);
    bool begin(void);
    bool getPowerButtonState(void) {return pwrButton;}
    float getBatteryVoltage(void) {return batteryVoltage;}
    uint8_t getBatteryPercentage(void) {return batteryPercentage;}
  
  private:
    const int pin_pwrSw;
    const int pin_pwrOff;
    const int pin_batMsr;

    float batteryVoltage = 0.0;
    uint8_t batteryPercentage = 0;
    bool pwrButton = false, pwrButtonOld = false;
    uint32_t startTime = 0;
    volatile bool initialized = false;

    const float batteryVoltagePercentage [101] =
    {
      4.2000, 4.1813, 4.1635, 4.1466, 4.1306, 4.1153, 4.1006, 4.0867, 4.0734, 4.0606, 4.0483, 4.0366, 4.0252, 4.0143, 4.0038, 3.9936, 3.9837, 3.9741, 3.9648, 3.9557, 3.9469,
      3.9382, 3.9298, 3.9214, 3.9133, 3.9053, 3.8974, 3.8896, 3.8819, 3.8743, 3.8668, 3.8594, 3.8520, 3.8448, 3.8376, 3.8304, 3.8234, 3.8163, 3.8094, 3.8025, 3.7957, 3.7889,
      3.7822, 3.7756, 3.7690, 3.7625, 3.7561, 3.7497, 3.7434, 3.7372, 3.7311, 3.7251, 3.7192, 3.7133, 3.7076, 3.7020, 3.6964, 3.6910, 3.6856, 3.6804, 3.6753, 3.6703, 3.6654, 
      3.6606, 3.6559, 3.6513, 3.6468, 3.6424, 3.6381, 3.6339, 3.6298, 3.6257, 3.6218, 3.6179, 3.6140, 3.6102, 3.6064, 3.6027, 3.5990, 3.5953, 3.5915, 3.5877, 3.5839, 3.5800, 
      3.5761, 3.5720, 3.5678, 3.5635, 3.5590, 3.5542, 3.5493, 3.5441, 3.5387, 3.5329, 3.5268, 3.5203, 3.5134, 3.5061, 3.4982, 3.4899, 3.4810
    };

    float measureBatteryVoltage(void);
    uint8_t calculateBatteryPercentage(float v);
    static void update(void *pvParameter);
};

#endif
