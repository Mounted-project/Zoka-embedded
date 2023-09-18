#include "Display_GC9A01.hpp"

DisplayGC9A01::DisplayGC9A01(uint8_t resolution, uint64_t backgroundColor)
{
    _backgroundColor = backgroundColor;
    _resolution = resolution;
    _radius = resolution / 2;
    _centerX = resolution / 2;
    _centerY = resolution / 2;
}

void DisplayGC9A01::setFontSprite(void)
{
    _splashScreenSprite.setFreeFont(&Syncopate_Bold_16);
    _splashScreenSubTitleSprite.setFreeFont(&Syncopate_Bold_12);
    _splashScreenSprite.setTextColor(TFT_GOLD, TFT_BLACK, true);
    _splashScreenSubTitleSprite.setTextColor(TFT_LIGHTGREY, TFT_BLACK, true);
    _splashScreenSprite.setTextDatum(MC_DATUM);
    _splashScreenSubTitleSprite.setTextDatum(MC_DATUM);

    _primaryDataSprite.setFreeFont(&Orbitron_Bold_35);
    _primaryDataUnitSprite.setFreeFont(&Roboto_Bold_15);
    _primaryDataUnitSprite.setTextColor(UNIT_TEXT_COLOR, TFT_BLACK, true);
    _primaryDataSprite.setTextDatum(MC_DATUM);
    _primaryDataUnitSprite.setTextDatum(MC_DATUM);

    _secondaryDataSprite.setFreeFont(&Orbitron_Bold_17);
    _secondaryDataUnitSprite.setFreeFont(&Roboto_Bold_15);
    _secondaryDataUnitSprite.setTextColor(UNIT_TEXT_COLOR, TFT_BLACK, true);
    _secondaryDataSprite.setTextDatum(MC_DATUM);
    _secondaryDataUnitSprite.setTextDatum(MC_DATUM);
    
    _tertiaryDataSprite.setFreeFont(&Orbitron_Bold_17);
    _tertiaryDataUnitSprite.setFreeFont(&Roboto_Bold_15);
    _tertiaryDataUnitSprite.setTextColor(UNIT_TEXT_COLOR, TFT_BLACK, true);
    _tertiaryDataSprite.setTextDatum(MC_DATUM);
    _tertiaryDataUnitSprite.setTextDatum(MC_DATUM);

    _menuTitleLeftSprite.setFreeFont(&Orbitron_Bold_13);
    _menuTitleCenterSprite.setFreeFont(&Orbitron_Bold_13);
    _menuTitleRightSprite.setFreeFont(&Orbitron_Bold_13); 
    _menuTitleLeftSprite.setTextColor(MENU_TEXT_COLOR, TFT_BLACK, true);
    _menuTitleCenterSprite.setTextColor(MENU_TEXT_COLOR, TFT_BLACK, true);
    _menuTitleRightSprite.setTextColor(MENU_TEXT_COLOR, TFT_BLACK, true);
    _menuTitleLeftSprite.setTextDatum(MC_DATUM);
    _menuTitleCenterSprite.setTextDatum(MC_DATUM);
    _menuTitleRightSprite.setTextDatum(MC_DATUM);

    _currentTimeSprite.setFreeFont(&Orbitron_Medium_10);
    _currentTimeSprite.setTextColor(CURRENT_TIME_TEXT_COLOR, TFT_BLACK, true);
    _currentTimeSprite.setTextDatum(MC_DATUM);
}

