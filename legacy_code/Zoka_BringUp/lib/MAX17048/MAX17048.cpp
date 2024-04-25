#include "MAX17048.h"
#include "Arduino.h"

MAX17048::MAX17048(uint8_t address, TwoWire& bus){
	_deviceAddr = address;
	_bus = &bus;
}
MAX17048::~MAX17048() {

}


uint16_t MAX17048::adc() { return read(REG::VCELL); }

float MAX17048::voltage() { return (float)read(REG::VCELL) * 78.125f * 1 / 1000000.f; }

uint8_t MAX17048::percent() { return (uint8_t)(read(REG::SOC) / 256); }

float MAX17048::accuratePercent() { return (float)read(REG::SOC) / 256.f; }

void MAX17048::quickStart() {
    uint16_t v = read(REG::MODE);
    bitSet(v, 14);
    write(REG::MODE, v);
}

void MAX17048::enableSleep(bool b) {
    uint16_t v = read(REG::MODE);
    bitWrite(v, 13, b);
    write(REG::MODE, v);
}

bool MAX17048::isSleepable() { return bitRead(read(REG::MODE), 13); }

uint16_t MAX17048::mode() { return read(REG::MODE); }

uint8_t MAX17048::version() { return (uint8_t)read(REG::VERSION); }

bool MAX17048::isHibernating() { return bitRead(read(REG::MODE), 12); }

float MAX17048::hibernateActTh() { return (float)highByte(read(REG::HIBRT)) * 0.00125; }

float MAX17048::hibernateHibTh() { return (float)lowByte(read(REG::HIBRT)) * 0.208; }

void MAX17048::hibernateActTh(float th) {
    uint16_t v = read(REG::HIBRT) & 0xFF00;
    if (th > 0.0) {
        if (th < 0.31875) v |= (uint16_t)(th / 0.00125) & 0x00FF;
        else v |= 0x00FF;
    }
    write(REG::HIBRT, v);
}

void MAX17048::hibernateHibTh(float th) {
    uint16_t v = read(REG::HIBRT) & 0x00FF;
    if (th > 0.0) {
        if (th < 53.04) v |= (uint16_t)(th / 0.208) << 8;
        else v |= 0xFF00;
    }
    write(REG::HIBRT, v);
}

void MAX17048::tempCompensate(float temp) {
    uint8_t v = 0;
    if (temp > 20.0) v = 0x97 + (temp - 20.0) * -0.5;
    else v = 0x97 + (temp - 20.0) * -5.0;
    rcomp(v);
}

void MAX17048::rcomp(uint8_t rcomp) {
    uint16_t v = (read(REG::CONFIG) & 0x00FF) | (rcomp << 8);
    write(REG::CONFIG, v);
}

void MAX17048::sleep(bool b) {
    uint16_t v = read(REG::CONFIG);
    bitWrite(v, 7, b);
    write(REG::CONFIG, v);
}

bool MAX17048::isAlerting() { return bitRead(read(REG::CONFIG), 5); }

void MAX17048::clearAlert() {
    uint16_t v = read(REG::CONFIG);
    bitClear(v, 5);
    write(REG::CONFIG, v);
}

uint8_t MAX17048::emptyAlertThreshold() { return 32 - (read(REG::CONFIG) & 0x001F); }

void MAX17048::setEmptyAlertThreshold(uint8_t th) {
    uint16_t v = read(REG::CONFIG);
    th = constrain(th, 1, 32);
    v &= 0xFFE0;
    v |= 32 - th;
    write(REG::CONFIG, v);
}

float MAX17048::vAlertMinThreshold() { return highByte(read(REG::VALRT)) * 0.02; }

void MAX17048::setVAlertMinThreshold(float th) {
    uint16_t v = read(REG::VALRT) & 0x00FF;
    if (th > 0.0) {
        if (th < 5.1) v |= (uint16_t)(th/ 0.02) << 8;
        else v |= 0xFF00;
    }
    write(REG::VALRT, v);
}

