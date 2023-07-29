/******************************************************************************
* file    rotaryEncoder.cpp
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
