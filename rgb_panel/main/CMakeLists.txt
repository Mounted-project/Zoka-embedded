idf_component_register(SRCS "lvgl_demo_ui.c" "rgb_lcd_example_main.cpp"
                       INCLUDE_DIRS ".")
set(CMAKE_CXX_STANDARD 23)
add_compile_options(-ffunction-sections -fdata-sections -Wl,--gc-sections)