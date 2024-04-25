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

//ESP32-S3 GPIOs
constexpr uint8_t ESP_POWER_EN = 44;
constexpr uint8_t I2C_SDA = 15;
constexpr uint8_t I2C_SCL = 2;
