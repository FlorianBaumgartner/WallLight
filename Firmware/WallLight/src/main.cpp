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
#include "console.hpp"
#include "utils.hpp"

#include "Engine/WallLight.hpp"


#define BUTTON            47
#define LED               10
#define BLINK_INTERVAL    200
#define WATCHDOG_TIMEOUT  30    // [s]

#define LOAD_DIRECTLY     true

#define LED_RGB_PIN       17
#define LED_WWA_PIN       -1

Utils utils;
WallLight* wallLight;

// const char* file = "rect_triangle_test.json";
// const char* file = "rect_triangle_color_test.json";
// const char* file = "Demo_SpeedTest.json";
// const char* file = "Demo_MultiPath.json";
const char* file = "rainbow.json";
// const char* file = "Animation_Matrix.json";
// const char* file = "Demo_ModuleTest_Generator.json";

bool loaded = LOAD_DIRECTLY;

void setup()
{
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  console.begin();
  if(!utils.begin(WATCHDOG_TIMEOUT, "DRIVE"))
  {
    console.error.println("[MAIN] Could not initialize utilities");
  }

  wallLight = new WallLight(LED_RGB_PIN, LED_WWA_PIN, utils.getLedCount(), utils.getLedFramerate(), utils.isLedRgbw());
  if(!wallLight->begin())
  {
    console.error.println("[MAIN] Could not initialize WallLight");
  }
  if(loaded)
  {
    wallLight->loadGraph(file);
  }
  wallLight->setBrightness(utils.getLedBrightness());
  console.log.println("OK, Let's go");
}

void loop()
{
  utils.feedWatchdog();

  static bool btnOld = false, btnNew = false;
  btnOld = btnNew; btnNew = !digitalRead(BUTTON);
  if(!btnOld && btnNew)
  {
    console.log.println("[MAIN] Button pressed!");
    if(!loaded)
    {
      wallLight->loadGraph(file);
      loaded = true;
    }
    else
    {
      wallLight->unloadGraph();
      loaded = false;
    }
  }
 
  static int t = 0;
  if(millis() - t > 5000)
  {
    t = millis();
    console.log.printf("Time: %d\n", t);
  }
  digitalWrite(LED, (millis() / BLINK_INTERVAL) & 1);
  delay(10);
}
