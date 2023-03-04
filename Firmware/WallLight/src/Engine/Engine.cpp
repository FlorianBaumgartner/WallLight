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

#include "ArduinoJson.h"
#include "Engine.hpp"
#include "utils.hpp"
#include "console.hpp"

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
    doc.clear();
    console.error.printf("[ENGINE] Failed to pasre JSON file, using default configuration: %d\n", error);
    return false;
  }
  file.close();

  strncpy(graphName, doc["name"].as<const char*>(), sizeof(graphName));
  graphRevisionMajor = int(atof(doc["revision"].as<const char*>()));
  graphRevisionMinor = int(atof(doc["revision"].as<const char*>()) * 10.0) % 10;
  console.log.printf("[ENGINE] Loading graph: '%s', Revision: %d.%d\n", graphName, graphRevisionMajor, graphRevisionMinor);
  if((graphRevisionMajor * 10 + graphRevisionMinor) > (Module::revision.major * 10 + Module::revision.minor))
  {
    console.warning.println("[ENGINE] Graph has has newer revsion than officially supported, try loading anyway");
  }

  bool moduleWarning = false;
  bool moduleError = false;
  JsonArray coefficients = doc["coefficients"].as<JsonArray>();
  JsonArray mainModules = doc["modules"].as<JsonArray>();
  uint32_t coefficientCount = coefficients.size();
  uint32_t mainModuleCount = mainModules.size();
  moduleCount = coefficientCount + mainModuleCount;
  modules = new Module*[moduleCount];
  for(int i = 0; i < moduleCount; i++)
  {
    modules[i] = nullptr;
  }
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
      console.warning.printf("[ENGINE] Module class type '%s' is not supported\n", classType);
      moduleWarning = true;
      continue;
    }
    if(module)
    {
      modules[index] = module;
    }
    else
    {
      console.error.printf("[ENGINE] Could not allocate Module '%s.%s'\n", classType, moduleType);
      moduleWarning = true;
      continue;
    }
    
    #if ENGINE_VERBOSE
      console.print("[ENGINE] Add "); module->printName(); console.printf(" with id %d\n", id);
    #endif
  }

  for(int i = 0; i < mainModuleCount; i++)
  {
    int index = i + coefficientCount;
    for (JsonVariant parameter : mainModules[i]["parameter"].as<JsonArray>())
    {
      uint16_t parameterIndex = parameter["index"].as<unsigned int>();
      int32_t id = parameter["id"].as<signed int>();
      uint32_t outputIndex = parameter["output"].as<unsigned int>();
      bool status = false;
      if(modules[index])
      {
        Module* sourceModule = getModuleFromId(id);
        if(sourceModule)
        {
          status = modules[index]->setParameterInput(parameterIndex, sourceModule, outputIndex);
          #if ENGINE_VERBOSE
            console.printf("[ENGINE] Connect parameter input '%d' of ", parameterIndex); modules[index]->printName();
            console.printf(" [ID: %d] to ", modules[index]->id); sourceModule->printName(); console.printf(" [ID: %d], Output index: %d\n", sourceModule->id, outputIndex);
          #endif
        } 
      }
      if(!status)
      {
        console.error.printf("[ENGINE] Error occured while setting Parameter '%d' of %s\n", parameterIndex, mainModules[i]["type"].as<const char*>());
        moduleError = true;
      }
    }

    if(mainModules[i].containsKey("input"))
    {
      for (JsonVariant input : mainModules[i]["input"].as<JsonArray>())
      {
        uint16_t inputIndex = input["index"].as<unsigned int>();
        int32_t id = input["id"].as<signed int>();
        uint32_t outputIndex = input["output"].as<unsigned int>();
        bool status = false;
        if(modules[index])
        {
          Module* sourceModule = getModuleFromId(id);
          if(sourceModule)
          {
            if(sourceModule->moduleClass == Module::MODULE_FUNCTION)
            {
              status = ((Function*)modules[index])->setInput(inputIndex, sourceModule, outputIndex);
              #if ENGINE_VERBOSE
                console.printf("[ENGINE] Connect input '%d' of ", inputIndex); modules[index]->printName();
                console.printf(" [ID: %d] to ", modules[index]->id); sourceModule->printName(); console.printf(" [ID: %d], Output index: %d\n", sourceModule->id, outputIndex);
              #endif
            }
            else
            {
              console.error.println("[ENGINE] Module contains inputs but is not of class type 'Function'");
              moduleError = true;
            }
          } 
        }
        if(!status)
        {
          console.error.printf("[ENGINE] Error occured while setting Input '%d' of %s\n", inputIndex, mainModules[i]["type"].as<const char*>());
          moduleError = true;
        }
      }
    }
  }

  for(int i = 0; i < moduleCount; i++)
  {
    if(modules[i])
    {
      if(modules[i]->moduleClass == Module::MODULE_FUNCTION)
      {
        Function* function = (Function*)modules[i];
        bool deepCopy = false;
        for(int p = 0; p < function->getInputCount(); p++)
        {
          if(getInputConnectionCount(function, p) > 1)
          {
            deepCopy = true;
          }
        }
        if(deepCopy)
        {
          console.print("[ENGINE] Make deep copy of "); modules[i]->printName(); console.printf(" [ID: %d]\n", modules[i]->id);
        }
        function->init(deepCopy);    // Check if output buffer needs to be allocated
      }
    }
  }

  int32_t outputId = doc["output"]["id"].as<signed int>();
  outputIndex = doc["output"]["output"].as<unsigned int>();
  Module* outputModule = getModuleFromId(outputId);
  if(!setOutput(outputModule, outputId))
  {
    console.error.printf("[ENGINE] Could not connect system output (Module with ID: %d not available)\n", outputId); 
    moduleError = true;
  }

  if(moduleWarning)
  {
    console.warning.println("[ENGINE] Not all modules could be loaded");
  }
  if(moduleError)
  {
    console.error.println("[ENGINE] Graph loading failed -> free all modules.");
    // TODO: Unload all modules
  }
  else
  {
    console.ok.println("[ENGINE] Graph loading was successful.");
    graphLoaded = true;
  }
  doc.clear();
  return !moduleError;
}

