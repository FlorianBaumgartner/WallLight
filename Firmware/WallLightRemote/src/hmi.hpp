/******************************************************************************
* file    hmi.hpp
*******************************************************************************
* brief   Human-Machine interface
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
#include "rotaryEncoder.hpp"

#define HMI_UPDATE_RATE                 1000          // [hz]

class Hmi
{
  public:
    enum ButtonDirection{UP = 1, DOWN = 0};

    Hmi(int clk, int data, int ld, int ens, int ena, int enb);
    bool begin(void);
    bool getIdButtonState(uint8_t channel, bool direction)   // direction: 0 = up, 1 = down
    {
      channel %= 8;
      return (direction? idButtonUp : idButtonDown) & (0x01 << channel);
    }
    bool getIdButtonEdge(uint8_t channel, bool direction)    // direction: 0 = up, 1 = down (clear flag after reading)
    {
      bool state = false;
      channel %= 8;
      if(direction)
      {
        state = idButtonUpEdge & (0x01 << channel);
        idButtonUpEdge &= ~(0x01 << channel);
      }
      else
      {
        state = idButtonDownEdge & (0x01 << channel);
        idButtonDownEdge &= ~(0x01 << channel);
      }
      return state;
    }
    bool getButtonSwitchState(uint8_t channel)
    {
      channel %= 8;
      return buttonSwitch & (0x01 << channel);
    }
    bool getRollerEncoderSwitchState(uint8_t channel)
    {
      channel %= 8;
      return rollerEncoderSwitch & (0x01 << channel);
    }
    bool getRollerEncoderSwitchEdge(uint8_t channel)
    {
      bool state = false;
      channel %= 8;
      state = rollerEncoderSwitchEdge & (0x01 << channel);
      rollerEncoderSwitchEdge &= ~(0x01 << channel);
      return state;
    }
    int32_t getRollerEncoderValue(uint8_t channel, bool clear = false)
    {
      channel %= 8;
      uint8_t pos = 0;
      int32_t value = 0;
      switch(channel)
      {
        case 0: pos = 0; break;
        case 1: pos = 1; break;
        case 4: pos = 2; break;
        case 5: pos = 3; break;
        default: return 0;
      }
      value = rollerEncoder[pos].getPosition();
      if(clear)
      {
        rollerEncoder[pos].setPosition(0);
      }
      return value;
    }
    bool getRotaryEncoderSwitchState(void) {return rotaryEncoderSwitch;}
    bool getRotaryEncoderSwitchEdge(void)
    {
      bool state = rotaryEncoderSwitchEdge;
      rotaryEncoderSwitchEdge = false;
      return state;
    }
    int32_t getRotaryEncoderValue(bool clear = false)
    {
      int32_t value = rotaryEncoder.getPosition();
      if(clear)
      {
        rotaryEncoder.setPosition(0);
      }
      return value;
    }

  
  private:
    const int pin_clk;
    const int pin_data;
    const int pin_ld;
    const int pin_ens;
    const int pin_ena;
    const int pin_enb;

    volatile uint32_t readOut = 0x00000000;

    uint8_t idButtonUp = 0x00, idButtonUpOld = 0x00, idButtonUpEdge = 0x00;
    uint8_t idButtonDown = 0x00, idButtonDownOld = 0x00, idButtonDownEdge = 0x00;
    uint8_t rollerEncoderSwitch = 0x00, rollerEncoderSwitchOld = 0x00, rollerEncoderSwitchEdge = 0x00;
    bool rotaryEncoderSwitch = false, rotaryEncoderSwitchOld = false, rotaryEncoderSwitchEdge = false;
    uint8_t buttonSwitch = 0x00;
    RotaryEncoder rollerEncoder[4] = {RotaryEncoder(&readOut, 16, 17, RotaryEncoder::LatchMode::TWO03),
                                      RotaryEncoder(&readOut, 19, 20, RotaryEncoder::LatchMode::TWO03),
                                      RotaryEncoder(&readOut, 24, 25, RotaryEncoder::LatchMode::TWO03),
                                      RotaryEncoder(&readOut, 27, 28, RotaryEncoder::LatchMode::TWO03)};
    RotaryEncoder rotaryEncoder = RotaryEncoder(pin_enb, pin_ena, RotaryEncoder::LatchMode::TWO03);

    const uint32_t ID_BUTTON_UP_POS[8] = {0x00000040, 0x00000010, 0x00000004, 0x00000001, 0x00004000, 0x00001000, 0x00000400, 0x00000100};
    const uint32_t ID_BUTTON_DOWN_POS[8] = {0x00000080, 0x00000020, 0x00000008, 0x00000002, 0x00008000, 0x00002000, 0x00000800, 0x00000200};
    const uint32_t BUTTON_SWITCH_POS[4] = {0x00400000, 0x00800000, 0x040000000, 0x80000000};
    const uint32_t ENCODER_SWITCH_POS[4] = {0x00040000, 0x00200000, 0x04000000, 0x20000000};

    volatile bool initialized = false;
    TaskHandle_t updateTaskHandle = nullptr;

    static void update(void *pvParameter);
    uint32_t readSerial(void);
};

#endif
