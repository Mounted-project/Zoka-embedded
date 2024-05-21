/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "Arduino.h"
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"

#include "ui/ui.h"
// #include "ui/components/ui_comp.h"
#include "ui/components/ui_comp_statusbar.h"

#include "MCP23017.h"
#include "ECX334.hpp"
#include "data_exchange.hpp"
#include "hardware.hpp"
#include "BLE_Hud_Service.hpp"

// #include "pins.hpp"
#include "lcd_parameters.h"

static const char *TAG = "Zoka Main Task";

static SemaphoreHandle_t lvgl_mux = NULL;
// we use two semaphores to sync the VSYNC event and the LVGL task, to avoid potential tearing effect
#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
SemaphoreHandle_t sem_vsync_end;
SemaphoreHandle_t sem_gui_ready;
#endif

static bool example_on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *event_data, void *user_data)
{
    BaseType_t high_task_awoken = pdFALSE;
#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
    if (xSemaphoreTakeFromISR(sem_gui_ready, &high_task_awoken) == pdTRUE)
    {
        xSemaphoreGiveFromISR(sem_vsync_end, &high_task_awoken);
    }
#endif
    return high_task_awoken == pdTRUE;
}

static void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)drv->user_data;
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
    xSemaphoreGive(sem_gui_ready);
    xSemaphoreTake(sem_vsync_end, portMAX_DELAY);
#endif
    // pass the draw buffer to the driver
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
    lv_disp_flush_ready(drv);
}

static void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

bool example_lvgl_lock(int timeout_ms)
{
    // Convert timeout in milliseconds to FreeRTOS ticks
    // If `timeout_ms` is set to -1, the program will block until the condition is met
    const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}

void example_lvgl_unlock(void)
{
    xSemaphoreGiveRecursive(lvgl_mux);
}

