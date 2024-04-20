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
constexpr uint8_t ESP_POWER_EN = 44;
constexpr uint8_t I2C_SDA = 15;
constexpr uint8_t I2C_SCL = 2;

uint8_t LCD_XCLR = 8;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define EXAMPLE_PIN_NUM_BK_LIGHT -1 // NOT SURE IF IT WORKS
uint8_t EXAMPLE_PIN_NUM_HSYNC = 46;
uint8_t EXAMPLE_PIN_NUM_VSYNC = 3;
uint8_t EXAMPLE_PIN_NUM_DE = 0; // NOT SURE IF IT WORKS
uint8_t EXAMPLE_PIN_NUM_PCLK = 9;
uint8_t B0 = 13;  // B0
uint8_t B1 = 14;  // B1
uint8_t B2 = 45;  // B2
uint8_t B3 = 47;  // B3
uint8_t B4 = 48;  // B4
uint8_t G0 = 39;  // G0
uint8_t G1 = 38;  // G1
uint8_t G2 = 12;  // G2
uint8_t G3 = 10;  // G3
uint8_t G4 = 11;  // G4
uint8_t G5 = 11; // G5
uint8_t R0 = 1;  // R0
uint8_t R1 = 43; // R1
uint8_t R2 = 42; // R2
uint8_t R3 = 41; // R3
uint8_t R4 = 40; // R4
uint8_t EXAMPLE_PIN_NUM_DISP_EN = 0; // NOT SURE IF IT WORKS
