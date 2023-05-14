/******************************************************************************
* file    gui.hpp
*******************************************************************************
* brief   Graphical User Interface Application Layer
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2023-05-13
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

#ifndef GUI_HPP
#define GUI_HPP

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>

class Gui : public lgfx::LGFX_Device
{
  public:
    Gui(int sclk, int mosi, int dc, int rst, int cs, int bl, int freq = 40000000);
    bool begin(void);

  private:
    static constexpr const uint32_t screenWidth         = 128;
    static constexpr const uint32_t screenHeight        = 128;
    static constexpr const uint32_t screenBufferHeight  = 16;
    static constexpr const float    lvglUpdateRate      = 20.0;   // Hz

    static void lvglInit(void);
    static void lvglUpdate(void* pvParameter);

    lgfx::Panel_GC9107   _panel_instance;
    lgfx::Bus_SPI        _bus_instance;
    lgfx::Light_PWM      _light_instance;
};

static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  Gui* display = (Gui*)disp->user_data;
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  display->startWrite();
  display->setAddrWindow(area->x1, area->y1, w, h);
  display->writePixels((lgfx::rgb565_t*)&color_p->full, w * h);
  display->endWrite();
  lv_disp_flush_ready(disp);
}

#endif