void DisplayGC9A01::initSprite(void)
{
    _splashScreenSprite.createSprite(SPLASH_SCREEN_SPRITE_WIDTH, SPLASH_SCREEN_SPRITE_HEIGHT);
    _splashScreenSubTitleSprite.createSprite(SPLASH_SCREEN_SUB_SPRITE_WIDTH, SPLASH_SCREEN_SUB_SPRITE_HEIGHT);

    _primaryDataSprite.createSprite(PRIMARY_DATA_SPRITE_WIDTH,PRIMARY_DATA_SPRITE_HEIGHT);
    _primaryDataUnitSprite.createSprite(PRIMARY_DATA_UNIT_SPRITE_WIDTH, PRIMARY_DATA_UNIT_SPRITE_HEIGHT);

    _secondaryDataSprite.createSprite(SECONDARY_DATA_SPRITE_WIDTH,SECONDARY_DATA_SPRITE_HEIGHT);
    _secondaryDataUnitSprite.createSprite(SECONDARY_DATA_UNIT_SPRITE_WIDTH, SECONDARY_DATA_UNIT_SPRITE_HEIGHT);

    _tertiaryDataSprite.createSprite(TERTIARY_DATA_SPRITE_WIDTH,TERTIARY_DATA_SPRITE_HEIGHT);
    _tertiaryDataUnitSprite.createSprite(TERTIARY_DATA_UNIT_SPRITE_WIDTH, TERTIARY_DATA_UNIT_SPRITE_HEIGHT);

    _menuTitleLeftSprite.createSprite(MENU_TITLE_SPRITE_WIDTH, MENU_TITLE_SPRITE_HEIGHT);
    _menuTitleCenterSprite.createSprite(MENU_TITLE_SPRITE_WIDTH, MENU_TITLE_SPRITE_HEIGHT);
    _menuTitleRightSprite.createSprite(MENU_TITLE_SPRITE_WIDTH, MENU_TITLE_SPRITE_HEIGHT);

    _currentTimeSprite.createSprite(CURRENT_TIME_SPRITE_WIDTH, CURRENT_TIME_SPRITE_HEIGHT);

   // _actualModeIcon.createSprite(ICON_ACTUAL_MODE_SPRITE_WIDTH, ICON_ACTUAL_MODE_SPRITE_HEIGHT);

    setFontSprite();

    _splashScreenSprite.setPivot(SPLASH_SCREEN_SPRITE_POSX, SPLASH_SCREEN_SPRITE_POSY);
    _splashScreenSubTitleSprite.setPivot(SPLASH_SCREEN_SUB_SPRITE_POSX, SPLASH_SCREEN_SUB_SPRITE_POSY);

    _primaryDataSprite.setPivot(PRIMARY_DATA_SPRITE_POSX, PRIMARY_DATA_SPRITE_POSY);
    _primaryDataUnitSprite.setPivot(PRIMARY_DATA_UNIT_SPRITE_POSX, PRIMARY_DATA_UNIT_SPRITE_POSY);

    _secondaryDataSprite.setPivot(SECONDARY_DATA_SPRITE_POSX, SECONDARY_DATA_SPRITE_POSY);
    _secondaryDataUnitSprite.setPivot(SECONDARY_DATA_UNIT_SPRITE_POSX, SECONDARY_DATA_UNIT_SPRITE_POSY);

    _tertiaryDataSprite.setPivot(TERTIARY_DATA_SPRITE_POSX, TERTIARY_DATA_SPRITE_POSY);
    _tertiaryDataUnitSprite.setPivot(TERTIARY_DATA_UNIT_SPRITE_POSX, TERTIARY_DATA_UNIT_SPRITE_POSY);

    _menuTitleLeftSprite.setPivot(MENU_TITLE_SPRITE_POSX, MENU_TITLE_SPRITE_POSY);
    _menuTitleCenterSprite.setPivot(MENU_TITLE_SPRITE_POSX, MENU_TITLE_SPRITE_POSY);
    _menuTitleRightSprite.setPivot(MENU_TITLE_SPRITE_POSX, MENU_TITLE_SPRITE_POSY);

    _currentTimeSprite.setPivot(CURRENT_TIME_SPRITE_POSX, CURRENT_TIME_SPRITE_POSY);

    _actualModeIcon.setPivot(ICON_ACTUAL_MODE_SPRITE_POSX, ICON_ACTUAL_MODE_SPRITE_POSY);
}

