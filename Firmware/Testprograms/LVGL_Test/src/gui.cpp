/******************************************************************************
* file    gui.cpp
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

#include "gui.hpp"
#include "Gui/ui.h"
#include "console.hpp"


Gui::Gui(int sclk, int mosi, int dc, int rst, int cs, int bl, int freq)
{
  {
    auto cfg = _bus_instance.config();

    cfg.spi_host = SPI2_HOST;           // SPI2_HOST or SPI3_HOST
    cfg.spi_mode = 0;                   // (0 ~ 3)
    cfg.freq_write = freq;              // (40MHz, 80MHz)
    cfg.freq_read  = 16000000;
    cfg.spi_3wire  = true;        
    cfg.use_lock   = true;        
    cfg.dma_channel = SPI_DMA_CH_AUTO;  // (0=DMA / 1=1ch / 2=ch / SPI_DMA_CH_AUTO)
    cfg.pin_sclk = sclk;
    cfg.pin_mosi = mosi;
    cfg.pin_miso = -1;
    cfg.pin_dc   = dc;

    _bus_instance.config(cfg);
    _panel_instance.setBus(&_bus_instance);
  }
  {
    auto cfg = _panel_instance.config();

    cfg.pin_cs           =    cs;
    cfg.pin_rst          =   rst;
    cfg.pin_busy         =    -1;

    cfg.panel_width      =   128;
    cfg.panel_height     =   128;
    cfg.offset_x         =     0;
    cfg.offset_y         =    32;
    cfg.offset_rotation  =     0;
    cfg.dummy_read_pixel =     8;
    cfg.dummy_read_bits  =     1;
    cfg.readable         =  true;
    cfg.invert           = false;
    cfg.rgb_order        = false;
    cfg.dlen_16bit       = false;
    cfg.bus_shared       =  true;

    _panel_instance.config(cfg);
  }
  {
    auto cfg = _light_instance.config();

    cfg.pin_bl = bl;
    cfg.invert = true;
    cfg.freq   = 44100;
    cfg.pwm_channel = 7;

    _light_instance.config(cfg);
    _panel_instance.setLight(&_light_instance);
  }
  setPanel(&_panel_instance);
}

bool Gui::begin(void)
{
  if(!init())
  {
    return false;
  }
  setBrightness(0);
  fillScreen(TFT_BLACK);
  setBrightness(255);

  lvglInit();                             // Initialize global (static) LVGL instance, is ignored if already initialized
  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf[screenWidth * screenBufferHeight];
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenBufferHeight);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.user_data = this;
  disp = lv_disp_drv_register(&disp_drv);
  lv_timer_set_period(disp->refr_timer, 1000.0 / lvglUpdateRate);

  ui_init();
  return true;
}

void Gui::lvglInit(void)
{
  static bool lvglInitialized = false;
  if(!lvglInitialized)
  {
    lvglInitialized = true;
    lv_init();
    xTaskCreate(lvglUpdate, "lvgl", 4096, nullptr, 1, nullptr);
    console.ok.println("[GUI] Initialization successfull.");
  }
}

void Gui::lvglUpdate(void* pvParameter)
{
  while(1)
  {
    TickType_t task_last_tick = xTaskGetTickCount();
    lv_task_handler();
    vTaskDelayUntil(&task_last_tick, (const TickType_t) (1000.0 / lvglUpdateRate));
  }
  vTaskDelete(NULL);
}
