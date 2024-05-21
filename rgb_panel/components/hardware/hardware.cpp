#include "MAX17048.h"
// #include "data_exchange.hpp"
#include "hardware.hpp"

#include "Arduino.h"

static const char *TAG = "Hardware Task";
void hardwareTask(void *pvParameters)
{
    ESP_LOGI(TAG, "Task is running");
    HardwareTaskParameters *taskParams = static_cast<HardwareTaskParameters *>(pvParameters);

    // Vérification de la validité des paramètres
    if (taskParams == nullptr || taskParams->Wire == nullptr || taskParams->data_exchange == nullptr)
    {
        ESP_LOGE(TAG, "Invalid task parameters!");
        vTaskDelete(nullptr); // Supprimer la tâche en cas de paramètres invalides
        return;
    }

    TwoWire *wire = taskParams->Wire;
    dataExchange *data_exchange = taskParams->data_exchange;
    MAX17048 max17048 = MAX17048(MAX17048_ADDR, *wire);
    while (1)
    {
        data_exchange->setNavigationModeVariable(&dataExchange::NavigationModeVariables::actualZokaBattery, std::to_string(max17048.percent()));
        ESP_LOGI(TAG, "Battery Percentage : %d", max17048.percent());
        ESP_LOGI(TAG, "Battery voltage : %f", max17048.voltage());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}