#ifndef ECX334_HPP
#define ECX334_HPP
#include <SPI.h>

byte readSPIRegister(byte reg);
void writeSPIRegister(byte reg, byte value);
void initLcdSpi(void);

#endif

// struct r_0X00
// {
//     bool *r_RGB_YCB : 1;
//     bool *r_YCB_DEC : 1;

// }

// struct registers_t
// {
//     bool r_PS0 : 1;
//     bool *r_MCLKPOL : 1;
//     bool *r_RGT DWN : 1;
//     bool *r_YCB_DEC : 1;
//     bool *r_RGB_YCB : 1;
//     bool *r_YCB_P : 1;
//     bool *r_VCAL_MON : 1;
//     uint8_t r_ORBIT_H : 5;
//     uint8_t r_ORBIT_V : 5;
//     bool *r_DITHERON : 1;
//     bool *r_LDOEXT : 1;
//     uint8_t r_LUMINANCE : 3;
//     bool *r_OTPDG_REGDIS : 1;
//     bool *r_OTPCALDAC_REGDIS : 1;
//     uint8_t r_DRGAMMA32 : 5;
//     uint8_t r_DRGAMMA64 : 5;
//     uint8_t r_DRGAMMA128 : 5;
//     uint8_t r_DRGAMMA256 : 5;
//     uint8_t r_DGGAMMA32 : 5;
//     uint8_t r_DGGAMMA64 : 5;
//     uint8_t r_DBGAMMA32 : 5;
//     uint8_t r_DBGAMMA64 : 5;
//     uint8_t r_DBGAMMA128 : 5;
//     uint8_t r_DBGAMMA256 : 5;
//     uint16_t r_CONT : 9;
//     uint8_t ur_RCONT : 7;
//     uint8_t ur_GCONT : 7;
//     uint8_t ur_BCONT : 7;
//     uint8_t r_BRT : 8;
//     uint8_t ur_RBRT : 7;
//     uint8_t ur_GBRT : 7;
//     uint8_t ur_BBRT : 7;
//     uint8_t r_LDOSEL : 5;
//     uint8_t r_CALDAC : 8;
//     uint8_t r_H_ACT_U : 8;
//     uint16_t r_H_ACT_D : 11;
//     uint8_t r_V_ACT_U : 8;
//     uint16_t r_V_ACT_D : 11;
//     uint16_t r_DE_U : 11;
//     uint16_t r_DE_D : 11;
//     uint_16_t r_WSST1_U : 10;
//     uint_16_t r_WSST1_D : 10;
//     uint_16_t r_WSST2_U : 10;
//     uint_16_t r_WSST2_D : 10;
//     uint8_t r_WSEN1_U : 8;
//     uint8_t r_WSEN1_W : 8;
//     uint16_t r_WSEN2_U : 11;
//     uint8_t r_WSEN2_W : 8;
//     uint8_t r_WSEN3_U : 8;
//     uint8_t r_WSEN3_W : 8;
//     uint8_t r_DESN_U : 8;
//     uint16_t r_DSEN_W : 11;
//     uint_16_t r_VCK_U : 10;
//     uint_16_t r_VCK_W : 10;
//     uint_16_t uint16_t r_SIGSELREF_U : 10;
//     uint8_t r_SIGSELREF_W : 8;
//     uint8_t r_SIGSELOFS_U : 4;
//     uint8_t r_SIGSELOFS_W : 8;
//     uint8_t r_SIGSEL_U : 4;
//     uint_16_t r_SIGSEL_W : 10;
//     uint_16_t r_SELREF_U : 10;
//     uint8_t r_SELREF_W : 8;
//     uint8_t r_SELOFS_U : 8;
//     uint8_t r_SELOFS_W : 8;
//     uint_16_t r_SEL_U : 10;
//     uint_16_t r_SEL_W : 10;
//     bool r_120MODE : 1;
//     bool r_RD_ON : 1;
//     uint8_t r_RD_ADDR : 8;
// }