bool Engine::setOutput(const Module* module, uint16_t index)
{
  if(module)
  {
    if(module->moduleClass == Module::MODULE_FUNCTION)
    {
      output = ((Function*)module)->getOutput(outputIndex);
      if(output)
      {
        console.log.print("[ENGINE] Connect system output to "); module->printName(); console.printf(" [ID: %d] with output index: %d\n", index, outputIndex);
        return true;
      }
      else
      {
        console.error.printf("[ENGINE] Output of '%s.%s' [ID: %d] is not allocated\n", module->className, module->moduleName, index); 
        return false;
      }
    }
    else
    {
      console.error.printf("[ENGINE] System output is connected to '%s.%s' [ID: %d] which is not of type 'Function'\n", module->className, module->moduleName, index); 
      return false;
    }
  }
  return false;
}

uint16_t Engine::getInputConnectionCount(Function* function, uint16_t index)
{
  if(function)
  {
    const Vector* inputVector = function->getInput(index);
    if(inputVector->module)
    {
      int32_t checkId = inputVector->module->id;
      uint16_t checkOutputIndex = inputVector->sourceIndex;
      uint16_t count = 0;

      for(int i = 0; i < moduleCount; i++)
      {
        const Module* module = modules[i];
        if(module)
        {
          if(module->moduleClass == Module::MODULE_FUNCTION)
          {
            Function* function = ((Function*)module);
            for(int p = 0; p < function->getInputCount(); p++)
            {
              if(function->getInput(p)->module)
              {
                if(function->getInput(p)->module->id == checkId && checkOutputIndex == p)
                {
                  count++;
                }
              }
            }
          }
        }
      }
      return count;
    }
  }
  return 0;
}

Module* Engine::getModuleFromId(int32_t moduleId)
{
  for(int i = 0; i < moduleCount; i++)
  {
    if(modules[i])
    {
      if(modules[i]->id == moduleId)
      {
        return modules[i];
      }
    }
  }
  console.error.printf("[ENGINE] No module found with ID: %d\n", moduleId);
  return nullptr;
}

bool Engine::update(float t)
{
  int temp = 0;
  while(graphLoaded)
  {
    bool allReady = true;
    for(int i = 0; i < moduleCount; i++)
    {
      if(modules[i])
      {
        allReady &= modules[i]->update(t);
        if(modules[i]->error)
        {
          return false;
        }
      }
    }
    if(allReady)
    {
      return true;
    }
    
    temp++;
    if(temp > 100)
    {
      console.warning.println("[ENGINE] Not all modules are ready, abort updating!");
      return false;
    }
  }
  return true;
}