void DisplayGC9A01::init(void)
{
    _display.init();
    _display.setRotation(2);
    _display.setPivot(_radius, _radius);
    _display.setTextColor(TFT_WHITE, TFT_BLACK, true);
    initSprite();
    #ifndef DEV_MODE
    splashScreen(SPLASH_SCREEN_BLINKING, "AUGMOUNTED", "LOADING ...");
    #else
    _display.fillScreen(_backgroundColor);
    #endif
    drawUnit(_current_mode, false);
    drawIconMode(_current_mode);
    deviceStatus(deviceConnected);
    cursorManagement(MIDDLE, false, false);
}

void DisplayGC9A01::splashScreen(bool inOut, String str1, String str2)
{
    _display.fillScreen(_backgroundColor);

    _rc = _png.openFLASH((uint8_t*)logo_mounted, sizeof(logo_mounted), pngDrawLogo);
    if (_rc == PNG_SUCCESS) {
        _display.startWrite();
        _rc = _png.decode(NULL, 0);
        _display.endWrite();
        _png.close();
    }

    _splashScreenSprite.drawString(str1, _splashScreenSprite.width()/2, _splashScreenSprite.height()/2);
    _splashScreenSprite.pushRotated(270, TFT_BLACK);

    _splashScreenSubTitleSprite.drawString(str2, _splashScreenSubTitleSprite.width()/2, _splashScreenSubTitleSprite.height()/2);
    _splashScreenSubTitleSprite.pushRotated(270, TFT_BLACK);

    for (int i = 0; i < 360; i++)
    {   
        _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 1+i, TFT_GOLD, _backgroundColor, false);
        if(i<20)
            delay(5);
        else
            delay(1);
    }

    if(inOut) {
        for(int i = 0; i<4; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 360, TFT_BLACK, _backgroundColor, false);
            delay(100);
            _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 360, TFT_LIGHTGREY, _backgroundColor, false);
            delay(100); 
        }
    }
    else {
        _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 360, TFT_LIGHTGREY, _backgroundColor, false);
        delay(1000);
    }
    _display.fillScreen(_backgroundColor);
}   

void DisplayGC9A01::deviceStatus(bool state)
{
    if(state) {
        _rc = _png.openFLASH((uint8_t*)ble_connected, sizeof(ble_connected), pngDrawBLELogoStatus);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }
    } else {
        _rc = _png.openFLASH((uint8_t*)ble_disconnected, sizeof(ble_disconnected), pngDrawBLELogoStatus);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }
    }
}

void DisplayGC9A01::drawMenu(bool arcRoundedEnd, uint8_t thickness)
{
    _inner_radius = _radius - (thickness);
    #ifdef V2_PROTO
    _display.drawSmoothArc(_centerX, _centerY, _radius-18, _radius-20, 1, 160, _arcBatteryColor, _backgroundColor, arcRoundedEnd);
    _display.fillRect(117,220, 3, 18, _arcBatteryColor);
    _display.fillRect(83, 6, 3, 22, _arcBatteryColor);
    #else   
    _display.drawSmoothArc(_centerX, _centerY, _radius, _inner_radius, 0, 160, DISPLAY_MENU_COLOR, _backgroundColor, arcRoundedEnd);
    #endif
}

