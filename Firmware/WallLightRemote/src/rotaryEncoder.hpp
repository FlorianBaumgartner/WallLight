/******************************************************************************
* file    rotaryEncoder.hpp
*******************************************************************************
* brief   Rotary Encoder Class
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-07-29
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
