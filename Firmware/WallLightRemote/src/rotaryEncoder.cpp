/******************************************************************************
* file    rotaryEncoder.cpp
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

#include "RotaryEncoder.hpp"

#define LATCH0 0 // input state at position 0
#define LATCH3 3 // input state at position 3

// The array holds the values 1 for the entries where a position was decremented,
// a 1 for the entries where the position was incremented
// and 0 in all the other (no change or not valid) cases.

const int8_t KNOBDIR[] = {
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0};


// positions: [3] 1 0 2 [3] 1 0 2 [3]
// [3] is the positions where my rotary switch detends
// ==> right, count up
// <== left,  count down


RotaryEncoder::RotaryEncoder(int a, int b, LatchMode mode) : enc_a(a), enc_b(b), _mode(mode), _usePhysicalPins(true)
{ 
  int sig1 = digitalRead(enc_a);
  int sig2 = digitalRead(enc_b);
  _oldState = sig1 | (sig2 << 1);

  // start with position 0;
  _position = 0;
  _positionExt = 0;
  _positionExtPrev = 0;
}

RotaryEncoder::RotaryEncoder(const volatile uint32_t* reg, int a, int b, LatchMode mode) : _reg(reg), enc_a(a), enc_b(b), _mode(mode), _usePhysicalPins(false)
{
  int sig1 = (*_reg & (1 << enc_a))? 1 : 0;
  int sig2 = (*_reg & (1 << enc_b))? 1 : 0;
  _oldState = sig1 | (sig2 << 1);

  // start with position 0;
  _position = 0;
  _positionExt = 0;
  _positionExtPrev = 0;
}

RotaryEncoder::Direction RotaryEncoder::getDirection()
{
  RotaryEncoder::Direction ret = Direction::NOROTATION;
  if(_positionExtPrev > _positionExt)
  {
    ret = Direction::COUNTERCLOCKWISE;
    _positionExtPrev = _positionExt;
  }
  else if(_positionExtPrev < _positionExt)
  {
    ret = Direction::CLOCKWISE;
    _positionExtPrev = _positionExt;
  }
  else
  {
    ret = Direction::NOROTATION;
    _positionExtPrev = _positionExt;
  }
  return ret;
}

void RotaryEncoder::setPosition(long newPosition)
{
  switch(_mode)
  {
  case LatchMode::FOUR3:
  case LatchMode::FOUR0:
    // only adjust the external part of the position.
    _position = ((newPosition << 2) | (_position & 0x03L));
    _positionExt = newPosition;
    _positionExtPrev = newPosition;
    break;

  case LatchMode::TWO03:
    // only adjust the external part of the position.
    _position = ((newPosition << 1) | (_position & 0x01L));
    _positionExt = newPosition;
    _positionExtPrev = newPosition;
    break;
  }
}

void RotaryEncoder::tick(void)
{
  int sig1 = 0, sig2 = 0;
  if(_usePhysicalPins)
  {
    sig1 = digitalRead(enc_a);
    sig2 = digitalRead(enc_b);
  }
  else
  {
    sig1 = (*_reg & (1 << enc_a))? 1 : 0;
    sig2 = (*_reg & (1 << enc_b))? 1 : 0;
  }
  int8_t thisState = sig1 | (sig2 << 1);

  if (_oldState != thisState)
  {
    _position += KNOBDIR[thisState | (_oldState << 2)];
    _oldState = thisState;

    switch (_mode)
    {
      case LatchMode::FOUR3:
        if (thisState == LATCH3)
        {
          _positionExt = _position >> 2;          // The hardware has 4 steps with a latch on the input state 3
        }
        break;

      case LatchMode::FOUR0:
        if (thisState == LATCH0)
        {
          _positionExt = _position >> 2;          // The hardware has 4 steps with a latch on the input state 0
        }
        break;

      case LatchMode::TWO03:
        if ((thisState == LATCH0) || (thisState == LATCH3))
        {
          _positionExt = _position >> 1;          // The hardware has 2 steps with a latch on the input state 0 and 3
        }
        break;
    }
  }
}
