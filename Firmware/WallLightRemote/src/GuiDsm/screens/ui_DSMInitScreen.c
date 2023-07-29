// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.1
// LVGL version: 8.3.6
// Project name: WallLightRemote_DSM

#include "../ui.h"

void ui_DSMInitScreen_screen_init(void)
{
    ui_DSMInitScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_DSMInitScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_DSMInitImage = lv_img_create(ui_DSMInitScreen);
    lv_img_set_src(ui_DSMInitImage, &ui_img_walllight_remote_loadingscreen_png);
    lv_obj_set_width(ui_DSMInitImage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_DSMInitImage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_DSMInitImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_DSMInitImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_DSMInitImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_DSMVersionLabel = lv_label_create(ui_DSMInitScreen);
    lv_obj_set_width(ui_DSMVersionLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_DSMVersionLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_DSMVersionLabel, 11);
    lv_obj_set_y(ui_DSMVersionLabel, 145);
    lv_obj_set_align(ui_DSMVersionLabel, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_DSMVersionLabel, "Vx.y");
    lv_obj_set_style_text_color(ui_DSMVersionLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_DSMVersionLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DSMDateLabel = lv_label_create(ui_DSMInitScreen);
    lv_obj_set_width(ui_DSMDateLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_DSMDateLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_DSMDateLabel, -11);
    lv_obj_set_y(ui_DSMDateLabel, 145);
    lv_obj_set_align(ui_DSMDateLabel, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_DSMDateLabel, "DD.MM.YY");
    lv_obj_set_style_text_color(ui_DSMDateLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_DSMDateLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

}
