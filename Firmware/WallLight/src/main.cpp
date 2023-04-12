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
Preferences preferences;

const char* dirName = "graphs/";
uint32_t fileCount = 0;
uint32_t fileIndex = 0;
char** files = nullptr;
bool scanDir(const char* dirName, char*** files, uint32_t* fileCount);

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
  wallLight->setBrightness(utils.getLedBrightness());
  console.log.println("OK, Let's go");

  if(!scanDir(dirName, &files, &fileCount))
  {
    console.error.println("[MAIN] Could not scan directory");
  }

  preferences.begin("WallLight", false);
  uint32_t bootCount = preferences.getUInt("bootCount", 0);
  fileIndex = preferences.getUInt("fileIndex", 0) % fileCount;
  console.log.printf("[MAIN] Boot Count: %d\n", bootCount);
  bootCount++;
  preferences.putUInt("bootCount", bootCount);
  preferences.end();
}

void loop()
{
  utils.feedWatchdog();

  static bool loadDirectly = LOAD_DIRECTLY;
  static bool btnOld = false, btnNew = false;
  btnOld = btnNew; btnNew = !digitalRead(BUTTON);
  if((!btnOld && btnNew) || loadDirectly)
  {
    console.log.println("[MAIN] Button pressed!");
    loadDirectly = false;
    if(files)
    {
      if(files[fileIndex])
      {
        preferences.begin("WallLight", false);
        preferences.putUInt("fileIndex", fileIndex);
        preferences.end();
        wallLight->loadGraph(files[fileIndex]);
      }
      else
      {
        console.error.println("[MAIN] File name is not valid");
      }
    }
    else
    {
      console.error.println("[MAIN] No files found to load");
    }
    fileIndex++;
    fileIndex %= fileCount;
  }
 
  static int t = 0;
  if(millis() - t > 5000)
  {
    t = millis();
    console.log.printf("[MAIN] Time: %d\n", t);
  }
  digitalWrite(LED, (millis() / BLINK_INTERVAL) & 1);
  delay(10);
}

bool scanDir(const char* dirName, char*** files, uint32_t* fileCount)
{
  char** scan = nullptr;
  uint32_t count = 0;
  File dir;
  if(!dir.open(dirName))
  {
    console.error.printf("[MAIN] Could not open directory \"%s\"\n", dirName);
    return false;
  }
  for(int i = 0; i < 2; i++)      // Do twice, first to get file count, secondly to save file names
  {
    dir.rewind();
    while(true)
    {
      File file;
      if(!file.openNext(&dir, O_RDONLY))
      {
        break;          // No more files in dir
      }
      if(file.isDir())
      {
        file.close();   // skip subdirectories
        continue;
      }
      if(i > 0)
      {
        size_t dirnameLength = strlen(dirName);
        const size_t FILE_NAME_LENGTH = 80;       // Max File name legth is 80 characters
        scan[count] = new char[FILE_NAME_LENGTH];
        strncpy(scan[count], dirName, min(dirnameLength, FILE_NAME_LENGTH));
        file.getName(scan[count] + dirnameLength, FILE_NAME_LENGTH);
        console.log.printf("[MAIN] File found: %s\n", scan[count]);
      }
      count++;
      file.close();
    }
    if(i == 0)    // After first round, allocate filename buffer and reset file count as it is counted again
    {
      scan = new char*[count];
      count = 0;
    }
  }
  dir.close();
  console.log.printf("[MAIN] Total file count in directory \"%s\": %d\n", dirName, count);
  *fileCount = count;
  *files = scan;
  return true;
}