void DisplayGC9A01::drawDynamicMenu(bool inOut, bool arcRoundedEnd, uint8_t thickness, Move cursorSt)
{
    _inner_radius = ((_radius) - (thickness / 2)) - 2;
    
    #ifdef V2_PROTO
    if (inOut) {
        // fade out
        for (int i = 0; i <= 30; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, 70-i, 110+i, TEST_CURSOR, _backgroundColor, arcRoundedEnd);
            if(i>1 && i<25){
                _rad = _inner_radius - i;
                _display.drawSmoothArc(_centerX, _centerY, _rad, _rad - 1, 40, 140, CURSOR_COLOR_MENU_DYN, _backgroundColor, arcRoundedEnd);
                _display.drawSmoothArc(_centerX, _centerY, _rad, _rad - 2, 40, 140, CURSOR_COLOR_MENU_DYN, _backgroundColor, arcRoundedEnd);
                _display.drawSmoothArc(_centerX, _centerY, _rad, _rad + 1, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
                _display.drawSmoothArc(_centerX, _centerY, _rad, _rad + 2, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
             }
        }
        _display.drawSmoothArc(_centerX, _centerY, _rad, _rad - 1, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
        _display.drawSmoothArc(_centerX, _centerY, _rad, _rad - 2, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
        drawDynamicMenuIcons(true);
        cursorManagement(cursorSt, false, true);
    }
    else
    {
        cursorManagement(cursorSt, true, false);
        _rad = 73;
        // fade in
        for (int i = 0; i <= 30; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, 40+i, 140-i, _backgroundColor, _backgroundColor, arcRoundedEnd);
            if(i>1 && i<25){
                _rad += 1;
                _display.drawSmoothArc(_centerX, _centerY, _rad+1, _rad, 40, 140, TEST_CURSOR, _backgroundColor, arcRoundedEnd);
                _display.drawSmoothArc(_centerX, _centerY, _rad+2, _rad, 40, 140, TEST_CURSOR, _backgroundColor, arcRoundedEnd);
                _display.drawSmoothArc(_centerX, _centerY, _rad-1, _rad, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
                _display.drawSmoothArc(_centerX, _centerY, _rad-2, _rad, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
            }
        }
        //_display.drawSmoothArc(_centerX, _centerY, _rad+1, _rad, 40, 140, _arcBatteryColor, _backgroundColor, arcRoundedEnd);
        _display.drawSmoothArc(_centerX, _centerY, _rad+1, _rad, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
        _display.drawSmoothArc(_centerX, _centerY, _rad+2, _rad, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
    }
    #else
    if (inOut) {
        //  fade in
        for (int i = 0; i <= 30; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, 70-i, 110+i, CURSOR_COLOR, _backgroundColor, arcRoundedEnd);
            if(i<21)
                _display.drawSmoothArc(_centerX, _centerY, _inner_radius, _inner_radius - i, 40, 140, DISPLAY_DYNAMIC_MENU_COLOR, _backgroundColor, arcRoundedEnd);
        }
        drawDynamicMenuIcons(true);
        cursorManagement(cursorSt, false, true);
    }
    else
    {
        // fade out
        for (int i = 0; i <= 30; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, 40+i, 140-i, CURSOR_COLOR, _backgroundColor, arcRoundedEnd);
            if(i<21)
                _display.drawSmoothArc(_centerX, _centerY, (_inner_radius - 20) + i, _inner_radius - 20, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
        }
        cursorManagement(cursorSt, true, false);
    }
    #endif
}

void DisplayGC9A01::drawDynamicMenuIcons(bool state)
{
    if(state) {
        _rc = _png.openFLASH((uint8_t*)urban, sizeof(urban), pngDrawUrbanIconMenu);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }

        _rc = _png.openFLASH((uint8_t*)mountain, sizeof(mountain), pngDrawMountainIconMenu);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }

        _rc = _png.openFLASH((uint8_t*)custom, sizeof(custom), pngDrawCustomIconMenu);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }
    } 
}

void DisplayGC9A01::drawIconMode(MODE actualMode)
{
    _actualModeIcon.fillSprite(TFT_BLACK);
    _actualModeIcon.pushSprite(ICON_ACTUAL_MODE_SPRITE_POSX,ICON_ACTUAL_MODE_SPRITE_POSY);
    
    if(actualMode == MOUNTAIN) {
        _rc = _png.openFLASH((uint8_t*)mountain, sizeof(mountain), pngDrawMountainIconActualMode);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }
    }else if(actualMode == URBAN) {
        _rc = _png.openFLASH((uint8_t*)urban, sizeof(urban), pngDrawUrbanIconActualMode);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }
    }else if(actualMode == CUSTOM) { 
        _rc = _png.openFLASH((uint8_t*)custom, sizeof(custom), pngDrawCustomIconActualMode);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }
    }
}

