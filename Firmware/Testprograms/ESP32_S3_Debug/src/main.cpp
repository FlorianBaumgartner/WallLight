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

#define BUTTON 0
#define LED 10
#define BLINK_INTERVAL 200
#define WATCHDOG_TIMEOUT 0 // [s]

Utils utils;
Preferences preferences;

void setup()
{
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  console.begin();
  if (!utils.begin(WATCHDOG_TIMEOUT, "DRIVE"))
  {
    console.error.println("[MAIN] Could not initialize utilities");
  }

  preferences.begin("WallLight", false);
  uint32_t bootCount = preferences.getUInt("bootCount", 0);
  console.log.printf("[MAIN] Boot Count: %d\n", bootCount);
  bootCount++;
  preferences.putUInt("bootCount", bootCount);
  preferences.end();
}

void loop()
{
  utils.feedWatchdog();

  static bool btnOld = false, btnNew = false;
  btnOld = btnNew;
  btnNew = !digitalRead(BUTTON);
  if ((!btnOld && btnNew))
  {
    console.log.println("[MAIN] Button pressed!");
  }

  static int t = 0;
  if (millis() - t > 5000)
  {
    t = millis();
    console.log.printf("[MAIN] Time: %d\n", t);
  }
  digitalWrite(LED, (millis() / BLINK_INTERVAL) & 1);
  delay(10);
}