static void example_lvgl_port_task(void *arg)
{
    ESP_LOGI(TAG, "Starting LVGL task");
    uint32_t task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
    ui_init();

    dataExchange *data_exchange = static_cast<dataExchange *>(arg);
    while (1)
    {
        ESP_LOGI("lvgl", "time :%s", data_exchange->getNavigationModeVariable(&dataExchange::NavigationModeVariables::actualTime).c_str());
        // Lock the mutex due to the LVGL APIs are not thread-safe

        if (example_lvgl_lock(-1))
        {
            if (data_exchange->getNavigationModeVariable(&dataExchange::NavigationModeVariables::dataRefreshed))
            {
                lv_bar_set_value(ui_ZokaBatteryBar1, std::stoi(data_exchange->getNavigationModeVariable(&dataExchange::NavigationModeVariables::actualZokaBattery)), LV_ANIM_OFF);
                lv_bar_set_value(ui_PhoneBatteryBar1, std::stoi(data_exchange->getNavigationModeVariable(&dataExchange::NavigationModeVariables::actualPhoneBattery)), LV_ANIM_OFF);
                lv_label_set_text(ui_PhoneBatteryPercent1, data_exchange->getNavigationModeVariable(&dataExchange::NavigationModeVariables::actualPhoneBattery).c_str());
                lv_label_set_text(ui_ZokaBatteryPercent1, data_exchange->getNavigationModeVariable(&dataExchange::NavigationModeVariables::actualZokaBattery).c_str());
                lv_label_set_text(ui_Time1, data_exchange->getNavigationModeVariable(&dataExchange::NavigationModeVariables::actualTime).c_str());

                lv_arc_set_value(ui_ArcSpeed1, std::stoi(data_exchange->getNavigationModeVariable(&dataExchange::NavigationModeVariables::actualSpeed)));
                lv_label_set_text(ui_SpeedLabel1, data_exchange->getNavigationModeVariable(&dataExchange::NavigationModeVariables::actualSpeed).c_str());

                lv_label_set_text(ui_SpeedLimitLabel1, "30");
                lv_label_set_text(ui_RemainingDistance1, "0");
                lv_label_set_text(ui_TimeRemaining1, "00:00");
                lv_label_set_text(ui_TimeOfArival1, "00:00");

                if (data_exchange->getNavigationModeVariable(&dataExchange::NavigationModeVariables::isConnected))
                {
                    _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 500, &ui_MainScreen_screen_init);
                }
                else
                {
                    _ui_screen_change(&ui_ConnectScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 500, &ui_ConnectScreen_screen_init);
                }
                data_exchange->setNavigationModeVariable(&dataExchange::NavigationModeVariables::dataRefreshed, false);
            }
            task_delay_ms = lv_timer_handler();
            // Release the mutex
            example_lvgl_unlock();
        }
        if (task_delay_ms > EXAMPLE_LVGL_TASK_MAX_DELAY_MS)
        {
            task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
        }
        else if (task_delay_ms < EXAMPLE_LVGL_TASK_MIN_DELAY_MS)
        {
            task_delay_ms = EXAMPLE_LVGL_TASK_MIN_DELAY_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

extern "C"
{
    void app_main()
    {
        dataExchange data_exchange;
        BLE_Hud_Service ble_com_hud = BLE_Hud_Service(&data_exchange);
        initArduino();
        ESP_LOGI(TAG, "Stared app_main");
        initLcdSpi();
        // pinMode(ESP_POWER_EN, OUTPUT);
        gpio_set_level(ESP_POWER_EN, true); // setting the state before setting the gpio as output prevent gpio glitch, otherwise the ESP32 will suicide.
        gpio_set_direction(ESP_POWER_EN, GPIO_MODE_OUTPUT);
        pinMode(ESP_POWER_EN, OUTPUT);
        // gpio_set_drive_capability(ESP_POWER_EN, GPIO_DRIVE_CAP_3);
        // digitalWrite(ESP_POWER_EN, HIGH);
        gpio_set_level(ESP_POWER_EN, true);
        Wire.begin(I2C_SDA, I2C_SCL, 100000);

        MCP23017 MCP = MCP23017(MCP23017_ADDR, Wire);
        MCP.init();
        MCP.pinMode(BQ_CHARGE_STATUS, 1);
        MCP.pinMode(BQ_PGOOD, 1);

        MCP.pinMode(RGB_LED_R, 0);
        MCP.pinMode(RGB_LED_G, 0);
        MCP.pinMode(RGB_LED_B, 0);
        MCP.pinMode(LEDS_ENABLE, 0);

        MCP.pinMode(ENABLE_1_8V, 0);
        MCP.pinMode(ENABLE_10V, 0);
        MCP.pinMode(RGB_LCD_LSB, 0);

        MCP.pinMode(BUTTON_ENCODER, 1);
        MCP.pinMode(BUTTON1, 1);
        MCP.pinMode(BUTTON2, 1);

        MCP.digitalWrite(ENABLE_1_8V, 0);
        MCP.digitalWrite(ENABLE_10V, 0);
        MCP.digitalWrite(RGB_LCD_LSB, 1);
        MCP.digitalWrite(RGB_LED_R, 1);
        MCP.digitalWrite(RGB_LED_G, 1);
        MCP.digitalWrite(RGB_LED_B, 1);

        MCP.digitalWrite(LEDS_ENABLE, 0);
        ESP_LOGI(TAG, "Stared app_main");

        static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
        static lv_disp_drv_t disp_drv;      // contains callback functions
#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
        ESP_LOGI(TAG, "Create semaphores");
        sem_vsync_end = xSemaphoreCreateBinary();
        assert(sem_vsync_end);
        sem_gui_ready = xSemaphoreCreateBinary();
        assert(sem_gui_ready);
#endif

        ESP_LOGI(TAG, "Install RGB LCD panel driver");
        esp_lcd_panel_handle_t panel_handle = NULL;
        esp_lcd_rgb_panel_config_t panel_config;
        panel_config.data_width = 16;
        panel_config.psram_trans_align = 64;
        panel_config.sram_trans_align = 64;
        panel_config.num_fbs = EXAMPLE_LCD_NUM_FB;
        panel_config.flags.no_fb = 0;

#if CONFIG_EXAMPLE_USE_BOUNCE_BUFFER
        panel_config.bounce_buffer_size_px = BUFFER_SIZE;
#else
        panel_config.bounce_buffer_size_px = 0; // JULIEN
#endif

        panel_config.clk_src = LCD_CLK_SRC_DEFAULT;
        panel_config.disp_gpio_num = -1;
        panel_config.pclk_gpio_num = LCD_PCLK;
        panel_config.vsync_gpio_num = LCD_VSYNC;
        panel_config.hsync_gpio_num = LCD_HSYNC;
        panel_config.de_gpio_num = -1;
        panel_config.bits_per_pixel = 0;

        panel_config.data_gpio_nums[0] = LCD_B0;
        panel_config.data_gpio_nums[1] = LCD_B1;
        panel_config.data_gpio_nums[2] = LCD_B2;
        panel_config.data_gpio_nums[3] = LCD_B3;
        panel_config.data_gpio_nums[4] = LCD_B4;
        panel_config.data_gpio_nums[5] = LCD_G0;
        panel_config.data_gpio_nums[6] = LCD_G1;
        panel_config.data_gpio_nums[7] = LCD_G2;
        panel_config.data_gpio_nums[8] = LCD_G3;
        panel_config.data_gpio_nums[9] = LCD_G4;
        panel_config.data_gpio_nums[10] = LCD_G5;
        panel_config.data_gpio_nums[11] = LCD_R0;
        panel_config.data_gpio_nums[12] = LCD_R1;
        panel_config.data_gpio_nums[13] = LCD_R2;
        panel_config.data_gpio_nums[14] = LCD_R3;
        panel_config.data_gpio_nums[15] = LCD_R4;

        panel_config.timings.pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ;
        panel_config.timings.h_res = EXAMPLE_LCD_H_RES;
        panel_config.timings.v_res = EXAMPLE_LCD_V_RES;
        panel_config.timings.hsync_back_porch = 32;
        panel_config.timings.hsync_front_porch = 20;
        panel_config.timings.hsync_pulse_width = 16;
        panel_config.timings.vsync_back_porch = 32;
        panel_config.timings.vsync_front_porch = 19;
        panel_config.timings.vsync_pulse_width = 6;
        panel_config.timings.flags.pclk_active_neg = true;
        panel_config.flags.fb_in_psram = true;

        ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));

        ESP_LOGI(TAG, "Register event callbacks");
        esp_lcd_rgb_panel_event_callbacks_t cbs = {
            .on_vsync = example_on_vsync_event,
        };
        ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &cbs, &disp_drv));

        ESP_LOGI(TAG, "Initialize RGB LCD panel");
        ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
        ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
        ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));
        ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, true));

        ESP_LOGI(TAG, "Initialize LVGL library");
        lv_init();
        void *buf1 = NULL;
        void *buf2 = NULL;
