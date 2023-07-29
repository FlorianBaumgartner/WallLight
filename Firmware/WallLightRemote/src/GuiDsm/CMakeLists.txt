SET(SOURCES screens/ui_DSMInitScreen.c
    screens/ui_DSMMainScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_walllight_remote_loadingscreen_png.c)

add_library(ui ${SOURCES})
