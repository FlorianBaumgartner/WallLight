/******************************************************************************
* file    guiDsa.hpp
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

#ifndef GUI_DSA_HPP
#define GUI_DSA_HPP

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include "guiLvgl.hpp"

class DisplayDsa;

class GuiDsa : public lgfx::LGFX_Device, public GuiLvgl
{
  public:
    static constexpr const uint8_t  DISPLAY_COUNT         = 8;
    static constexpr const uint32_t SCREEN_WIDTH          = 128;
    static constexpr const uint32_t SCREEN_HEIGHT         = 128;
    static constexpr const uint32_t SCREEN_BUFFER_HEIGHT  = 16;
    static constexpr const float    UPDATE_RATE           = 30.0;   // Hz

    GuiDsa(int sclk, int mosi, int dc, int rst, int cs, int cs_0, int cs_1, int cs_2, int bl, int freq = 40000000);
    bool begin(void);
    void selectDisplay(uint8_t n = 0);

    void setId(uint8_t channel, uint32_t id, bool force = false);
    inline uint32_t getId(uint8_t channel) {return id[channel];};
    void setValue(uint8_t channel, float value, bool force = false);
    inline float getValue(uint8_t channel) {return value[channel];};
    void setStep(uint8_t channel, float step, bool force = false);
    inline float getStep(uint8_t channel) {return step[channel];};

  private:
    const int cs_0;
    const int cs_1;
    const int cs_2;

    void flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    void touchRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {};

    lgfx::Panel_GC9107   _panel_instance;
    lgfx::Bus_SPI        _bus_instance;
    lgfx::Light_PWM      _light_instance;
    DisplayDsa* disp;
    SemaphoreHandle_t dispMutex;

    lv_obj_t* labelId[DISPLAY_COUNT];
    lv_obj_t* labelValue[DISPLAY_COUNT];
    lv_obj_t* labelStep[DISPLAY_COUNT];

    volatile uint32_t id[DISPLAY_COUNT] = {1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007};
    volatile float value[DISPLAY_COUNT] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    volatile float step[DISPLAY_COUNT] = {0.1, 0.1, 1.0, 1.0, 0.1, 0.1, 1.0, 1.0};

    static const uint8_t LABEL_TEXT_SIZE = 12;
    char labelTextId[DISPLAY_COUNT][LABEL_TEXT_SIZE];
    char labelTextValue[DISPLAY_COUNT][LABEL_TEXT_SIZE];
    char labelTextStep[DISPLAY_COUNT][LABEL_TEXT_SIZE];
};

class DisplayDsa : public Display
{
  public:
    uint8_t channel;
    lv_color_t* buf[GuiDsa::SCREEN_WIDTH * GuiDsa::SCREEN_BUFFER_HEIGHT];
    DisplayDsa(): Display(GuiDsa::SCREEN_WIDTH, GuiDsa::SCREEN_BUFFER_HEIGHT) {};
};

#endif
