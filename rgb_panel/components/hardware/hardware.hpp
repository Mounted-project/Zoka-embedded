#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include "pins.hpp"
#include "data_exchange.hpp"

struct HardwareTaskParameters
{
    TwoWire *Wire;
    dataExchange *data_exchange;
};
void hardwareTask(void *pvParameters);

#endif // HARDWARE_HPP