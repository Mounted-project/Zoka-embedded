#ifndef GLB_HPP
#define GLB_HPP

#include "Components/Display_GC9A01/Display_GC9A01.hpp"
#include "AiEsp32RotaryEncoder.h"
#include "Components/Setup.hpp"

using namespace std;

static AiEsp32RotaryEncoder _rotaryEncoder;

class Glb : public DisplayGC9A01
{
public:
    Glb(uint8_t resolution, uint64_t backgroundColor, uint8_t encoderAPin, uint8_t encoderBPin, uint8_t encoderButtonPin, uint8_t encoderVccPin, uint8_t encoderSteps);
    void rotary_loop(Move cursor, bool encoderSwitch, bool menuDynamic);
    uint8_t get_battery_level(bool system, uint16_t phone_battery = 0);
    void dynamic_menu_management(void);
    void begin(void);
    void updateData(Data dt);
    String convertDataToString(std::string data);
    uint8_t convertDataToInt(std::string data);

private:
    Data _old_data;
    MODE _mode = URBAN;

    uint16_t _adc_read = 0;
    uint8_t _soc = 100;
    uint8_t _old_soc = 0;
    uint8_t _phone_battery = 0;
    uint8_t _old_phone_battery = 0;
    uint16_t _div_voltage = 0;
    uint16_t _bat_voltage = 0;
    uint8_t _soc_percentage = 0;

    bool _dynamic_menu_set = false;
    bool _selected_mode = false;
    bool _temp = HIGH;

    int _state = 0;
    Move _cursoSt = MIDDLE;
    Move _old_cursoSt = MIDDLE;
    Move _cursoStDynMenu = MIDDLE;
    Move _old_cursoStDynMenu = MIDDLE;
    bool _encoderSwitch = false;

    uint8_t _encoderAPin = 0;
    uint8_t _encoderBPin = 0;
    uint8_t _encoderButtonPin = 0;
    uint8_t _encoderVccPin = 0;
    uint8_t _encoderSteps = 0;

    static void IRAM_ATTR readEncoderISR();
};
#endif