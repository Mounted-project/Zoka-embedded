#ifndef ECX334_HPP
#define ECX334_HPP
#include <SPI.h>

byte readSPIRegister(byte reg);
void writeSPIRegister(byte reg, byte value);
void initLcdSpi(void);
#endif