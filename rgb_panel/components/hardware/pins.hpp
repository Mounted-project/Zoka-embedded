#ifndef PINS_HPP
#define PINS_HPP

constexpr uint8_t MCP23017_ADDR = 0x20;
constexpr uint8_t MAX17048_ADDR = 0x36;
constexpr uint8_t BQ24296_ADDR = 0x6B;

// MCP23017 BANK A
constexpr uint8_t ENABLE_1_8V = 0;
constexpr uint8_t ENABLE_10V = 1;
constexpr uint8_t RGB_LCD_LSB = 2;
constexpr uint8_t BQ_CHARGE_STATUS = 3;
constexpr uint8_t BQ_PGOOD = 4;

// MCP23017 BANK B
constexpr uint8_t BUTTON_ENCODER = 8;
constexpr uint8_t BUTTON1 = 9;
constexpr uint8_t BUTTON2 = 10;
constexpr uint8_t LEDS_ENABLE = 11;
constexpr uint8_t RGB_LED_R = 12;
constexpr uint8_t RGB_LED_G = 13;
constexpr uint8_t RGB_LED_B = 14;

// ESP32-S3 GPIOs
constexpr gpio_num_t ESP_POWER_EN = GPIO_NUM_44;
constexpr uint8_t I2C_SDA = 15;
constexpr uint8_t I2C_SCL = 2;

constexpr uint8_t LCD_PCLK = 9;
constexpr uint8_t LCD_HSYNC = 3;
constexpr uint8_t LCD_VSYNC = 46;
constexpr uint8_t LCD_XCLR = 8;

constexpr uint8_t LCD_B0 = 13; // B0
constexpr uint8_t LCD_B1 = 14; // B1
constexpr uint8_t LCD_B2 = 45; // B2
constexpr uint8_t LCD_B3 = 47; // B3
constexpr uint8_t LCD_B4 = 48; // B4
constexpr uint8_t LCD_G0 = 39; // G0
constexpr uint8_t LCD_G1 = 38; // G1
constexpr uint8_t LCD_G2 = 12; // G2
constexpr uint8_t LCD_G3 = 10; // G3
constexpr uint8_t LCD_G4 = 11; // G4
constexpr uint8_t LCD_G5 = 11; // G5
constexpr uint8_t LCD_R0 = 1;  // R0
constexpr uint8_t LCD_R1 = 43; // R1
constexpr uint8_t LCD_R2 = 42; // R2
constexpr uint8_t LCD_R3 = 41; // R3
constexpr uint8_t LCD_R4 = 40; // R4
#endif                         // PINS_HPP
