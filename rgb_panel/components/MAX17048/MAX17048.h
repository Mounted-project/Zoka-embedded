#pragma once
#ifndef MAX17048_H
#define MAX17048_H

#include <Wire.h>

class MAX17048
{

    enum class REG
    {
        VCELL = 0x02,
        SOC = 0x04,
        MODE = 0x06,
        VERSION = 0x08,
        HIBRT = 0x0A,
        CONFIG = 0x0C,
        VALRT = 0x14,
        CRATE = 0x16,
        VRESET_ID = 0x18,
        STATUS = 0x1A,
        TABLE = 0x40,
        CMD = 0xFE
    };

public:
    MAX17048(uint8_t address, TwoWire &bus);
    virtual ~MAX17048();
    uint16_t adc();
    float voltage();
    uint8_t percent();
    float accuratePercent();
    void quickStart();
    void enableSleep(bool b);
    bool isSleepable();
    uint16_t mode();
    uint8_t version();
    bool isHibernating();
    float hibernateActTh();
    float hibernateHibTh();
    void hibernateActTh(float th);
    void hibernateHibTh(float th);
    void tempCompensate(float temp);
    void rcomp(uint8_t rcomp);
    void sleep(bool b);
    bool isAlerting();
    void clearAlert();
    uint8_t emptyAlertThreshold();
    void setEmptyAlertThreshold(uint8_t th);
    float vAlertMinThreshold();
    void setVAlertMinThreshold(float th);
    float vAlertMaxThreshold();
    void setVAlertMaxThreshold(float th);
    float vResetThreshold();
    void setVResetThreshold(float th);
    bool isComparatorEnabled();
    void setComparatorEnabled(bool b);
    uint8_t getId();
    bool isVResetAlertEnabled();
    void setVResetAlertEnabled(bool b);
    uint8_t getAlertFlags();
    void clearAlertFlags(uint8_t flags);
    float getCrate();
    uint8_t getStatus();
    bool isHighVoltage();
    bool isLowVoltage();
    bool isResetVoltage();
    bool isLowSOC();
    bool isChnageSOC();
    void reset();

private:
    uint8_t toUnderlying(const MAX17048::REG value);
    TwoWire *_bus;
    uint8_t _deviceAddr;
    void write(const MAX17048::REG reg, const bool stop);
    void write(const MAX17048::REG reg, const uint16_t data, const bool stop);
    uint16_t read(const MAX17048::REG reg);
    bool b_quick_start{false};
    bool b_sleep{false};
    TwoWire *wire;
};

#endif // MAX17048_H
