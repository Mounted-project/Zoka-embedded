#include "BLE_Hud_Service.hpp"
#include "data_exchange.hpp"
// Define UART service UUID & Charactéristique UUID
#define AUGMOUNTED_SERVICE_UUID "3496c184-9e6b-456e-9ac5-42bb7e8db1a6"

#define USER_DATA_SERVICE_UUID "16c20be6-87de-492b-9ca9-c4802f73450f"
#define CHARACTERISTIC_UUID_SPEED "e8884cc8-89ad-442e-8133-c744b6c723b5"
#define CHARACTERISTIC_UUID_ALTITUDE "2bb580f0-cc1d-4a3a-8ce3-2110f8fdf7f1"
#define CHARACTERISTIC_UUID_TIME "bf4eeeb0-d5ca-49c3-9c8f-64767dde3444"
#define CHARACTERISTIC_UUID_GPS "44ab84ab-3d7b-4c14-bf62-26d2dabf156f"
#define CHARACTERISTIC_UUID_PHONE_BATT "260a7202-ce80-42be-8b13-ebf10e433c57"
#define CHARACTERISTIC_UUID_WEATHER_TEMP "bbbcc252-1068-43c4-b4ad-af0a78c44d69"

#define DEVICE_DATA_SERVICE_UUID "11ad4651-4704-427f-9514-55bdef0f09c4"
#define CHARACTERISTIC_UUID_SW "22b58c42-60d2-4af4-ad43-86967f8a0c2b"
#define CHARACTERISTIC_UUID_HW "3ad8d4c7-547c-4ff0-a987-e7ce65e9da94"
#define CHARACTERISTIC_UUID_BATTERY "68b77095-ca71-4e71-a31a-477cef200b0e"

class MyServerCallbacks : public BLEServerCallbacks
{
private:
    dataExchange *data_exchange;

public:
    MyServerCallbacks(dataExchange *exchange) : data_exchange(exchange) {}

    // Using the class MyServerCallbacks to use the following functions
    // to know if the client is connected to the hud or not.
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        Serial.println("Client Connect");
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("Client Disconnect");
    }
};

BLE_Hud_Service::BLE_Hud_Service(dataExchange *data_exchange) : data_exchange(data_exchange)
{
    pServer = NULL;
    pAdvertising = BLEDevice::getAdvertising();
    pServiceUserData = NULL;
    pServiceDataService = NULL;
    deviceConnected = false;
    deviceDisconnected = false;
}

void BLE_Hud_Service::initserviceUUID(void)
{
    Serial.println("1- Please scan for BLE devices in the app");
    Serial.println("2- Connect to AugMounted");

    BLEDevice::init("Mounted Zoka");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    // Create the BLE Server
    // pServer->setCallbacks(new MyServerCallbacks());

    // Create an instance of MyServerCallbacks with the pointer to data_exchange
    MyServerCallbacks *callbacks = new MyServerCallbacks(data_exchange);

    // Set the callbacks for the server
    pServer->setCallbacks(callbacks);
}