void DisplayGC9A01::drawMenuTitle(String leftTitle, String centerTitle, String rightTitle)
{
    _menuTitleLeftSprite.drawString(leftTitle,_menuTitleLeftSprite.width()/2, _menuTitleLeftSprite.height()/2);
    _menuTitleLeftSprite.pushRotated(230, TFT_BLACK);

    _menuTitleCenterSprite.drawString(centerTitle,_menuTitleCenterSprite.width()/2, _menuTitleCenterSprite.height()/2);
    _menuTitleCenterSprite.pushRotated(270, TFT_BLACK);

    _menuTitleRightSprite.drawString(rightTitle,_menuTitleLeftSprite.width()/2, _menuTitleLeftSprite.height()/2);
    _menuTitleRightSprite.pushRotated(310, TFT_BLACK);
}

void DisplayGC9A01::cursorManagement(Move current_state_menu, bool afterDynamicMenu, bool menuDynamic)
{
    // position[0] : LEFT
    // position[1] : MIDDLE
    // position[2] : RIGHT
    const uint8_t position[] = {30, 70, 110};
    const uint8_t position_dy[] = {40, 74, 108};
    static Move old_value = MIDDLE;
    static Move old_value_dy = MIDDLE;
    const int size = 40;
    const int size_dyn = 34;

    if(!menuDynamic) {
        if (afterDynamicMenu)
        {
            _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[0], position[2]+ size, _backgroundColor, _backgroundColor, false);
            _display.drawSmoothArc(_centerX, _centerY, CURSOR_MENU_DYN_POSITION_EXT, CURSOR_MENU_DYN_POSITION_INT, position_dy[0], position_dy[2]+size_dyn, _backgroundColor, _backgroundColor, false);
            _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[1], position[1] + size, CURSOR_COLOR, _backgroundColor, false);
            drawUnit(_current_mode, false);
            drawIconMode(_current_mode);
            return;
        }

        if (current_state_menu == MIDDLE)
        {
            for (int i = 0; i < size; i++)
            {
                if (old_value == LEFT)
                {
                    _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[old_value], position[old_value] + i, TFT_BLACK, _backgroundColor, false);
                    _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[old_value] + i, position[old_value] + size + i, CURSOR_COLOR, _backgroundColor, false);
                }
                else if(old_value == RIGHT)
                {
                    _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[old_value] + size - i, position[old_value] + size, TFT_BLACK, _backgroundColor, false);
                    _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[old_value] - i, position[old_value] + size - i, CURSOR_COLOR, _backgroundColor, false);
                }
                else 
                {
                   _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[old_value], position[old_value] + size, CURSOR_COLOR, _backgroundColor, false);
                }
            }
        }
        else if (current_state_menu == LEFT)
        {
            for (int i = 0; i < size; i++)
            {
                _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[old_value] + size - i, position[old_value] + size, TFT_BLACK, _backgroundColor, false);
                _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[old_value] - i, position[old_value] + size - i, CURSOR_COLOR, _backgroundColor, false);
            }
        }
        else if (current_state_menu == RIGHT)
        {
            for (int i = 0; i < size; i++)
            {
                _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[old_value], position[old_value] + i, TFT_BLACK, _backgroundColor, false);
                _display.drawSmoothArc(_centerX, _centerY, CURSOR_POSITION_EXT, CURSOR_POSITION_INT, position[old_value] + i, position[old_value] + size + i, CURSOR_COLOR, _backgroundColor, false);
            }
        }
        old_value = current_state_menu;
    }
    else
    {
        drawUnit(_current_mode, true);
        if (current_state_menu == MIDDLE)
        {
            for (int i = 0; i < size_dyn; i++)
            {
                if (old_value_dy == LEFT)
                {
                    _display.drawSmoothArc(_centerX, _centerY, CURSOR_MENU_DYN_POSITION_EXT, CURSOR_MENU_DYN_POSITION_INT, position_dy[old_value_dy], position_dy[old_value_dy] + i, TFT_BLACK, _backgroundColor, false);
                    _display.drawSmoothArc(_centerX, _centerY, CURSOR_MENU_DYN_POSITION_EXT, CURSOR_MENU_DYN_POSITION_INT, position_dy[old_value_dy] + i, position_dy[old_value_dy] + size_dyn + i, CURSOR_COLOR_MENU_DYN, _backgroundColor, false);
                }
                else if(old_value_dy == RIGHT)
                {
                    _display.drawSmoothArc(_centerX, _centerY, CURSOR_MENU_DYN_POSITION_EXT, CURSOR_MENU_DYN_POSITION_INT, position_dy[old_value_dy] + size_dyn - i, position_dy[old_value_dy] + size_dyn, TFT_BLACK, _backgroundColor, false);
                    _display.drawSmoothArc(_centerX, _centerY, CURSOR_MENU_DYN_POSITION_EXT, CURSOR_MENU_DYN_POSITION_INT, position_dy[old_value_dy] - i, position_dy[old_value_dy] + size_dyn - i, CURSOR_COLOR_MENU_DYN, _backgroundColor, false);
                }
                else 
                {
                   _display.drawSmoothArc(_centerX, _centerY, CURSOR_MENU_DYN_POSITION_EXT, CURSOR_MENU_DYN_POSITION_INT, position[old_value_dy], position[old_value_dy] + size_dyn, CURSOR_COLOR_MENU_DYN, _backgroundColor, false);
                }
            }
        }
        else if (current_state_menu == LEFT)
        {
            for (int i = 0; i < size_dyn; i++)
            {
                _display.drawSmoothArc(_centerX, _centerY, CURSOR_MENU_DYN_POSITION_EXT, CURSOR_MENU_DYN_POSITION_INT, position_dy[old_value_dy] + size_dyn - i, position_dy[old_value_dy] + size_dyn, TFT_BLACK, _backgroundColor, false);
                _display.drawSmoothArc(_centerX, _centerY, CURSOR_MENU_DYN_POSITION_EXT, CURSOR_MENU_DYN_POSITION_INT, position_dy[old_value_dy] - i, position_dy[old_value_dy] + size_dyn - i, CURSOR_COLOR_MENU_DYN, _backgroundColor, false);
            }
        }
        else if (current_state_menu == RIGHT)
        {
            for (int i = 0; i < size_dyn; i++)
            {
                _display.drawSmoothArc(_centerX, _centerY, CURSOR_MENU_DYN_POSITION_EXT, CURSOR_MENU_DYN_POSITION_INT, position_dy[old_value_dy], position_dy[old_value_dy] + i, TFT_BLACK, _backgroundColor, false);
                _display.drawSmoothArc(_centerX, _centerY, CURSOR_MENU_DYN_POSITION_EXT, CURSOR_MENU_DYN_POSITION_INT, position_dy[old_value_dy] + i, position_dy[old_value_dy] + size_dyn + i, CURSOR_COLOR_MENU_DYN, _backgroundColor, false);
            }
        }
        old_value_dy = current_state_menu;
    }
}

