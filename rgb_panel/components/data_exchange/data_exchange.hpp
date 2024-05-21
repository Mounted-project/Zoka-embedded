#ifndef DATA_EXCHANGE_H
#define DATA_EXCHANGE_H

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// Définition du type pour les chaînes de caractères protégées
typedef std::string protected_string_t;
class dataExchange
{

    typedef protected_string_t Time_t;
    typedef protected_string_t PhoneRemainingBatteryPrct_t;
    typedef protected_string_t ZokaRemainingBatteryPrct_t;
    typedef protected_string_t Speed_t;
    typedef protected_string_t SpeedUnit_t;
    typedef protected_string_t SpeedLimitation_t;
    typedef protected_string_t TimeOfArrival_t;
    typedef protected_string_t TimeRemaining_t;
    typedef protected_string_t DistanceRemaining_t;
    typedef protected_string_t Altitude_t;
    typedef protected_string_t ActualTurnByTurnDirection_t;
    typedef protected_string_t ActualTurnByTurnDistance_t;
    typedef protected_string_t WeatherTemperature_t;
    typedef bool DataRefreshed;
    typedef bool IsConnected;

public:
    struct NavigationModeVariables
    {
        Time_t actualTime = "00:00";
        PhoneRemainingBatteryPrct_t actualPhoneBattery = "0";
        ZokaRemainingBatteryPrct_t actualZokaBattery = "0";
        Speed_t actualSpeed = "00";
        SpeedUnit_t actualSpeedunit = "Km/h";
        SpeedLimitation_t actualSpeedLimitation = "00";
        TimeOfArrival_t actualTimeOfArrival = "00:00";
        TimeRemaining_t actualTimeRemaining = "0:00";
        DistanceRemaining_t actualDistanceRemaining = "0";
        ActualTurnByTurnDirection_t actualTurnByTurnDirection = "NONE";
        ActualTurnByTurnDistance_t actualTurnByTurnDistance = "0m";
        Altitude_t actualAltitude = "0m";
        WeatherTemperature_t actualWeatherTemperature = "0°C";
        DataRefreshed dataRefreshed = false;
        IsConnected isConnected = false;
    };
    dataExchange();
    ~dataExchange();

    template <typename NavigationModeMember>
    NavigationModeMember &getNavigationModeVariable(NavigationModeMember NavigationModeVariables::*member);

    template <typename NavigationModeMember>
    void setNavigationModeVariable(NavigationModeMember NavigationModeVariables::*member, const NavigationModeMember &value);

private:
    SemaphoreHandle_t dataExchangeVariables;

    // Définition des types pour chaque membre de la structure

    // Structure pour les variables de mode de navigation

    NavigationModeVariables navVars;
};
template <typename NavigationModeMember>
NavigationModeMember &dataExchange::getNavigationModeVariable(NavigationModeMember NavigationModeVariables::*member)
{
    xSemaphoreTake(dataExchangeVariables, portMAX_DELAY);
    NavigationModeMember &value = navVars.*member;
    xSemaphoreGive(dataExchangeVariables);
    return value;
}

template <typename NavigationModeMember>
void dataExchange::setNavigationModeVariable(NavigationModeMember NavigationModeVariables::*member, const NavigationModeMember &value)
{
    xSemaphoreTake(dataExchangeVariables, portMAX_DELAY);
    navVars.*member = value;
    xSemaphoreGive(dataExchangeVariables);
}

// Explicit template instantiations for all member types
template protected_string_t &dataExchange::getNavigationModeVariable(protected_string_t NavigationModeVariables::*member);
template void dataExchange::setNavigationModeVariable(protected_string_t NavigationModeVariables::*member, const protected_string_t &value);
#endif // DATA_EXCHANGE_H