float MAX17048::vAlertMaxThreshold() { return (read(REG::VALRT) & 0x00FF) * 0.02; }

void MAX17048::setVAlertMaxThreshold(float th) {
    uint16_t v = read(REG::VALRT) & 0xFF00;
    if (th > 0.0) {
        if (th < 5.1) v |= (uint8_t)(th / 0.02);
        else v |= 0x00FF;
    }
    write(REG::VALRT, v);
}
// float MAX17048::vResetThreshold() { return read(static_cast<uint8_t>(REG::VRESET_ID) >> 9) * 0.04; }
float MAX17048::vResetThreshold() { return read(static_cast<REG>(static_cast<uint8_t>((REG::VRESET_ID))>> 9) ) * 0.04; }

// float MAX17048::vResetThreshold() { return read((REG::VRESET_ID) >> 9) * 0.04; }

void MAX17048::setVResetThreshold(float th) {
    uint16_t v = read(REG::VRESET_ID) & 0x01FF;
    if (th > 0.0) {
        if (th < 5.08) v |= (uint16_t)(th / 0.04) << 9;
        else v |= 0xFE00;
    }
    write(REG::VRESET_ID, v);
}

bool MAX17048::isComparatorEnabled() { return bitRead(read(REG::VRESET_ID), 8); }

void MAX17048::setComparatorEnabled(bool b) {
    uint16_t v = read(REG::VRESET_ID);
    bitWrite(v, 8,  b);
    write(REG::VRESET_ID, v);
}

uint8_t MAX17048::getId() { return lowByte(read(REG::VRESET_ID)); }

bool MAX17048::isVResetAlertEnabled() { return bitRead(read(REG::STATUS), 14); }

void MAX17048::setVResetAlertEnabled(bool b) {
    uint16_t v = read(REG::STATUS);
    bitWrite(v, 14, b);
    write(REG::STATUS, v);
}

uint8_t MAX17048::getAlertFlags() { return highByte(read(REG::STATUS)) & 0x3F; }

void MAX17048::clearAlertFlags(uint8_t flags) {
    uint16_t v = read(REG::STATUS);
    v &= ~((flags & 0x3F) << 8);
    write(REG::STATUS, v);
}

float MAX17048::getCrate() { return (float)read(REG::CRATE) * 0.208f; }

uint8_t MAX17048::getStatus() { return read(REG::STATUS); }

bool MAX17048::isHighVoltage() { return bitRead(getAlertFlags(), 1); }

bool MAX17048::isLowVoltage() { return bitRead(getAlertFlags(), 2); }

bool MAX17048::isResetVoltage() { return bitRead(getAlertFlags(), 3); }

bool MAX17048::isLowSOC() { return bitRead(getAlertFlags(), 4); }

bool MAX17048::isChnageSOC() { return bitRead(getAlertFlags(), 5); }

void MAX17048::reset() { write(REG::CMD, 0x5400); }

void MAX17048::write(const REG reg, const bool stop = true)
{
    _bus->beginTransmission(_deviceAddr);
    _bus->write((uint8_t)reg);
    _bus->endTransmission(stop);
}

void MAX17048::write(const MAX17048::REG reg, const uint16_t data, const bool stop = true)
{
    _bus->beginTransmission(_deviceAddr);
    _bus->write((uint8_t)reg);
    _bus->write((data & 0xFF00) >> 8);
    _bus->write((data & 0x00FF) >> 0);
    _bus->endTransmission(stop);
}

uint16_t MAX17048::read(const REG reg)
{
    MAX17048::write(reg, false);
    _bus->requestFrom((uint8_t)_deviceAddr, (uint8_t)2); // 2byte R/W only
    uint16_t data = (uint16_t)((_bus->read() << 8) & 0xFF00);
    data |= (uint16_t)(_bus->read() & 0x00FF);
    return data;
}

uint8_t MAX17048::toUnderlying(REG value) {
    return static_cast<uint8_t>(value);
}
