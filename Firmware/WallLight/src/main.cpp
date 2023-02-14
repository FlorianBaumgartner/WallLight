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
#include "console.h"
#include "utils.h"
#include "Adafruit_NeoPXL8.h"

#define LED               10
#define BLINK_INTERVAL    500
#define WATCHDOG_TIMEOUT  10    // [s]

Utils utils;
int8_t pins[8] = {17, -1, -1, -1, -1, -1, -1, -1};
Adafruit_NeoPXL8 leds(70, pins, NEO_GRB);

void setup()
{
  pinMode(LED, OUTPUT);
  console.begin();
  if(!utils.begin(WATCHDOG_TIMEOUT, "DRIVE"))
  {
    console.error.println("[MAIN] Could not initialize utilities");
  }

  if (!leds.begin())
  {
    console.error.println("[MAIN] Could not initialize LEDs");
  }
  leds.setBrightness(50);
  leds.fill(leds.Color(255, 100, 0));
  leds.show();

  console.log.println("OK, Let's go");
}

void loop()
{
  utils.feedWatchdog();
 
  static int t = 0;
  if(millis() - t > 1000)
  {
    t = millis();
    console.log.printf("Time: %d\n", t);
  }
  digitalWrite(LED, (millis() / BLINK_INTERVAL) & 1);
  delay(1);
}
