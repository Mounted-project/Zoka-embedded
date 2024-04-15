#include "Arduino.h"
#include "ECX334.hpp"

constexpr uint8_t LCD_SPI_MISO = 4;
constexpr uint8_t LCD_SPI_MOSI = 5;
constexpr uint8_t LCD_SPI_SCLK = 6;
constexpr uint8_t LCD_SPI_CS = 7;

SPIClass spi(HSPI); // Use HSPI peripheral
SPISettings spiSettings(800000, LSBFIRST, SPI_MODE0);

void writeSPIRegister(byte reg, byte value)
{
    spi.beginTransaction(spiSettings);
    digitalWrite(LCD_SPI_CS, LOW);
    spi.transfer(reg);
    spi.transfer(value);
    digitalWrite(LCD_SPI_CS, HIGH);
    spi.endTransaction();
}

byte readSPIRegister(byte reg)
{
    // Step 1: Set RD_ON to 1
    writeSPIRegister(0x80, 0x01); // Assuming 0x01 is the correct value to enable read mode

    // Step 2: Write the address of the read data on RD_ADDR
    writeSPIRegister(0x81, reg);

    // Depending on the device, you might need to wait a bit for the data to be ready
    delayMicroseconds(100); // Adjust timing based on the device's requirements

    // Step 3: Read the data
    spi.beginTransaction(spiSettings);
    digitalWrite(LCD_SPI_CS, LOW);

    // The actual read operation might not require sending a specific command here,
    // since you've already set up the read via the two writes above.
    // A dummy transfer might be needed to clock out the data.
    byte readData = spi.transfer(0x00); // Perform a dummy write to read the data

    digitalWrite(LCD_SPI_CS, HIGH);
    spi.endTransaction();

    return readData;
}
void initLcdSpi(void)
{
    pinMode(LCD_SPI_CS, OUTPUT);
    pinMode(LCD_SPI_MOSI, OUTPUT);
    pinMode(LCD_SPI_MISO, INPUT);
    pinMode(LCD_SPI_SCLK, OUTPUT);
    spi.begin(LCD_SPI_SCLK, LCD_SPI_MISO, LCD_SPI_MOSI, LCD_SPI_CS);
    pinMode(LCD_SPI_CS, OUTPUT);
    digitalWrite(LCD_SPI_CS, HIGH);
}
