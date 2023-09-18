#include "SPIFFS.h"
#include "BLE_Hud_Service.hpp"
#include "Components/Glb/Glb.hpp"

Glb global = Glb(DISPLAY_RESOLUTION, DISPLAY_BACKGROUND_COLOR, ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
BLE_Hud_Service ble_com_hud = BLE_Hud_Service();

void setup()
{
  Serial.begin(115200);
  global.begin();
  ble_com_hud.initserviceUUID();
  ble_com_hud.createService();
}

void loop()
{
  global.dynamic_menu_management();
  global.updateData(ble_com_hud.hudconnected2(global.get_battery_level(SYSTEM_BATTERY)));
}