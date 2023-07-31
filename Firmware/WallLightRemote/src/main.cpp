/******************************************************************************
* file    main.cpp
*******************************************************************************
* brief   Main Program
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-02-14
*******************************************************************************
* MIT License
*
* Copyright (c) 2022 Crelin - Florian Baumgartner
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

#include <Arduino.h>
#include <Preferences.h>
#include "console.hpp"
#include "utils.hpp"
#include "guiDsa.hpp"
#include "guiDsm.hpp"
#include "hmi.hpp"


#define BLINK_INTERVAL    200
#define WATCHDOG_TIMEOUT  30    // [s]

#define DSA_SCLK      3
#define DSA_MOSI      2
#define DSA_DC        6
#define DSA_CS        5
#define DSA_CS0       7
#define DSA_CS1       8
#define DSA_CS2       9
#define DSA_RST       1
#define DSA_BL        21

#define DSM_SCLK      36
#define DSM_MOSI      35
#define DSM_DC        37
#define DSM_CS        34
#define DSM_RST       33
#define DSM_BL        38

#define TCH_SCL       15
#define TCH_SDA       16
#define TCH_RST       17
#define TCH_INT       18

#define HMI_ENS       39
#define HMI_ENA       40
#define HMI_ENB       41
#define HMI_LD        42
#define HMI_CLK       47
#define HMI_DATA      48

#define PWR_OFF       11
#define PWM_SW        12
#define BAT_MSR       4

#define PDM_CLK       14
#define PDM_DATA      13


static Utils utils;
static Preferences preferences;
static GuiDsa guiDsa(DSA_SCLK, DSA_MOSI, DSA_DC, DSA_RST, DSA_CS, DSA_CS0, DSA_CS1, DSA_CS2, DSA_BL);
static GuiDsm guiDsm(DSM_SCLK, DSM_MOSI, DSM_DC, DSM_RST, DSM_CS, DSM_BL, TCH_SCL, TCH_SDA, TCH_INT, TCH_RST);
static Hmi hmi(HMI_CLK, HMI_DATA, HMI_LD, HMI_ENA, HMI_ENB, HMI_ENS);


void setup()
{
  console.begin();

  if(!utils.begin(WATCHDOG_TIMEOUT, "DRIVE"))
  {
    console.error.println("[MAIN] Could not initialize utilities");
  }
  if(!hmi.begin())
  {
    console.error.println("[MAIN] Could not initialize HMI");
  }
  if(!guiDsm.begin())
  {
    console.error.println("[MAIN] Could not initialize DSM GUI");
  }
  if(!guiDsa.begin())
  {
    console.error.println("[MAIN] Could not initialize DSA GUI");
  }
  
  preferences.begin("WallLightRemote", false);
  uint32_t bootCount = preferences.getUInt("bootCount", 0);
  console.log.printf("[MAIN] Boot Count: %d\n", bootCount);
  bootCount++;
  preferences.putUInt("bootCount", bootCount);
  preferences.end();
  
  guiDsm.loadMainUi();
}

void loop()
{
  utils.feedWatchdog();

  for(int i = 0; i < 8; i++)
  {
    if(hmi.getIdButtonEdge(i, Hmi::UP))
    {
      guiDsa.setId(i, guiDsa.getId(i) + 1);
    }
    if(hmi.getIdButtonEdge(i, Hmi::DOWN))
    {
      guiDsa.setId(i, guiDsa.getId(i) - 1);
    }
  }
  for(int i = 0; i < 4; i++)
  {
    const uint8_t pos[4] = {2, 3, 6, 7};
    guiDsa.setValue(i[pos], hmi.getButtonSwitchState(i[pos])? guiDsa.getStep(i[pos]) : 0.0);
  }
  for(int i = 0; i < 4; i++)
  {
    const uint8_t pos[4] = {0, 1, 4, 5};
    static int32_t encoder[4] = {0, 0, 0, 0};
    static float value[4] = {0.0, 0.0, 0.0, 0.0};
    int32_t encoderPos = hmi.getEncoderValue(i[pos]);
    value[i] += (encoderPos - encoder[i]) * guiDsa.getStep(i[pos]);
    encoder[i] = encoderPos;
    guiDsa.setValue(i[pos], value[i]);
  }
  for(int i = 0; i < 4; i++)
  {
    const uint8_t pos[4] = {0, 1, 4, 5};
    const float stepSize[4] = {0.01, 0.1, 1.0, 10.0};
    static uint8_t stepSizeIdx[4] = {1, 1, 1, 1};
    if(hmi.getEncoderSwitchEdge(i[pos]))
    {
      stepSizeIdx[i]++;
      stepSizeIdx[i] %= sizeof(stepSize) / sizeof(float);
      guiDsa.setStep(i[pos], stepSize[stepSizeIdx[i]]);
    }
  }
 
  static int t = 0;
  if(millis() - t > 5000)
  {
    t = millis();
    console.log.printf("[MAIN] Time: %d\n", t);
  }
  delay(5);
}