void DisplayGC9A01::drawData(String str, int placement)
{
    clearData(placement);
    switch (placement)
    {
    case 0:
        _primaryDataSprite.drawString(str, _primaryDataSprite.width()/2, _primaryDataSprite.height()/2);
        _primaryDataSprite.pushRotated(270,TFT_BLACK);
        break;

    case 1:
        _secondaryDataSprite.drawString(str, _secondaryDataSprite.width()/2, _secondaryDataSprite.height()/2);
        _secondaryDataSprite.pushRotated(270, TFT_BLACK);
        break;

    case 2:
        _tertiaryDataSprite.drawString(str, _tertiaryDataSprite.width()/2, _tertiaryDataSprite.height()/2);
        _tertiaryDataSprite.pushRotated(270, TFT_BLACK);
        break;
    }
}

void DisplayGC9A01::drawUnit(MODE actualMode, bool menuDynamic)
{   
    clearUnit();

    if(menuDynamic)
    { 
        drawData("--", 0);
        drawData("--", 1);
        drawData("--", 2);

        _primaryDataUnitSprite.drawString("", _primaryDataUnitSprite.width()/2, _primaryDataUnitSprite.height()/2);
        _primaryDataUnitSprite.pushRotated(270, TFT_BLACK);

        _secondaryDataUnitSprite.drawString("", _secondaryDataUnitSprite.width()/2, _secondaryDataUnitSprite.height()/2);
        _secondaryDataUnitSprite.pushRotated(270, TFT_BLACK);

        _tertiaryDataUnitSprite.drawString("", _tertiaryDataUnitSprite.width()/2, _tertiaryDataUnitSprite.height()/2);
        _tertiaryDataUnitSprite.pushRotated(270, TFT_BLACK);
    }
    else 
    {    
        if(actualMode == MOUNTAIN){
            _primaryDataUnitSprite.drawString("KM/H", _primaryDataUnitSprite.width()/2, _primaryDataUnitSprite.height()/2);
            _primaryDataUnitSprite.pushRotated(270, TFT_BLACK);

            _secondaryDataUnitSprite.drawString("M", _secondaryDataUnitSprite.width()/2, _secondaryDataUnitSprite.height()/2);
            _secondaryDataUnitSprite.pushRotated(270, TFT_BLACK);

            _tertiaryDataUnitSprite.drawString("C", _tertiaryDataUnitSprite.width()/2, _tertiaryDataUnitSprite.height()/2);
            _tertiaryDataUnitSprite.pushRotated(270, TFT_BLACK);
        }
        else if(actualMode == URBAN) {
            _primaryDataUnitSprite.drawString("GPS", _primaryDataUnitSprite.width()/2, _primaryDataUnitSprite.height()/2);
            _primaryDataUnitSprite.pushRotated(270, TFT_BLACK);

            _secondaryDataUnitSprite.drawString("KM/H", _secondaryDataUnitSprite.width()/2, _secondaryDataUnitSprite.height()/2);
            _secondaryDataUnitSprite.pushRotated(270, TFT_BLACK);

            _tertiaryDataUnitSprite.drawString("C", _tertiaryDataUnitSprite.width()/2, _tertiaryDataUnitSprite.height()/2);
            _tertiaryDataUnitSprite.pushRotated(270, TFT_BLACK);
        } else {
            _primaryDataUnitSprite.drawString("VS (m/s)", _primaryDataUnitSprite.width()/2, _primaryDataUnitSprite.height()/2);
            _primaryDataUnitSprite.pushRotated(270, TFT_BLACK);

            _secondaryDataUnitSprite.drawString("M/S", _secondaryDataUnitSprite.width()/2, _secondaryDataUnitSprite.height()/2);
            _secondaryDataUnitSprite.pushRotated(270, TFT_BLACK);

            _tertiaryDataUnitSprite.drawString("C", _tertiaryDataUnitSprite.width()/2, _tertiaryDataUnitSprite.height()/2);
            _tertiaryDataUnitSprite.pushRotated(270, TFT_BLACK);
        }
    }
}

