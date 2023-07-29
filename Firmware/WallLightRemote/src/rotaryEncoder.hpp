/******************************************************************************
* file    rotaryEncoder.hpp
*******************************************************************************
* brief   Rotary Encoder Class
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-07-29
*******************************************************************************
* Library is based on: https://github.com/mathertel/RotaryEncoder/tree/master
*******************************************************************************
* See http://www.mathertel.de/License.aspx
* 
* Software License Agreement (BSD License)
* 
* Copyright (c) 2005-2014 by Matthias Hertel,  http://www.mathertel.de/
* 
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 
* • Redistributions of source code must retain the above copyright notice,
*   this list of conditions and the following disclaimer. 
* • Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution. 
* • Neither the name of the copyright owners nor the names of its contributors
*   may be used to endorse or promote products derived from this software
*   without specific prior written permission. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#ifndef ROTARY_ENCODER_HPP
#define ROTARY_ENCODER_HPP

#include <Arduino.h>

class RotaryEncoder
{
  public:
    enum class Direction
    {
      NOROTATION = 0,
      CLOCKWISE = 1,
      COUNTERCLOCKWISE = -1
    };

    enum class LatchMode
    {
      FOUR3 = 1, // 4 steps, Latch at position 3 only (compatible to older versions)
      FOUR0 = 2, // 4 steps, Latch at position 0 (reverse wirings)
      TWO03 = 3  // 2 steps, Latch at position 0 and 3 
    };

    RotaryEncoder(int a, int b, LatchMode mode = LatchMode::FOUR0);
    RotaryEncoder(const volatile uint32_t* reg, int a, int b, LatchMode mode = LatchMode::FOUR0);
    int32_t getPosition() {return _positionExt;};     // Retrieve the current position
    Direction getDirection();                         // Simple retrieve of the direction the knob was rotated last time. 0 = No rotation, 1 = Clockwise, -1 = Counter Clockwise
    void setPosition(long newPosition);               // Adjust the current position
    void tick(void);                                  // Call this function every some milliseconds or by using an interrupt for handling state changes of the rotary encoder.
  
  private:
    const bool _usePhysicalPins = true;               // Use physical pins or register
    volatile const uint32_t* _reg = nullptr;          // Pointer to the register for the encoder
    int enc_a, enc_b;
    
    LatchMode _mode;                                  // Latch mode from initialization

    volatile int8_t _oldState;

    volatile int32_t _position;                       // Internal position (4 times _positionExt)
    volatile int32_t _positionExt;                    // External position
    volatile int32_t _positionExtPrev;                // External position (used only for direction checking)
};

#endif
