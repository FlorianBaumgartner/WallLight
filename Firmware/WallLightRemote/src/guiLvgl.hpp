/******************************************************************************
* file    guiLvgl.hpp
*******************************************************************************
* brief   Graphical User Interface Lvgl abstraction layer
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

#ifndef GUI_LVGL_HPP
#define GUI_LVGL_HPP

#include <Arduino.h>
#include <lvgl.h>
#include <LovyanGFX.hpp>
#include "console.hpp"

class GuiLvgl
{
  public:
    static constexpr const float LVGL_UPDATE_RATE = 200.0;   // Hz

    virtual void flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    virtual void touchRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

    static void lvglInit(void)
    {
      static bool lvglInitialized = false;
      if(!lvglInitialized)
      {
        lvglInitialized = true;
        lv_init();
      }
    }

    static void lvglStart(void)
    {
      static bool lvglStarted = false;
      if(!lvglStarted)
      {
        lvglStarted = true;
        xTaskCreate(lvglUpdate, "lvgl", 4096, nullptr, 30, nullptr);
        console.ok.println("[GUI] Initialization successfull.");
      }
    }

    static void lvglUpdate(void* pvParameter)
    {
      while(1)
      {
        TickType_t task_last_tick = xTaskGetTickCount();
        lv_task_handler();
        vTaskDelayUntil(&task_last_tick, (const TickType_t) (1000.0 / LVGL_UPDATE_RATE));
      }
      vTaskDelete(NULL);
    }
};

class Display
{
  public:
    const uint32_t width;
    const uint32_t screenBufferHeight; 
    lv_disp_draw_buf_t draw_buf;
    lv_color_t* buf;
    lv_disp_drv_t disp_drv;
    lv_disp_t* disp;
    GuiLvgl* gui;
    volatile bool initialized = false; 
    Display(uint32_t width, uint32_t height): width(width), screenBufferHeight(height) {};
};

static void lvglFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  Display* display = (Display*)disp->user_data;
  display->gui->flush(disp, area, color_p);
}

static void lvglTouchRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  Display* display = (Display*)indev_driver->user_data;
  display->gui->touchRead(indev_driver, data);
}

#endif
