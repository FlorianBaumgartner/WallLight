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

#define ENGINE_VERBOSE true

Engine::Engine()
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

  JsonArray coefficients = doc["coefficients"].as<JsonArray>();
  JsonArray mainModules = doc["modules"].as<JsonArray>();
  uint32_t coefficientCount = coefficients.size();
  uint32_t mainModuleCount = mainModules.size();
  moduleCount = coefficientCount + mainModuleCount;
  modules = new Module*[moduleCount];
  console.log.printf("[ENGINE] Coefficient count: %d, MainModule count: %d, Totally allocated Modules: %d\n", coefficientCount, mainModuleCount, moduleCount);

  for(int i = 0; i < coefficientCount; i++)
  {
    int32_t id = coefficients[i]["id"];
    float value = coefficients[i]["value"];
    modules[i] = new Coefficient(id, value);
    #if ENGINE_VERBOSE
      console.print("[ENGINE] Add "); console[COLOR_YELLOW_BOLD].print("Coefficient"); console.printf(" with id %d, value: %.2f\n", id, value);
    #endif
  }

  for(int i = 0; i < mainModuleCount; i++)
  {
    int index = i + coefficientCount;
    int32_t id = mainModules[i]["id"];
    char classType[MODULE_TYPE_LENGTH];
    strncpy(classType, mainModules[i]["type"].as<const char*>(), MODULE_TYPE_LENGTH);
    char* moduleType = strtok(classType, ".");
    moduleType = strtok(nullptr, ".");

    ConsoleColor color;
    Module* module = nullptr;
    Module::ModuleClass moduleClass;
    if(strncmp(classType, "Generator", MODULE_TYPE_LENGTH) == 0)
    {
      moduleClass = Module::MODULE_GENERATOR;
      color = COLOR_BLUE_BOLD;
      module = allocateGenerator(moduleType, id);
    }
    else if(strncmp(classType, "Modifier", MODULE_TYPE_LENGTH) == 0)
    {
      moduleClass = Module::MODULE_MODIFIER;
      color = COLOR_GREEN_BOLD;
      module = allocateModifier(moduleType, id);
    }
    else if(strncmp(classType, "Function", MODULE_TYPE_LENGTH) == 0)
    {
      moduleClass = Module::MODULE_FUNCTION;
      color = COLOR_MAGENTA_BOLD;
      module = allocateFunction(moduleType, id);
    }
    else
    {
      console.warning.printf("[ENGINE] Module type '%s' is not supported\n", classType);
      continue;
    }
    if(module)
    {
      modules[index] = module;
    }
    else
    {
      console.error.printf("[ENGINE] Could not allocate Module '%s.%s'\n", classType, moduleType);
      continue;
    }
    

    #if ENGINE_VERBOSE
      console.print("[ENGINE] Add "); console[color].print(classType); console.print('.'); console[COLOR_YELLOW_BOLD].print(moduleType); console.printf(" with id %d\n", id);
    #endif
  }




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