void DisplayGC9A01::clearUnit(void)
{
    _primaryDataUnitSprite.fillSprite(TFT_BLACK);
    _primaryDataUnitSprite.pushRotated(270, TFT_BLUE);

    _secondaryDataUnitSprite.fillSprite(TFT_BLACK);
    _secondaryDataUnitSprite.pushRotated(270, TFT_BLUE);

    _tertiaryDataUnitSprite.fillSprite(TFT_BLACK);
    _tertiaryDataUnitSprite.pushRotated(270, TFT_BLUE);
}

void DisplayGC9A01::clearData(int placement)
{
    switch(placement)
    {
        case 0: 
            _primaryDataSprite.fillSprite(TFT_BLACK);
            _primaryDataSprite.pushRotated(270, TFT_BLUE);
            break;

         case 1: 
            _secondaryDataSprite.fillSprite(TFT_BLACK);
            _secondaryDataSprite.pushRotated(270, TFT_BLUE);
            break;

        case 2:
            _tertiaryDataSprite.fillSprite(TFT_BLACK);
            _tertiaryDataSprite.pushRotated(270, TFT_BLUE); 
            break;

        case 3:
            _currentTimeSprite.fillSprite(TFT_BLACK);
            _currentTimeSprite.pushRotated(270, TFT_BLUE);
            break;
    }
}

