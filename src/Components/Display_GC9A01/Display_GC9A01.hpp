#ifndef Display_GC91A01_H
    #define Display_GC91A01_H

    #include "AiEsp32RotaryEncoder.h"
    #include "Components/Setup.hpp"

    using namespace std;

    enum BATTERYCOLOR
    {
        RED = TFT_RED,
        ORANGE = TFT_ORANGE,
        YELLOW = TFT_YELLOW,
        GREEN = TFT_GREEN
    };

    static PNG _png;
    static TFT_eSPI _display;

    class DisplayGC9A01
    {
    public:
        DisplayGC9A01(uint8_t resolution, uint64_t backgroundColor);

        void init(void);
        void deviceStatus(bool state);
        void setFontSprite(void);
        void initSprite(void);
        void splashScreen(bool inOut, String str1, String str2);
        
        void drawMenu(bool arcRoundedEnd, uint8_t thickness);
        void drawDynamicMenu(bool inOut, bool arcRoundedEnd, uint8_t thickness, Move cursorSt);
        void drawDynamicMenuIcons(bool state);
        void drawMenuTitle(String leftTitle, String centerTitle, String rightTitle);
        void drawTime(String actual_time);
        void drawData(String str, int placement);
        void drawUnit(MODE actualMode, bool menuDynamic);
        void clearUnit(void);
        void clearData(int placement);
        void drawIconMode(MODE actualMode);
        
        void deviceBatteryManagement(uint8_t batteryLevel, bool arcRoundedEnd, uint8_t thickness);
        void phoneBatteryManagement(uint8_t batteryLevel, bool arcRoundedEnd, uint8_t thickness);
       
        void cursorManagement(Move current_state_menu, bool afterDynamicMenu, bool menuDynamic);
        
        static void pngDrawBLELogoStatus(PNGDRAW *pDraw);
        static void pngDrawLogo(PNGDRAW *pDraw);
        static void pngDrawMountainIconMenu(PNGDRAW *pDraw);
        static void pngDrawUrbanIconMenu(PNGDRAW *pDraw);
        static void pngDrawCustomIconMenu(PNGDRAW *pDraw);
        static void pngDrawMountainIconActualMode(PNGDRAW *pDraw);
        static void pngDrawUrbanIconActualMode(PNGDRAW *pDraw);
        static void pngDrawCustomIconActualMode(PNGDRAW *pDraw);

        bool deviceConnected = false;
        MODE _current_mode = MOUNTAIN;
        MODE _old_mode = MOUNTAIN;

    private:
        TFT_eSprite plane_s = TFT_eSprite(&_display);
        TFT_eSprite _sprite = TFT_eSprite(&_display);
        
        TFT_eSprite _splashScreenSprite = TFT_eSprite(&_display);
        TFT_eSprite _splashScreenSubTitleSprite = TFT_eSprite(&_display);
    
        TFT_eSprite _primaryDataSprite = TFT_eSprite(&_display);
        TFT_eSprite _secondaryDataSprite = TFT_eSprite(&_display);
        TFT_eSprite _tertiaryDataSprite = TFT_eSprite(&_display);

        TFT_eSprite _primaryDataUnitSprite = TFT_eSprite(&_display);
        TFT_eSprite _secondaryDataUnitSprite = TFT_eSprite(&_display);
        TFT_eSprite _tertiaryDataUnitSprite = TFT_eSprite(&_display);

        TFT_eSprite _menuTitleLeftSprite = TFT_eSprite(&_display);
        TFT_eSprite _menuTitleCenterSprite = TFT_eSprite(&_display);
        TFT_eSprite _menuTitleRightSprite = TFT_eSprite(&_display);

        TFT_eSprite _currentTimeSprite = TFT_eSprite(&_display);

        TFT_eSprite _actualModeIcon = TFT_eSprite(&_display);

        uint8_t _centerX;
        uint8_t _centerY;
        uint64_t _backgroundColor;
        uint8_t _resolution;
        uint8_t _radius;
        BATTERYCOLOR _arcBatteryColor = GREEN;
        int16_t _rc;

        //MODE _current_mode = MOUNTAIN;
        uint8_t _inner_radius;
        uint8_t _rad;
    };

#endif
