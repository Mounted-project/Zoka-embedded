SET(SOURCES screens/ui_BootScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_m_png.c)

add_library(ui ${SOURCES})
