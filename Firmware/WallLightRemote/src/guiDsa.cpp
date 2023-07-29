/******************************************************************************
* file    guiDsa.cpp
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

#include "guiDsa.hpp"
#include "GuiDsa/ui.h"
#include "console.hpp"


GuiDsa::GuiDsa(int sclk, int mosi, int dc, int rst, int cs, int cs_0, int cs_1, int cs_2, int bl, int freq) : cs_0(cs_0), cs_1(cs_1), cs_2(cs_2)
{
  pinMode(cs_0, OUTPUT);
  pinMode(cs_1, OUTPUT);
  pinMode(cs_2, OUTPUT);
  pinMode(rst, OUTPUT);

  digitalWrite(rst, LOW);
  delay(1);
  digitalWrite(rst, HIGH);

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

    cfg.panel_width      =   SCREEN_WIDTH;
    cfg.panel_height     =   SCREEN_HEIGHT;
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
    cfg.invert = false;
    cfg.freq   = 44100;
    cfg.pwm_channel = 7;

    _light_instance.config(cfg);
    _panel_instance.setLight(&_light_instance);
  }
  setPanel(&_panel_instance);
}

bool GuiDsa::begin(void)
{
  static DisplayDsa display[DISPLAY_COUNT];
  disp = display;

  setBrightness(0);
  for(int i = 0; i < DISPLAY_COUNT; i++)
  {
    selectDisplay(i);
    if(!init_without_reset())
    {
      return false;
    }
  }

  GuiLvgl::lvglInit();                             // Initialize global (static) LVGL instance, is ignored if already initialized
  for(int i = 0; i < DISPLAY_COUNT; i++)
  {
    lv_disp_draw_buf_init(&display[i].draw_buf, display[i].buf, NULL, SCREEN_WIDTH * SCREEN_BUFFER_HEIGHT);
    lv_disp_drv_init(&display[i].disp_drv);
    display[i].gui = this;
    display[i].channel = i;
    display[i].disp_drv.hor_res = SCREEN_WIDTH;
    display[i].disp_drv.ver_res = SCREEN_HEIGHT;
    display[i].disp_drv.flush_cb = lvglFlush;
    display[i].disp_drv.draw_buf = &display[i].draw_buf;
    display[i].disp_drv.user_data = &display[i];
    display[i].disp = lv_disp_drv_register(&display[i].disp_drv);
    lv_timer_set_period(display[i].disp->refr_timer, 1000.0 / UPDATE_RATE);

    lv_disp_set_default(display[i].disp);
    ui_init();
    labelId[i] = ui_labelId;         // Save all pointers before they get overwritten by the next iteration
    labelValue[i] = ui_labelValue;
    labelStep[i] = ui_labelStep;

    setId(i, id[i], true);
    setValue(i, value[i], true);
    setStep(i, step[i], true);
  }
  
  GuiLvgl::lvglStart();                   // Start LVGL task if not already started
  while(true)                             // Wait until all displays are updated
  {
    uint8_t total = 0;
    for(int i = 0; i < DISPLAY_COUNT; i++)
    {
      if(display[i].initialized)
      {
        total++;
      }
    }
    if(total == DISPLAY_COUNT)
    {
      break;
    }
    delay(10);
  }
  setBrightness(255);
  return true;
}

void GuiDsa::selectDisplay(uint8_t n)
{
  digitalWrite(cs_0, n & 0x01);
  digitalWrite(cs_1, n & 0x02);
  digitalWrite(cs_2, n & 0x04);
  delayMicroseconds(10);
}

void GuiDsa::setId(uint8_t channel, uint32_t id, bool force)
{
  id = constrain(id, 1000, 1999);
  if(id != this->id[channel] || force)
  {
    this->id[channel] = id;
    snprintf(labelTextId[channel], LABEL_TEXT_SIZE, "ID: %04d", this->id[channel]);
    lv_label_set_text_static(labelId[channel], labelTextId[channel]);
  }
}

void GuiDsa::setValue(uint8_t channel, float value, bool force)
{
  if(value != this->value[channel] || force)
  {
    this->value[channel] = value;
    snprintf(labelTextValue[channel], LABEL_TEXT_SIZE, "%.2f", this->value[channel]);
    lv_label_set_text_static(labelValue[channel], labelTextValue[channel]);
  }
}

void GuiDsa::setStep(uint8_t channel, float step, bool force)
{
  if(step != this->step[channel] || force)
  {
    this->step[channel] = step;
    if(this->step[channel] >= 1.0)
    {
      snprintf(labelTextStep[channel], LABEL_TEXT_SIZE, "Step: %.0f", this->step[channel]);
    }
    else if(this->step[channel] >= 0.1)
    {
      snprintf(labelTextStep[channel], LABEL_TEXT_SIZE, "Step: %.1f", this->step[channel]);
    }
    else
    {
      snprintf(labelTextStep[channel], LABEL_TEXT_SIZE, "Step: %.2f", this->step[channel]);
    }
    lv_label_set_text_static(labelStep[channel], labelTextStep[channel]);
  }
}

void GuiDsa::flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  DisplayDsa* display = (DisplayDsa*)disp->user_data;
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  while(transmitting)
  {
    delayMicroseconds(100);
  }
  transmitting = true;
  selectDisplay(display->channel);
  startWrite();
  setAddrWindow(area->x1, area->y1, w, h);
  writePixels((lgfx::rgb565_t*)&color_p->full, w * h);
  endWrite();
  lv_disp_flush_ready(disp);
  display->initialized = true;
  transmitting = false;
}
