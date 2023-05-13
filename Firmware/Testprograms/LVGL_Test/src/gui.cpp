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
  return true;
}

void Gui::update(void)
{
  static uint32_t t = 0;
  startWrite();

  setTextColor(TFT_BLACK);
  setCursor(5, 5);
  printf("%d", t);

  t = millis();

  setTextColor(TFT_RED);
  setCursor(5, 5);
  printf("%d", t);

  fillRect(30, 30, 68, 68, TFT_WHITE);
  endWrite();
}