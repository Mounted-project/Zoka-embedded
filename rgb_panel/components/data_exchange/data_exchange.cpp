#include "data_exchange.hpp"

dataExchange::dataExchange()
{
    dataExchangeVariables = xSemaphoreCreateMutex();
}

dataExchange::~dataExchange()
{
    if (dataExchangeVariables != nullptr)
    {
        vSemaphoreDelete(dataExchangeVariables);
    }
}
