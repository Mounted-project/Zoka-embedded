#ifndef _SETUP_HPP
    #define _SETUP_HPP

/*
font converter : https://oleddisplay.squix.ch/#/home
file converter : https://notisrac.github.io/FileToCArray/ (select 'treat as binary' and 'unsigned char' as Data type)
*/
    #include <Arduino.h>
    #include <TFT_eSPI.h>
    #include <PNGdec.h>

    //DEBUG
    #undef DEV_MODE
    #define DEV_APP_IN_PROGRESS
    #define WROVER_BOARD
    #define V2_PROTO

    //HARDWARE PIN
    #define BATTERY_PIN 35                      //named "SP" on wroom32 dev board
    #define ROTARY_ENCODER_BUTTON_PIN 15
    #define ROTARY_ENCODER_A_PIN 32
    #define ROTARY_ENCODER_B_PIN 21
    #define ROTARY_ENCODER_VCC_PIN 0 // 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder 
    #define ROTARY_ENCODER_STEPS 4
    
    #ifdef WROVER_BOARD
    #define SUICIDE_PIN 2
    #endif

    #define DISPLAY_RESOLUTION 240
    #define DISPLAY_BACKGROUND_COLOR TFT_BLACK
    #define DISPLAY_MENU_COLOR TFT_NAVY
    #define DISPLAY_DYNAMIC_MENU_COLOR TFT_NAVY
    #define ARC_ROUNDED_END false

    //LOGO MAX SIZE
    #define MAX_IMAGE_WDITH 60

    //MENU and BATTERY ARC CIRCLE THICKNESS
    #define ARC_ROUNDED false
    #define MENU_THICKNESS 22
    #define BATTERY_THICKNESS 6
    #define BATTERY_THICKNESS 6
    #define BATTERY_PHONE_ARC_OFFSET 17

    #define MENU_TITLE_SPRITE_WIDTH 70
    #define MENU_TITLE_SPRITE_HEIGHT 30

    #define MENU_TITLE_SPRITE_POSX 35
    #define MENU_TITLE_SPRITE_POSY 125

    //MENU TITLE POSITION
    #define MENU_TITLE_LEFT "OFF"       /*do not change position for v1.0*/
    #define MENU_TITLE_CENTER "MENU"    /*do not change position for v1.0*/
    #define MENU_TITLE_RIGHT "CONF"     /*do not change position for v1.0*/

    //TEXT COLOR
    #define SPLASHSCREEN_TEXT_COLOR TFT_ORANGE
    #define SPLASHSCREEN_SUBTITTLE_TEXT_COLOR TFT_GOLD
    #define UNIT_TEXT_COLOR TFT_GREEN
    #define MENU_TEXT_COLOR TFT_WHITE
    #define CURRENT_TIME_TEXT_COLOR TFT_GOLD

    //SPLASH_SCREEN TITLE
    #define SPLASH_SCREEN_SPRITE_WIDTH 180
    #define SPLASH_SCREEN_SPRITE_HEIGHT 40
    #define SPLASH_SCREEN_SPRITE_POSX 90
    #define SPLASH_SCREEN_SPRITE_POSY 20
    //SPLASH_SCREEN SUBTITLE
    #define SPLASH_SCREEN_SUB_SPRITE_WIDTH 180
    #define SPLASH_SCREEN_SUB_SPRITE_HEIGHT 40
    #define SPLASH_SCREEN_SUB_SPRITE_POSX 90
    #define SPLASH_SCREEN_SUB_SPRITE_POSY -10

    #define SPLASH_SCREEN_BLINKING false

    //PRIMARY_DATA
    #define PRIMARY_DATA_SPRITE_WIDTH 70
    #define PRIMARY_DATA_SPRITE_HEIGHT 35
    #define PRIMARY_DATA_SPRITE_POSX 35
    #define PRIMARY_DATA_SPRITE_POSY 60
    //PRIMARY_UNIT
    #define PRIMARY_DATA_UNIT_SPRITE_WIDTH 70
    #define PRIMARY_DATA_UNIT_SPRITE_HEIGHT 15
    #define PRIMARY_DATA_UNIT_SPRITE_POSX 35
    #define PRIMARY_DATA_UNIT_SPRITE_POSY 15

    //SECONDARY_DATA
    #define SECONDARY_DATA_SPRITE_WIDTH 65
    #define SECONDARY_DATA_SPRITE_HEIGHT 20
    #define SECONDARY_DATA_SPRITE_POSX 65
    #define SECONDARY_DATA_SPRITE_POSY -20
    //SECONDARY_UNIT
    #define SECONDARY_DATA_UNIT_SPRITE_WIDTH 65
    #define SECONDARY_DATA_UNIT_SPRITE_HEIGHT 15
    #define SECONDARY_DATA_UNIT_SPRITE_POSX 65
    #define SECONDARY_DATA_UNIT_SPRITE_POSY -44

    //TERTIARY_DATA
    #define TERTIARY_DATA_SPRITE_WIDTH 50
    #define TERTIARY_DATA_SPRITE_HEIGHT 20
    #define TERTIARY_DATA_SPRITE_POSX -15
    #define TERTIARY_DATA_SPRITE_POSY -20
    //TERTIARY_UNIT
    #define TERTIARY_DATA_UNIT_SPRITE_WIDTH 50
    #define TERTIARY_DATA_UNIT_SPRITE_HEIGHT 15
    #define TERTIARY_DATA_UNIT_SPRITE_POSX -15
    #define TERTIARY_DATA_UNIT_SPRITE_POSY -44

    //CURRENT TIME
    #define CURRENT_TIME_SPRITE_WIDTH 30
    #define CURRENT_TIME_SPRITE_HEIGHT 40
    #define CURRENT_TIME_SPRITE_POSX -93
    #define CURRENT_TIME_SPRITE_POSY 20

    //CURSOR COLOR & POSITION
    #define CURSOR_COLOR TFT_GREEN
    #define CURSOR_COLOR_MENU_DYN TFT_YELLOW
    #define TEST_CURSOR TFT_YELLOW
    #define CURSOR_POSITION_EXT 98
    #define CURSOR_POSITION_INT 95
    #define CURSOR_MENU_DYN_POSITION_EXT 72     //78
    #define CURSOR_MENU_DYN_POSITION_INT 70     //76

    //LOGOS POSITION
    #define LOGO_MOUNTED_POSX 50
    #define LOGO_MOUNTED_POSY 97
    //BLE 
    #define BLE_LOGO_POSX 100
    #define BLE_LOGO_POSY 193
    //MOUTAIN ICON MENU
    #define MOUTAIN_ICON_POSX 43    //38
    #define MOUTAIN_ICON_POSY 157
    //URBAN ICON MENU
    #define URBAN_ICON_POSX 27
    #define URBAN_ICON_POSY 112
    //CUSTOM ICON MENU
    #define CUSTOM_ICON_POSX 43     //38
    #define CUSTOM_ICON_POSY 66
    //MOUTAIN ICON ACTUAL MODE
    #define MOUTAIN_ICON_ACTUAL_MODE_POSX 125
    #define MOUTAIN_ICON_ACTUAL_MODE_POSY 193
    //URBAN ICON ACTUAL MODE
    #define URBAN_ICON_ACTUAL_MODE_POSX 125
    #define URBAN_ICON_ACTUAL_MODE_POSY 193
    //CUSTOM ICON ACTUAL MODE
    #define CUSTOM_ICON_ACTUAL_MODE_POSX 125
    #define CUSTOM_ICON_ACTUAL_MODE_POSY 195
    //SPRITE ACTUAL MODE ICON
    #define ICON_ACTUAL_MODE_SPRITE_WIDTH 20
    #define ICON_ACTUAL_MODE_SPRITE_HEIGHT 20
    #define ICON_ACTUAL_MODE_SPRITE_POSX 125
    #define ICON_ACTUAL_MODE_SPRITE_POSY 193

    //SELECT BATTERY (PHONE or SYSTEM)
    #define PHONE_BATTERY false
    #define SYSTEM_BATTERY true

    //FONT
    #include "../include/fonts/numberFont.h"
    #include "../include/fonts/unitFont.h"
    #include "../include/fonts/secondaryDataFont.h"
    #include "../include/fonts/splashscreenTitle.h"
    #include "../include/fonts/splashscreenSubTitle.h"
    #include "../include/fonts/menuTitleFont.h"
    #include "../include/fonts/currentTimeFont.h"

    //LOGO
    #include "../include/logos_hex/logo_mounted.h"
    #include "../include/logos_hex/urban.h"
    #include "../include/logos_hex/mountain.h"
    #include "../include/logos_hex/custom.h"
    #include "../include/logos_hex/ble_connected.h"
    #include "../include/logos_hex/ble_disconnected.h"

    typedef enum 
    {
        MOUNTAIN = 0, 
        URBAN = 1, 
        CUSTOM = 2,
    } MODE; 

    // delete MOVE struct in AiEsp32RotaryEncoder and uncomment here
   /*  typedef enum 
    {
        LEFT = 0, 
        MIDDLE = 1, 
        RIGHT = 2,
    } MOVE; */

    typedef struct {
        std::string _device_connected;
        std::string _get_speed;
        std::string _get_altitude;
        std::string _get_time;
        std::string _get_gps;
        std::string _get_phone_battery;
        std::string _get_local_temperature;
        std::string _get_vertical_speed;
        std::string _get_wind_speed;
    } Data;

#endif