void BLE_Hud_Service::createService(void)
{
    BLEService *pServiceUserData = pServer->createService(USER_DATA_SERVICE_UUID);
    pCharacteristicSpeed = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_SPEED,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicSpeed->addDescriptor(new BLE2902());

    pCharacteristicAltitude = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_ALTITUDE,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicAltitude->addDescriptor(new BLE2902());

    pCharacteristicTime = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_TIME,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicTime->addDescriptor(new BLE2902());

    pCharacteristicGps = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_GPS,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicGps->addDescriptor(new BLE2902());

    pCharacteristicPhoneBattery = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_PHONE_BATT,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicPhoneBattery->addDescriptor(new BLE2902());

    pCharacteristicWeatherTemp = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_WEATHER_TEMP,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicWeatherTemp->addDescriptor(new BLE2902());
    pServiceUserData->start();

    BLEService *pServiceDataService = pServer->createService(DEVICE_DATA_SERVICE_UUID);
    pCharacteristicSw = pServiceDataService->createCharacteristic(
        CHARACTERISTIC_UUID_SW,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicSw->addDescriptor(new BLE2902());

    pCharacteristicHw = pServiceDataService->createCharacteristic(
        CHARACTERISTIC_UUID_HW,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicHw->addDescriptor(new BLE2902());

    pCharacteristicBattery = pServiceDataService->createCharacteristic(
        CHARACTERISTIC_UUID_BATTERY,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicBattery->addDescriptor(new BLE2902());
    // Launch of the service
    pServiceDataService->start();

    pAdvertising->addServiceUUID(AUGMOUNTED_SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising();

    Serial.println("Waiting a client connection to notify...");
}

void BLE_Hud_Service::hudconnected2(uint8_t battery_level)
{
    if (deviceConnected)
    {
        bool value_refreshed = false;
        deviceDisconnected = deviceConnected;
        data_exchange->setNavigationModeVariable(&dataExchange::NavigationModeVariables::isConnected, true);
        _get_speed = pCharacteristicSpeed->getValue().c_str();
        _get_altitude = pCharacteristicAltitude->getValue().c_str();
        _get_time = pCharacteristicTime->getValue().c_str();
        _get_gps = pCharacteristicGps->getValue().c_str();
        _get_phone_battery = pCharacteristicPhoneBattery->getValue().c_str();
        _get_local_temperature = pCharacteristicWeatherTemp->getValue().c_str();

        // Value created and assigned by hud :
        // Soft version
        pCharacteristicSw->setValue("v1.0.0");
        // Hard version
        pCharacteristicHw->setValue("v1.0.0");
        // State battery
        dtostrf(battery_level, 1, 2, _batteryLevel);
        pCharacteristicBattery->setValue(_batteryLevel);

        /*
                if(battery_level < _old_bL) {
                    pCharacteristicBattery->setValue(_batteryLevel);
                    Serial.print("_old_bL : ");
                    Serial.println(_old_bL);
                    Serial.print("_bL : ");
                    Serial.println(_bL);
                    _old_bL = _bL;
                }
        */
        // new_data._device_connected = "yes";

        // Speed management
        if (_get_speed.length() > 0)
        {

            data_exchange->setNavigationModeVariable(&dataExchange::NavigationModeVariables::actualSpeed, _get_speed);
            value_refreshed = true;
            // new_data._get_speed = _get_speed;
        }
        // Altitude management
        if (_get_altitude.length() > 0)
        {
            data_exchange->setNavigationModeVariable(&dataExchange::NavigationModeVariables::actualAltitude, _get_altitude);
            value_refreshed = true;
            // new_data._get_altitude = _get_altitude;
        }
        // Time management
        if (_get_time.length() > 0)
        {
            Serial.println("Time");
            data_exchange->setNavigationModeVariable(&dataExchange::NavigationModeVariables::actualTime, _get_time);
            value_refreshed = true;
            // new_data._get_time = _get_time;
        }

        // GPS management
        if (_get_gps.length() > 0)
        {
            value_refreshed = true;
            // Serial.println("GPS");
        }

        // Phone battery management
        if (_get_phone_battery.length() > 0)
        {
            // Serial.println("Phone Battery");
            data_exchange->setNavigationModeVariable(&dataExchange::NavigationModeVariables::actualPhoneBattery, _get_phone_battery);
            value_refreshed = true;
            // new_data._get_phone_battery = _get_phone_battery;
        }
        // Weather temp management
        if (_get_local_temperature.length() > 0)
        {
            // Serial.println("Weather temp");
            data_exchange->setNavigationModeVariable(&dataExchange::NavigationModeVariables::actualWeatherTemperature, _get_local_temperature);
            value_refreshed = true;
            // new_data._get_local_temperature = _get_local_temperature;
        }
        if (value_refreshed)
        {
            data_exchange->setNavigationModeVariable(&dataExchange::NavigationModeVariables::dataRefreshed, true);
        }
    }

    //  if the user is deconnected --> restart advertising
    //&& deviceDisconnected
    if (!deviceConnected && deviceDisconnected)
    {
        data_exchange->setNavigationModeVariable(&dataExchange::NavigationModeVariables::isConnected, false);

        Serial.println("AugMounted is deconnected"); // give the bluetooth stack the chance to get things ready
        BLEDevice::startAdvertising();               // restart advertising
        Serial.println("AugMounted restart advertising");
        deviceDisconnected = deviceConnected;

        // _device_connected = "no";
        _get_speed = "--";
        _get_altitude = "--";
        _get_time = "--:--";
        _get_gps = "--";
        _get_phone_battery = "0";
        _get_local_temperature = "--";
        // _get_vertical_speed = "--";
        // _get_wind_speed = "--";
    }

    // return new_data;
}