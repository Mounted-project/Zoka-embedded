/**
 * Name: bq2429x
 * Author: Luka Mustafa - Institute IRNAS Race { info@irnas.eu }
 * Version: 1.0
 * Description: A library for interfacing the MAXIM MAX17201/MAX17205
 * 				Li+ fuel gauges.
 * Source: https://github.com/pAIgn10/bq2429x
 * License: Copyright (c) 2017 Nick Lamprianidis 
 *          This library is licensed under the GPL license
 *          http://www.opensource.org/licenses/mit-license.php
 * Inspiration: The library is inspired by: https://github.com/pAIgn10/bq2429x
 * Filename: bq2429x.cpp
 * File description: Definitions and methods for the bq2429x library
 */

#include "bq2429x.h"

// Initializes variables and the Wire library
bq2429x::bq2429x(uint8_t address, TwoWire& bus) { 
	_deviceAddr = address;
	_bus = &bus;
}

bq2429x::~bq2429x(){}
// _bus->requestFrom(_deviceAddr, (int)1,HIGH);

uint8_t bq2429x::getStatus()
{
	_bus->beginTransmission(_deviceAddr);
	_bus->write(bq2429x_STATUS_ADDR);
	_bus->endTransmission(false);
	_bus->requestFrom(static_cast<uint16_t>(_deviceAddr), static_cast<uint8_t>(1), static_cast<uint8_t>(HIGH));
    return _bus->read();
}  

uint8_t bq2429x::getFaults()
{
	_bus->beginTransmission(_deviceAddr);
	_bus->write(bq2429x_FAULT_ADDR);
	_bus->endTransmission(false);
	_bus->requestFrom(static_cast<uint16_t>(_deviceAddr), static_cast<uint8_t>(1), static_cast<uint8_t>(HIGH)); //send stop
    return _bus->read();
}  
   
   
void bq2429x::setChargeVoltage(double voltage)
{
	_bus->beginTransmission(_deviceAddr);
	_bus->write(bq2429x_CHARGE_VOL_CTRL_ADDR);
	_bus->endTransmission(false);
	_bus->requestFrom(static_cast<uint16_t>(_deviceAddr), static_cast<uint8_t>(1), static_cast<uint8_t>(HIGH));
    
    uint8_t current_value = _bus->read();
    uint8_t new_value = voltage;
    Serial.print("Actual Charge Voltage Settings :");
	Serial.println(current_value);
    _bus->beginTransmission(_deviceAddr);
	_bus->write(bq2429x_CHARGE_VOL_CTRL_ADDR);
    _bus->write(new_value);
    _bus->endTransmission(false);
    return;
}

void bq2429x::setChargeCurrent(double current)
{
	_bus->beginTransmission(_deviceAddr);
	_bus->write(bq2429x_CHARGE_CUR_CTRL_ADDR);
	_bus->endTransmission(false);
	_bus->requestFrom(static_cast<uint16_t>(_deviceAddr), static_cast<uint8_t>(1), static_cast<uint8_t>(HIGH));
    
    uint8_t current_value = _bus->read();
    uint8_t new_value = current;
    Serial.print("Actual Charge Current Settings :");
	Serial.println(current_value);

    _bus->beginTransmission(_deviceAddr);
	_bus->write(bq2429x_CHARGE_CUR_CTRL_ADDR);
    _bus->write(new_value);
    _bus->endTransmission(false);
    return;
}

void bq2429x::setInputCurrentLimit(double current)
{
	_bus->beginTransmission(_deviceAddr);
	_bus->write(bq2429x_INPUT_CTRL_ADDR);
	_bus->endTransmission(false);
	_bus->requestFrom(static_cast<uint16_t>(_deviceAddr), static_cast<uint8_t>(1), static_cast<uint8_t>(HIGH));
    
    uint8_t current_value = _bus->read();
    uint8_t new_value = current;
	Serial.print("Actual Input Current Limit Settings :");
	Serial.println(current_value);
    
    _bus->beginTransmission(_deviceAddr);
	_bus->write(bq2429x_INPUT_CTRL_ADDR);
    _bus->write(new_value);
    _bus->endTransmission(false);
    return;
}

void bq2429x::setInputVoltageLimit(double voltage)
{
	_bus->beginTransmission(_deviceAddr);
	_bus->write(bq2429x_INPUT_CTRL_ADDR);
	_bus->endTransmission(false);
	_bus->requestFrom(static_cast<uint16_t>(_deviceAddr), static_cast<uint8_t>(1), static_cast<uint8_t>(HIGH));
    
    uint8_t current_value = _bus->read();
    uint8_t new_value = voltage;
	Serial.print("Actual Input Voltage Limit Settings :");
	Serial.println(current_value);
    
    _bus->beginTransmission(_deviceAddr);
	_bus->write(bq2429x_INPUT_CTRL_ADDR);
    _bus->write(new_value);
    _bus->endTransmission(false);
    return;
}