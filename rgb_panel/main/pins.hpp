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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ 54000000
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL 1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_BK_LIGHT -1 // NOT SURE IF IT WORKS
#define EXAMPLE_PIN_NUM_HSYNC 46
#define EXAMPLE_PIN_NUM_VSYNC 3
#define EXAMPLE_PIN_NUM_DE -1 // NOT SURE IF IT WORKS
#define EXAMPLE_PIN_NUM_PCLK 9
#define EXAMPLE_PIN_NUM_DATA0 13  // B0
#define EXAMPLE_PIN_NUM_DATA1 14  // B1
#define EXAMPLE_PIN_NUM_DATA2 45  // B2
#define EXAMPLE_PIN_NUM_DATA3 47  // B3
#define EXAMPLE_PIN_NUM_DATA4 48  // B4
#define EXAMPLE_PIN_NUM_DATA5 39  // G0
#define EXAMPLE_PIN_NUM_DATA6 38  // G1
#define EXAMPLE_PIN_NUM_DATA7 12  // G2
#define EXAMPLE_PIN_NUM_DATA8 10  // G3
#define EXAMPLE_PIN_NUM_DATA9 11  // G4
#define EXAMPLE_PIN_NUM_DATA10 11 // G5
#define EXAMPLE_PIN_NUM_DATA11 1  // R0
#define EXAMPLE_PIN_NUM_DATA12 43 // R1
#define EXAMPLE_PIN_NUM_DATA13 42 // R2
#define EXAMPLE_PIN_NUM_DATA14 41 // R3
#define EXAMPLE_PIN_NUM_DATA15 40 // R4
#define EXAMPLE_PIN_NUM_DISP_EN -1

#define LCD_SPI_MISO 4
#define LCD_SPI_MOSI 5
#define LCD_SPI_SCLK 6
#define LCD_SPI_CS 7

#define LCD_XCLR 8