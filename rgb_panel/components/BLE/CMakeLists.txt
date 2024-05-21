
set(COMPONENT_SRCS "BLE_Hud_Service.cpp")
set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_PRIV_REQUIRES arduino-esp32 data_exchange)
set(REQUIRES bt)
register_component()