#if CONFIG_EXAMPLE_DOUBLE_FB
        ESP_LOGI(TAG, "Use double frame buffers as LVGL draw buffers");
        ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 2, &buf1, &buf2));
        // initialize LVGL draw buffers
        lv_disp_draw_buf_init(&disp_buf, buf1, buf2, BUFFER_SIZE / sizeof(lv_color_t));
#else
        ESP_LOGI(TAG, "Allocate separate LVGL draw buffers from PSRAM");
        buf1 = heap_caps_malloc(BUFFER_SIZE, MALLOC_CAP_SPIRAM);
        // buf1 = heap_caps_malloc(BUFFER_SIZE, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
        assert(buf1);
        // initialize LVGL draw buffers
        lv_disp_draw_buf_init(&disp_buf, buf1, buf2, BUFFER_SIZE / sizeof(lv_color_t));

#endif // CONFIG_EXAMPLE_DOUBLE_FB

        ESP_LOGI(TAG, "Register display driver to LVGL");
        lv_disp_drv_init(&disp_drv);

        disp_drv.hor_res = EXAMPLE_LCD_H_RES;
        disp_drv.ver_res = EXAMPLE_LCD_V_RES;
        disp_drv.flush_cb = example_lvgl_flush_cb;
        disp_drv.draw_buf = &disp_buf;
        disp_drv.user_data = panel_handle;
        disp_drv.rotated = 1;
#if CONFIG_EXAMPLE_DOUBLE_FB
        disp_drv.full_refresh = true; // the full_refresh mode can maintain the synchronization between the two frame buffers
