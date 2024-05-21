#ifndef BLE_Hud_Service_H
#define BLE_Hud_Service_H

#include <Arduino.h>
#include <BLEDevice.h>

#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "data_exchange.hpp"
using namespace std;
static bool deviceConnected;

class BLE_Hud_Service
{
public:
    BLE_Hud_Service(dataExchange *data_exchange);
    void initserviceUUID(void);
    void createService(void);
    bool deviceDisconnected;
    void hudconnected2(uint8_t battery_level);

private:
    dataExchange *data_exchange;
    BLEServer *pServer;
    BLEServer *pServiceUserData;
    BLEServer *pServiceDataService;
    // Service User data
    BLECharacteristic *pCharacteristic;
    BLECharacteristic *pCharacteristicSpeed;
    BLECharacteristic *pCharacteristicAltitude;
    BLECharacteristic *pCharacteristicTime;
    BLECharacteristic *pCharacteristicGps;
    BLECharacteristic *pCharacteristicPhoneBattery;
    BLECharacteristic *pCharacteristicWeatherTemp;
    // Service Data service
    BLECharacteristic *pCharacteristicSw;
    BLECharacteristic *pCharacteristicHw;
    BLECharacteristic *pCharacteristicBattery;

    BLEAdvertising *pAdvertising;

    std::string _get_speed;
    std::string _get_altitude;
    std::string _get_time;
    std::string _get_gps;
    std::string _get_phone_battery;
    std::string _get_local_temperature;

    char _batteryLevel[8];
    char _old_batteryLevel[8] = "100";

    int _bL;
    int _old_bL;
    int testbat;
};

#endif