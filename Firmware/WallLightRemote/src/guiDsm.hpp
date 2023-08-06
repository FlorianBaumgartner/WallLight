/******************************************************************************
* file    guiDsm.hpp
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

#ifndef GUI_DSM_HPP
#define GUI_DSM_HPP

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include "guiLvgl.hpp"


class DisplayDsm;

class GuiDsm : public lgfx::LGFX_Device, public GuiLvgl
{
  public:
    static constexpr const uint32_t SCREEN_WIDTH          = 170;
    static constexpr const uint32_t SCREEN_HEIGHT         = 320;
    static constexpr const uint32_t SCREEN_BUFFER_HEIGHT  = 60;
    static constexpr const float    UPDATE_RATE           = 30.0;   // Hz
    static constexpr const float    TOUCH_UPDATE_RATE     = 30.0;   // [Hz]

    GuiDsm(int sclk, int mosi, int dc, int rst, int cs, int bl, int tch_scl, int tch_sda, int tch_irq, int tch_rst, int freq = 40000000);
    bool begin(SemaphoreHandle_t* i2cMutex = nullptr, bool startLvglTask = false);
    void loadMainUi(void);
    
  private:
    const int tch_scl;
    const int tch_sda;
    const int tch_irq;
    const int tch_rst;

    void flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    void touchRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
    void formatDate(char* date);
    void initTouch(void);
    void startTouch(void);
    static void IRAM_ATTR touchInterrupt(void);

    lgfx::Panel_ST7789   _panel_instance;
    lgfx::Bus_SPI        _bus_instance;
    lgfx::Light_PWM      _light_instance;
    lgfx::Touch_GT911    _touch_instance;
    DisplayDsm* disp;
    SemaphoreHandle_t dispMutex;
    SemaphoreHandle_t* i2cMutex = nullptr;

    static GuiDsm* staticRef;
    const char labelVersion[5] = "V" FIRMWARE_VERSION;
    char labelDate[10] = "DD.MM.YY";
};

class DisplayDsm : public Display
{
  public:
    volatile bool isrFlag = false;
    lv_indev_drv_t indev_drv;       // Touch input device driver
    lv_color_t* buf[GuiDsm::SCREEN_WIDTH * GuiDsm::SCREEN_BUFFER_HEIGHT];
    DisplayDsm(): Display(GuiDsm::SCREEN_WIDTH, GuiDsm::SCREEN_BUFFER_HEIGHT) {};
};

#endif
