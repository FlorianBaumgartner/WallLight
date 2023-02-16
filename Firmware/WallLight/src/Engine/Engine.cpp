/******************************************************************************
* file    Engine.cpp
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

#include "Engine.h"
#include "utils.h"
#include "console.h"
#include "ArduinoJson.h"

Engine::Engine(uint16_t pixelCount, uint16_t framerate): PIXELCOUNT(pixelCount), FRAMERATE(framerate)
{

}

bool Engine::loadGraph(const char* path)
{
  File file = fatfs.open(path);
  if(!file)
  {
    console.error.println("[ENGINE] Open file failed");
    return false;
  }
  uint32_t fileSize = file.fileSize();
  uint32_t jsonSize = ((fileSize / 1024) + 1) * 1024;
  console.log.printf("[ENGINE] File size if %d Bytes -> Allocate %d Bytes for JSON object\n", fileSize, jsonSize);

  DynamicJsonDocument doc(jsonSize);
  DeserializationError error = deserializeJson(doc, file);
  if(error)
  {
    file.close();
    console.error.printf("[ENGINE] Failed to pasre JSON file, using default configuration: %d\n", error);
    return false;
  }

  strncpy(graphName, doc["name"].as<const char*>(), sizeof(graphName));
  graphRevisionMajor = int(atof(doc["revision"].as<const char*>()));
  graphRevisionMinor = int(atof(doc["revision"].as<const char*>()) * 10.0) % 10;
  console.log.printf("[ENGINE] Loading graph: '%s', Revision: %d.%d\n", graphName, graphRevisionMajor, graphRevisionMinor);

  output.value[0][0] = 1.0;
  output.value[1][1] = 1.0;
  output.value[2][2] = 1.0;
  output.value[0][3] = 1.0;
  output.value[1][3] = 1.0;
  output.value[2][3] = 1.0;

  console.ok.println("[ENGINE] Graph loading was successful.");
  file.close();
  return true;
}
