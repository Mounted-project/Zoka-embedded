cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(EXTRA_COMPONENT_DIRS components/arduino-esp32 components/MCP23017 components/ECX334 components/lvgl components/data_exchange components/MAX17048 components/BLE)
# add_compile_options("-Wno-format")
project(Zoka)