void DisplayGC9A01::drawTime(String actual_time)
{
    // time format : XX:YY:ZZ
    // we just need to keep hours (XX), and minutes (YY)
    clearData(3);

    String hours = actual_time.substring(0, 2);
    String minutes = actual_time.substring(3, 5);

    _currentTimeSprite.drawString(hours, _currentTimeSprite.width() / 2, 8);
    _currentTimeSprite.drawLine(10, _currentTimeSprite.height() / 2, _currentTimeSprite.width() - 8, _currentTimeSprite.height() / 2, TFT_DARKGREY);
    _currentTimeSprite.drawString(minutes, _currentTimeSprite.width() / 2, (_currentTimeSprite.height()/2)+8);
    _currentTimeSprite.pushRotated(270, TFT_BLACK);
}

//Battery management
void DisplayGC9A01::deviceBatteryManagement(uint8_t batteryLevel, bool arcRoundedEnd, uint8_t thickness)
{
    _inner_radius = _radius - (thickness / 2);

    if (batteryLevel > 110)
        _arcBatteryColor = GREEN;
    else if (batteryLevel > 65 && batteryLevel <= 109)
        _arcBatteryColor = YELLOW;
    else if (batteryLevel > 40 && batteryLevel <= 65)
        _arcBatteryColor = ORANGE;
    else
        _arcBatteryColor = RED;

    // 160 because batteryLevel is mapped from 0->4096 to 1->159
    _display.drawSmoothArc(_centerX, _centerY, _radius-3, _inner_radius-3, 200 + (160 - batteryLevel), 360, _arcBatteryColor, _backgroundColor, arcRoundedEnd);
    _display.drawSmoothArc(_centerX, _centerY, _radius-3, _inner_radius-3, 200, 200 + (160 - batteryLevel), TFT_BLACK, _backgroundColor, arcRoundedEnd);
}

void DisplayGC9A01::phoneBatteryManagement(uint8_t batteryLevel, bool arcRoundedEnd, uint8_t thickness)
{
    _inner_radius = _radius - (thickness / 2);

    if (batteryLevel > 110)
        _arcBatteryColor = GREEN;
    else if (batteryLevel > 65 && batteryLevel <= 109)
        _arcBatteryColor = YELLOW;
    else if (batteryLevel > 40 && batteryLevel <= 65)
        _arcBatteryColor = ORANGE;
    else
        _arcBatteryColor = RED;

    _display.drawSmoothArc(_centerX, _centerY, _radius - BATTERY_PHONE_ARC_OFFSET, _inner_radius - BATTERY_PHONE_ARC_OFFSET, 200 + (160 - batteryLevel), 360, _arcBatteryColor, _backgroundColor, arcRoundedEnd);
    _display.drawSmoothArc(_centerX, _centerY, _radius - BATTERY_PHONE_ARC_OFFSET, _inner_radius - BATTERY_PHONE_ARC_OFFSET, 200, 200 + (160 - batteryLevel), TFT_BLACK, _backgroundColor, arcRoundedEnd);
}

//Icons
void DisplayGC9A01::pngDrawBLELogoStatus(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(BLE_LOGO_POSX, BLE_LOGO_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
void DisplayGC9A01::pngDrawLogo(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(LOGO_MOUNTED_POSX, LOGO_MOUNTED_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
void DisplayGC9A01::pngDrawMountainIconMenu(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(MOUTAIN_ICON_POSX, MOUTAIN_ICON_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
void DisplayGC9A01::pngDrawUrbanIconMenu(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(URBAN_ICON_POSX, URBAN_ICON_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
void DisplayGC9A01::pngDrawCustomIconMenu(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(CUSTOM_ICON_POSX, CUSTOM_ICON_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void DisplayGC9A01::pngDrawMountainIconActualMode(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(MOUTAIN_ICON_ACTUAL_MODE_POSX, MOUTAIN_ICON_ACTUAL_MODE_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
void DisplayGC9A01::pngDrawUrbanIconActualMode(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(URBAN_ICON_ACTUAL_MODE_POSX, URBAN_ICON_ACTUAL_MODE_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
void DisplayGC9A01::pngDrawCustomIconActualMode(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(CUSTOM_ICON_ACTUAL_MODE_POSX, CUSTOM_ICON_ACTUAL_MODE_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
