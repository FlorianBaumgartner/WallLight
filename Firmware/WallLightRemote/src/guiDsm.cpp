/******************************************************************************
* file    guiDsm.cpp
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

#include "guiDsm.hpp"
#include "GuiDsm/ui.h"
#include "console.hpp"


GuiDsm* GuiDsm::staticRef = nullptr;


GuiDsm::GuiDsm(int sclk, int mosi, int dc, int rst, int cs, int bl, int tch_scl, int tch_sda, int tch_irq, int tch_rst, int freq) : tch_scl(tch_scl), tch_sda(tch_sda), tch_irq(tch_irq), tch_rst(tch_rst)
{
  staticRef = this;

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
    cfg.offset_x         =    35;
    cfg.offset_y         =     0;
    cfg.offset_rotation  =     0;
    cfg.dummy_read_pixel =     8;
    cfg.dummy_read_bits  =     1;
    cfg.readable         =  true;
    cfg.invert           =  true;
    cfg.rgb_order        = false;
    cfg.dlen_16bit       = false;
    cfg.bus_shared       = false;

    _panel_instance.config(cfg);
  }
  {
    auto cfg = _touch_instance.config();

    cfg.x_min      = 0;
    cfg.x_max      = SCREEN_WIDTH;
    cfg.y_min      = 0;
    cfg.y_max      = SCREEN_HEIGHT;
    cfg.pin_int    = -1;            // Use custom implementation of interrupt handling
    cfg.pin_rst    = tch_rst;
    cfg.bus_shared = false;
    cfg.offset_rotation = 0;

    cfg.i2c_port   = 1;
    cfg.pin_sda    = tch_sda;
    cfg.pin_scl    = tch_scl;
    cfg.freq       = 400000;
    cfg.i2c_addr   = 0x14;

    _touch_instance.config(cfg);
    _panel_instance.setTouch(&_touch_instance);
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

  formatDate(labelDate);
}

bool GuiDsm::begin(bool startLvglTask)
{
  static DisplayDsm display;
  disp = &display;

  setBrightness(0);
  initTouch();
  if(!init())
  {
    return false;
  }

  // _touch_instance.setTouchNums(1);        // Set number of touch points to 1
  // delay(300);
  // pinMode(tch_irq, INPUT_PULLUP);
  // attachInterrupt(tch_irq, touchInterrupt, RISING);
  // lgfx::touch_point_t point;
  // getTouch(&point);

  lvglInit();                             // Initialize global (static) LVGL instance, is ignored if already initialized
  lv_disp_draw_buf_init(&display.draw_buf, display.buf, NULL, SCREEN_WIDTH * SCREEN_BUFFER_HEIGHT);
  lv_disp_drv_init(&display.disp_drv);
  display.gui = this;
  display.disp_drv.hor_res = SCREEN_WIDTH;
  display.disp_drv.ver_res = SCREEN_HEIGHT;
  display.disp_drv.flush_cb = lvglFlush;
  display.disp_drv.draw_buf = &display.draw_buf;
  display.disp_drv.user_data = &display;
  display.disp = lv_disp_drv_register(&display.disp_drv);
  lv_timer_set_period(display.disp->refr_timer, 1000.0 / UPDATE_RATE);

  lv_indev_drv_init(&display.indev_drv);
  display.indev_drv.type = LV_INDEV_TYPE_POINTER;
  display.indev_drv.read_cb = lvglTouchRead;
  display.indev_drv.user_data = &display;
  lv_indev_drv_register(&display.indev_drv);
  startTouch();

  lv_disp_set_default(display.disp);
  ui_init();

  lv_label_set_text_static(ui_DSMVersionLabel, labelVersion);
  lv_label_set_text_static(ui_DSMDateLabel, labelDate);

  lv_task_handler();                        // Call LVGL task handler once to initialize display

  if(startLvglTask)
  {
    lvglStart();                            // Start LVGL task if not already started
    while(!display.initialized)             // Wait until display is updated
    {
      delay(10);
    }
  }

  setBrightness(255);
  return true;
}

void GuiDsm::loadMainUi(void)
{
  lv_disp_set_default(disp->disp);
  lv_disp_load_scr(ui_DSMMainScreen);
}

void GuiDsm::formatDate(char* date)
{
  char s_month[4];
  int month, day, year;
  sscanf(__DATE__, "%s %d %d", s_month, &day, &year);

  if (strcmp(s_month, "Jan") == 0) month = 1;
  else if (strcmp(s_month, "Feb") == 0) month = 2;
  else if (strcmp(s_month, "Mar") == 0) month = 3;
  else if (strcmp(s_month, "Apr") == 0) month = 4;
  else if (strcmp(s_month, "May") == 0) month = 5;
  else if (strcmp(s_month, "Jun") == 0) month = 6;
  else if (strcmp(s_month, "Jul") == 0) month = 7;
  else if (strcmp(s_month, "Aug") == 0) month = 8;
  else if (strcmp(s_month, "Sep") == 0) month = 9;
  else if (strcmp(s_month, "Oct") == 0) month = 10;
  else if (strcmp(s_month, "Nov") == 0) month = 11;
  else if (strcmp(s_month, "Dec") == 0) month = 12;

  snprintf(date, 10, "%02d.%02d.%02d", day, month, year - 2000);
}

void GuiDsm::initTouch(void)
{
  pinMode(tch_irq, OUTPUT);
  pinMode(tch_rst, OUTPUT);

  digitalWrite(tch_irq, LOW);
  digitalWrite(tch_rst, LOW);

  delay(11);
  digitalWrite(tch_irq, HIGH);
  delayMicroseconds(110);
  pinMode(tch_rst, INPUT);

  delay(6);
  digitalWrite(tch_irq, LOW);
  delay(51);
}

void GuiDsm::startTouch(void)
{
  _touch_instance.setTouchNums(1);        // Set number of touch points to 1
  delay(300);
  pinMode(tch_irq, INPUT_PULLUP);
  attachInterrupt(tch_irq, touchInterrupt, RISING);
}

void IRAM_ATTR GuiDsm::touchInterrupt(void)
{
  staticRef->disp->isrFlag = true;
}

void GuiDsm::flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  DisplayDsm* display = (DisplayDsm*)disp->user_data;
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  waitDMA();
  startWrite();
  setAddrWindow(area->x1, area->y1, w, h);
  writePixels((lgfx::rgb565_t*)&color_p->full, w * h);
  endWrite();
  lv_disp_flush_ready(disp);
  display->initialized = true;
}

void GuiDsm::touchRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
   DisplayDsm* display = (DisplayDsm*)indev_driver->user_data;

    if(display->isrFlag)
    {
      display->isrFlag = false;
      lgfx::touch_point_t point;
      uint8_t count = ((GuiDsm*)display->gui)->getTouch(&point);
      if(count > 0)
      {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = point.x;
        data->point.y = point.y;
      }
      else
      {
        data->state = LV_INDEV_STATE_REL;
      }
    }
    // TODO: Check if touch state should be released if flag is not set
}