#endif
        // lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
        lv_disp_drv_register(&disp_drv);
        ESP_LOGI(TAG, "Install LVGL tick timer");
        // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
        const esp_timer_create_args_t lvgl_tick_timer_args = {
            .callback = &example_increase_lvgl_tick,
            .name = "lvgl_tick"};
        esp_timer_handle_t lvgl_tick_timer = NULL;
        ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));

        lvgl_mux = xSemaphoreCreateRecursiveMutex();
        assert(lvgl_mux);
        // Lock the mutex due to the LVGL APIs are not thread-safe
        ESP_LOGI(TAG, "Create LVGL task");
        xTaskCreate(example_lvgl_port_task, "LVGL", EXAMPLE_LVGL_TASK_STACK_SIZE, &data_exchange, EXAMPLE_LVGL_TASK_PRIORITY, NULL);
        pinMode(LCD_XCLR, OUTPUT);
        ESP_LOGI("LCD : ", "STARTING POWER ON SEQUENCE\n");
        // ESP_LOGI("LCD : ", "Init step 1\n");
        digitalWrite(LCD_XCLR, LOW);
        MCP.digitalWrite(ENABLE_1_8V, 1);
        delay(1);
        MCP.digitalWrite(ENABLE_10V, 1);
        delay(2);
        // ESP_LOGI("LCD : ", "Init step 2\n");
        digitalWrite(LCD_XCLR, HIGH);
        // delay(2);
        // ESP_LOGI("LCD : ", "Init step 3\n");

        // byte read_data;
        delay(11);
        writeSPIRegister(0X03, 0x80); // TODO WAS A0
        // read_data = readSPIRegister(0X03);
        // ESP_LOGI(TAG, "Read data: 0x%02X", read_data);
        // delay(1);
        writeSPIRegister(0X04, 0x5F);
        // read_data = readSPIRegister(0X04);
        // ESP_LOGI(TAG, "Read data: 0x%02X", read_data);
        // delay(1);
        writeSPIRegister(0X53, 0x02);
        // delay(1);
        writeSPIRegister(0X5B, 0x4F);
        // delay(1);
        writeSPIRegister(0X5C, 0x4D);
        // ESP_LOGI("LCD : ", "Init step 4\n");
        // delay(1);
        writeSPIRegister(0X00, 0x0D);
        // ESP_LOGI("LCD : ", "Init step 5\n");
        delay(1);
        writeSPIRegister(0X04, 0x1F);
        // ESP_LOGI("LCD : ", "Init step 6\n");
        delay(4);
        writeSPIRegister(0X5B, 0x04);
        writeSPIRegister(0X5C, 0x04);
        // ESP_LOGI("LCD : ", "Init step 7\n");
        delay(1);
        writeSPIRegister(0X5B, 0x00);
        writeSPIRegister(0X5C, 0x00);
        // ESP_LOGI("LCD : ", "Init step 8\n");
        writeSPIRegister(0X53, 0x00);
        // ESP_LOGI("LCD : ", "Init step 9\n");
        delay(1);
        writeSPIRegister(0X03, 0x00); // Was 0x20
        ESP_LOGI("LCD : ", "POWER ON SEQUENCE DONE\n");
        uint8_t data0X09 = readSPIRegister(0x09);
        ESP_LOGI(TAG, "Read data: 0x%02X", data0X09);

        writeSPIRegister(0X18, 0xFF);
        writeSPIRegister(0X19, 0x7F);
        writeSPIRegister(0X1A, 0x7F);
        writeSPIRegister(0X1B, 0x7F);

        writeSPIRegister(0X05, 0x0F);
        writeSPIRegister(0X1D, 0xFF);

        uint8_t read_data = readSPIRegister(0X18);
        ESP_LOGI(TAG, "Brighness: 0x%02X", read_data);
        read_data = readSPIRegister(0X05);
        ESP_LOGI(TAG, "Luminance: 0x%02X", read_data);
        read_data = readSPIRegister(0X08);
        ESP_LOGI(TAG, "Check lum and caldac = 0x00: 0x%02X", read_data);
        // TaskHandle_t xTask = xTaskGetCurrentTaskHandle();
        // vTaskPrioritySet(xTask, 24);

        long buttonPressTime = 0;
        bool buttonPressed = false;
        bool switchh = false;

        for (uint8_t i = 0; i <= 100; i++)
        {
            lv_bar_set_value(ui_BootBar, i, LV_ANIM_OFF);
            vTaskDelay(10);
        }
        _ui_screen_change(&ui_ConnectScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 500, &ui_ConnectScreen_screen_init);

        HardwareTaskParameters hardwareTaskParameters{&Wire, &data_exchange};
        xTaskCreate(hardwareTask, "batteryTask", 8192, &hardwareTaskParameters, 5, NULL);
        ble_com_hud.initserviceUUID();
        ble_com_hud.createService();

        while (1)
        {
            vTaskDelay(1);
            // ESP_LOGI("dataExchange", "actualSpeed : %s", data_exchange.getNavigationModeVariable(&dataExchange::NavigationModeVariables::actualTime).c_str());

            ble_com_hud.hudconnected2(std::stoi(data_exchange.getNavigationModeVariable(&dataExchange::NavigationModeVariables::actualZokaBattery)));
            if (MCP.digitalRead(BUTTON_ENCODER) == false)
            {
                if (!buttonPressed)
                {                               // Si c'est le début de l'appui
                    buttonPressTime = millis(); // Enregistre le moment de l'appui
                    buttonPressed = true;       // Marque le début de l'appui
                }
                else
                {
                    if (millis() - buttonPressTime >= 3000)
                    { // Si le bouton est pressé depuis 3 secondes
                        ESP_LOGI(TAG, "Button has been pressed for more than 3 secs");
                        MCP.digitalWrite(ENABLE_10V, LOW);
                        delay(5);
                        MCP.digitalWrite(ENABLE_1_8V, LOW);
                        delay(5);
                        gpio_set_level(ESP_POWER_EN, false);
                        gpio_set_direction(ESP_POWER_EN, GPIO_MODE_INPUT);
                        digitalWrite(ESP_POWER_EN, LOW);
                        buttonPressed = false;
                    }
                    // else if (millis() - buttonPressTime >= 300 && millis() - buttonPressTime <= 300)
                    // {
                    //     if (switchh)
                    //     {
                    //         _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 500, &ui_MainScreen_screen_init);
                    //         switchh = false;
                    //     }
                    //     else
                    //     {
                    //         _ui_screen_change(&ui_ConnectScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 500, &ui_ConnectScreen_screen_init);
                    //         switchh = true;
                    //     }
                    //     buttonPressed = false;
                    // }
                }
            }
            else
            {
                buttonPressed = false;
            }
        }
    }
}
