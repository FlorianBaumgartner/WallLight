/******************************************************************************
* file    console.cpp
*******************************************************************************
* brief   Wrapper for Serial Debugging (threadsafe, buffered, colorized, etc.)
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2022-08-02
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

#include "console.hpp"

bool Console::begin(void)
{
  return initialize();
}

bool Console::begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms, uint8_t rxfifo_full_thrhd)
{
  return initialize();
}

bool Console::initialize(void)
{
  initialized = true;
  return true;
}

void Console::end(void)
{
  initialized = false;
}

size_t Console::write(const uint8_t *buffer, size_t size)
{
  for(size_t i = 0; i < size; i++)
  {
    putchar(buffer[i]);
  }
  return 0;
}

void Console::printTimestamp(void)
{
  int h = min(millis() / 3600000, 99);
  int m = (millis() / 60000) % 60;
  int s = (millis() / 1000) % 60;
  int ms = millis() % 1000;
  printf("[%02d:%02d:%02d.%03d] ", h, m, s, ms);
}

void Console::printStartupMessage(void)
{
  print(CONSOLE_CLEAR);
  print(CONSOLE_COLOR_BOLD_CYAN CONSOLE_BACKGROUND_DEFAULT);
  println("****************************************************");
  println("*                     WallLight                    *");
  println("*             2023, Florian Baumgartner            *");
  println("****************************************************");
  println(CONSOLE_LOG);
}
