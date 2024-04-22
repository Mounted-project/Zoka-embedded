#include "Arduino.h"
#include "ECX334.hpp"
#include <SPI.h>

constexpr uint8_t LCD_SPI_MISO = 4;
constexpr uint8_t LCD_SPI_MOSI = 5;
constexpr uint8_t LCD_SPI_SCLK = 6;
constexpr uint8_t LCD_SPI_CS = 7;

SPIClass spi(HSPI); // Use HSPI peripheral
SPISettings spiSettings(800000, LSBFIRST, SPI_MODE3);

void writeSPIRegister(byte reg, byte value)
{
    uint8_t data[2] = {reg,value};
    spi.beginTransaction(spiSettings);
    digitalWrite(LCD_SPI_CS, LOW);
    spi.transferBytes(data,NULL,sizeof(data));
    digitalWrite(LCD_SPI_CS, HIGH);
    spi.endTransaction();
}

byte readSPIRegister(byte reg)
{
    uint8_t read_reg = 0x81;
    uint8_t data[2] = {read_reg,reg};
    byte reg_result[2]={0xFF,0xFF};
    writeSPIRegister(0x80,0x01); // SET TO READ MODE

    spi.beginTransaction(spiSettings);
    digitalWrite(LCD_SPI_CS, LOW);
    spi.transferBytes(data,NULL,sizeof(data));
    digitalWrite(LCD_SPI_CS, HIGH);
    spi.endTransaction();

    spi.beginTransaction(spiSettings);
    digitalWrite(LCD_SPI_CS, LOW);
    spi.transferBytes(data,reg_result,sizeof(data));
    digitalWrite(LCD_SPI_CS, HIGH);
    spi.endTransaction();
    // Step 3: Read the data
    return (uint16_t)reg_result[0] << 8 | reg_result[1];
    //  writeSPIRegister(0x80,0x00); // SET TO DEFAULT MODE
}
void initLcdSpi(void)
{
    pinMode(LCD_SPI_CS, OUTPUT);
    // pinMode(LCD_SPI_MOSI, OUTPUT);
    // pinMode(LCD_SPI_MISO, INPUT);
    // pinMode(LCD_SPI_SCLK, OUTPUT);
    spi.begin(LCD_SPI_SCLK, LCD_SPI_MISO, LCD_SPI_MOSI, LCD_SPI_CS);

}
