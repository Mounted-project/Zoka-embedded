set(COMPONENT_SRCS "hardware.cpp")
set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_PRIV_REQUIRES MAX17048 arduino-esp32 data_exchange)
register_component()