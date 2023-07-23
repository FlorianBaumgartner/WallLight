/******************************************************************************
* file    hmi.hpp
*******************************************************************************
* brief   Humanmachine interface
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-07-23
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

#ifndef HMI_HPP
#define HMI_HPP

#include <Arduino.h>
#include <SPI.h>

class Hmi
{
  public:
    Hmi(int clk, int data, int ld, int ens, int ena, int enb);
    bool begin(void);

    uint32_t readSerial(void);
  
  private:
    const int pin_clk;
    const int pin_data;
    const int pin_ld;
    const int pin_ens;
    const int pin_ena;
    const int pin_enb;

    // uint32_t readSerial(void);

    // SPIClass hspi = SPIClass(HSPI);
};

#endif
