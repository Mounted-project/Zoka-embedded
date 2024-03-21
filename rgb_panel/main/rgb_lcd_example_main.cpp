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

#include "MCP23017.h"
#include "pins.hpp"

static const char *TAG = "example";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ 54000000
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL 1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_BK_LIGHT -1 // NOT SURE IF IT WORKS
#define EXAMPLE_PIN_NUM_HSYNC 46
#define EXAMPLE_PIN_NUM_VSYNC 3
#define EXAMPLE_PIN_NUM_DE 1 // NOT SURE IF IT WORKS
#define EXAMPLE_PIN_NUM_PCLK 9
#define EXAMPLE_PIN_NUM_DATA0 13  // B0
#define EXAMPLE_PIN_NUM_DATA1 14  // B1
#define EXAMPLE_PIN_NUM_DATA2 45  // B2
#define EXAMPLE_PIN_NUM_DATA3 47  // B3
#define EXAMPLE_PIN_NUM_DATA4 48  // B4
#define EXAMPLE_PIN_NUM_DATA5 39  // G0
#define EXAMPLE_PIN_NUM_DATA6 38  // G1
#define EXAMPLE_PIN_NUM_DATA7 12  // G2
#define EXAMPLE_PIN_NUM_DATA8 10  // G3
#define EXAMPLE_PIN_NUM_DATA9 11  // G4
#define EXAMPLE_PIN_NUM_DATA10 11 // G5
#define EXAMPLE_PIN_NUM_DATA11 1  // R0
#define EXAMPLE_PIN_NUM_DATA12 43 // R1
#define EXAMPLE_PIN_NUM_DATA13 42 // R2
#define EXAMPLE_PIN_NUM_DATA14 41 // R3
#define EXAMPLE_PIN_NUM_DATA15 40 // R4
#define EXAMPLE_PIN_NUM_DISP_EN 1 // NOT SURE IF IT WORKS

#define LCD_SPI_MISO 4
#define LCD_SPI_MOSI 5
#define LCD_SPI_SCLK 6
#define LCD_SPI_CS 7

#define LCD_XCLR 8

// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES 800 // 1024
#define EXAMPLE_LCD_V_RES 600 // 768

#if CONFIG_EXAMPLE_DOUBLE_FB
#define EXAMPLE_LCD_NUM_FB 2
#else
#define EXAMPLE_LCD_NUM_FB 1
#endif // CONFIG_EXAMPLE_DOUBLE_FB

#define EXAMPLE_LVGL_TICK_PERIOD_MS 2

// we use two semaphores to sync the VSYNC event and the LVGL task, to avoid potential tearing effect
#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
SemaphoreHandle_t sem_vsync_end;
SemaphoreHandle_t sem_gui_ready;
#endif
extern "C"
{
    extern void example_lvgl_demo_ui(lv_disp_t *disp);
}
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
extern "C"
{
    void app_main()
    {
        initArduino();
        ESP_LOGI(TAG, "Stared app_main");
        pinMode(LCD_SPI_CS, OUTPUT);
        pinMode(LCD_SPI_MOSI, OUTPUT);
        pinMode(LCD_SPI_MISO, INPUT);
        pinMode(LCD_SPI_SCLK, OUTPUT);
        digitalWrite(LCD_SPI_CS, HIGH); // disable device
        pinMode(LCD_XCLR, OUTPUT);

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
        MCP.digitalWrite(RGB_LCD_LSB, 0);
        MCP.digitalWrite(RGB_LED_R, 1);
        MCP.digitalWrite(RGB_LED_G, 1);
        MCP.digitalWrite(RGB_LED_B, 1);

        MCP.digitalWrite(LEDS_ENABLE, 0);

        static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
        static lv_disp_drv_t disp_drv;      // contains callback functions

#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
        ESP_LOGI(TAG, "Create semaphores");
        sem_vsync_end = xSemaphoreCreateBinary();
        assert(sem_vsync_end);
        sem_gui_ready = xSemaphoreCreateBinary();
        assert(sem_gui_ready);
#endif

#if EXAMPLE_PIN_NUM_BK_LIGHT >= 0
        ESP_LOGI(TAG, "Turn off LCD backlight");
        gpio_config_t bk_gpio_config = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = 1ULL << EXAMPLE_PIN_NUM_BK_LIGHT};
        ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
#endif
        ESP_LOGI(TAG, "Install RGB LCD panel driver");
        esp_lcd_panel_handle_t panel_handle = NULL;
        //  // calculate buffer size
        //     size_t fb_size = rgb_panel_config->timings.h_res * rgb_panel_config->timings.v_res * fb_bits_per_pixel / 8;
        //     size_t bb_size = rgb_panel_config->bounce_buffer_size_px * fb_bits_per_pixel / 8;
        //     if (bb_size) {
        //         // we want the bounce can always end in the second buffer
        //         ESP_GOTO_ON_FALSE(fb_size % (2 * bb_size) == 0, ESP_ERR_INVALID_ARG, err, TAG,
        //                           "fb size must be even multiple of bounce buffer size");
        //     }
        esp_lcd_rgb_panel_config_t panel_config;
        panel_config.data_width = 16;
        panel_config.psram_trans_align = 64;
        panel_config.num_fbs = EXAMPLE_LCD_NUM_FB;

#if CONFIG_EXAMPLE_USE_BOUNCE_BUFFER
        panel_config.bounce_buffer_size_px = 10 * EXAMPLE_LCD_H_RES;
#endif

        panel_config.clk_src = LCD_CLK_SRC_DEFAULT;
        panel_config.disp_gpio_num = EXAMPLE_PIN_NUM_DISP_EN;
        panel_config.pclk_gpio_num = EXAMPLE_PIN_NUM_PCLK;
        panel_config.vsync_gpio_num = EXAMPLE_PIN_NUM_VSYNC;
        panel_config.hsync_gpio_num = EXAMPLE_PIN_NUM_HSYNC;
        panel_config.de_gpio_num = EXAMPLE_PIN_NUM_DE;

        panel_config.data_gpio_nums[0] = EXAMPLE_PIN_NUM_DATA0;
        panel_config.data_gpio_nums[1] = EXAMPLE_PIN_NUM_DATA1;
        panel_config.data_gpio_nums[2] = EXAMPLE_PIN_NUM_DATA2;
        panel_config.data_gpio_nums[3] = EXAMPLE_PIN_NUM_DATA3;
        panel_config.data_gpio_nums[4] = EXAMPLE_PIN_NUM_DATA4;
        panel_config.data_gpio_nums[5] = EXAMPLE_PIN_NUM_DATA5;
        panel_config.data_gpio_nums[6] = EXAMPLE_PIN_NUM_DATA6;
        panel_config.data_gpio_nums[7] = EXAMPLE_PIN_NUM_DATA7;
        panel_config.data_gpio_nums[8] = EXAMPLE_PIN_NUM_DATA8;
        panel_config.data_gpio_nums[9] = EXAMPLE_PIN_NUM_DATA9;
        panel_config.data_gpio_nums[10] = EXAMPLE_PIN_NUM_DATA10;
        panel_config.data_gpio_nums[11] = EXAMPLE_PIN_NUM_DATA11;
        panel_config.data_gpio_nums[12] = EXAMPLE_PIN_NUM_DATA12;
        panel_config.data_gpio_nums[13] = EXAMPLE_PIN_NUM_DATA13;
        panel_config.data_gpio_nums[14] = EXAMPLE_PIN_NUM_DATA14;
        panel_config.data_gpio_nums[15] = EXAMPLE_PIN_NUM_DATA15;

        panel_config.timings.pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ;
        panel_config.timings.h_res = EXAMPLE_LCD_H_RES;
        panel_config.timings.v_res = EXAMPLE_LCD_V_RES;
        panel_config.timings.hsync_back_porch = 32;
        panel_config.timings.hsync_front_porch = 20;
        panel_config.timings.hsync_pulse_width = 1;
        panel_config.timings.vsync_back_porch = 32;
        panel_config.timings.vsync_front_porch = 19;
        panel_config.timings.vsync_pulse_width = 1;
        panel_config.timings.flags.pclk_active_neg = 1;

        // panel_config.flags.xd = 1;

        //     esp_lcd_rgb_panel_config_t panel_config = {
        //         .data_width = 16, // RGB565 in parallel mode, thus 16bit in width
        //         .psram_trans_align = 64,
        //         .num_fbs = EXAMPLE_LCD_NUM_FB,
        // #if CONFIG_EXAMPLE_USE_BOUNCE_BUFFER
        //         .bounce_buffer_size_px = 10 * EXAMPLE_LCD_H_RES,
        // #endif
        //         .clk_src = LCD_CLK_SRC_DEFAULT,
        //         .disp_gpio_num = EXAMPLE_PIN_NUM_DISP_EN,
        //         .pclk_gpio_num = EXAMPLE_PIN_NUM_PCLK,
        //         .vsync_gpio_num = EXAMPLE_PIN_NUM_VSYNC,
        //         .hsync_gpio_num = EXAMPLE_PIN_NUM_HSYNC,
        //         .de_gpio_num = EXAMPLE_PIN_NUM_DE,
        //         .data_gpio_nums = {
        //             EXAMPLE_PIN_NUM_DATA0,
        //             EXAMPLE_PIN_NUM_DATA1,
        //             EXAMPLE_PIN_NUM_DATA2,
        //             EXAMPLE_PIN_NUM_DATA3,
        //             EXAMPLE_PIN_NUM_DATA4,
        //             EXAMPLE_PIN_NUM_DATA5,
        //             EXAMPLE_PIN_NUM_DATA6,
        //             EXAMPLE_PIN_NUM_DATA7,
        //             EXAMPLE_PIN_NUM_DATA8,
        //             EXAMPLE_PIN_NUM_DATA9,
        //             EXAMPLE_PIN_NUM_DATA10,
        //             EXAMPLE_PIN_NUM_DATA11,
        //             EXAMPLE_PIN_NUM_DATA12,
        //             EXAMPLE_PIN_NUM_DATA13,
        //             EXAMPLE_PIN_NUM_DATA14,
        //             EXAMPLE_PIN_NUM_DATA15,
        //         },
        //         .timings = {
        //             .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        //             .h_res = EXAMPLE_LCD_H_RES,
        //             .v_res = EXAMPLE_LCD_V_RES,
        //             // The following parameters should refer to LCD spec
        //             .hsync_back_porch = 32,
        //             .hsync_front_porch = 20,
        //             .hsync_pulse_width = 1,
        //             .vsync_back_porch = 32,
        //             .vsync_front_porch = 19,
        //             .vsync_pulse_width = 1,
        //             .flags.pclk_active_neg = 1,
        //         },
        //         .flags.xd = 1, // allocate frame buffer in PSRAM
        //     };
        size_t fb_size = panel_config.timings.h_res * panel_config.timings.v_res * panel_config.data_width / 8;

        size_t bb_size = panel_config.bounce_buffer_size_px * panel_config.data_width / 8;
        ESP_LOGI(TAG, "fb_size: %zu", fb_size); // Use %zu for size_t
        ESP_LOGI(TAG, "bb_size: %zu", bb_size); // Use %zu for

        ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));

        ESP_LOGI(TAG, "Register event callbacks");
        esp_lcd_rgb_panel_event_callbacks_t cbs = {
            .on_vsync = example_on_vsync_event,
        };
        ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &cbs, &disp_drv));

        ESP_LOGI(TAG, "Initialize RGB LCD panel");
        ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
        ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

#if EXAMPLE_PIN_NUM_BK_LIGHT >= 0
        ESP_LOGI(TAG, "Turn on LCD backlight");
        gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);
#endif

        ESP_LOGI(TAG, "Initialize LVGL library");
        lv_init();
        void *buf1 = NULL;
        void *buf2 = NULL;
#if CONFIG_EXAMPLE_DOUBLE_FB
        ESP_LOGI(TAG, "Use frame buffers as LVGL draw buffers");
        ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 2, &buf1, &buf2));
        // initialize LVGL draw buffers
        lv_disp_draw_buf_init(&disp_buf, buf1, buf2, EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES);
#else
        ESP_LOGI(TAG, "Allocate separate LVGL draw buffers from PSRAM");
        buf1 = heap_caps_malloc(EXAMPLE_LCD_H_RES * 100 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
        assert(buf1);
        buf2 = heap_caps_malloc(EXAMPLE_LCD_H_RES * 100 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
        assert(buf2);
        // initialize LVGL draw buffers
        lv_disp_draw_buf_init(&disp_buf, buf1, buf2, EXAMPLE_LCD_H_RES * 100);
#endif // CONFIG_EXAMPLE_DOUBLE_FB

        ESP_LOGI(TAG, "Register display driver to LVGL");
        lv_disp_drv_init(&disp_drv);
        disp_drv.hor_res = EXAMPLE_LCD_H_RES;
        disp_drv.ver_res = EXAMPLE_LCD_V_RES;
        disp_drv.flush_cb = example_lvgl_flush_cb;
        disp_drv.draw_buf = &disp_buf;
        disp_drv.user_data = panel_handle;
#if CONFIG_EXAMPLE_DOUBLE_FB
        disp_drv.full_refresh = true; // the full_refresh mode can maintain the synchronization between the two frame buffers
#endif
        lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

        ESP_LOGI(TAG, "Install LVGL tick timer");
        // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
        const esp_timer_create_args_t lvgl_tick_timer_args = {
            .callback = &example_increase_lvgl_tick,
            .name = "lvgl_tick"};
        esp_timer_handle_t lvgl_tick_timer = NULL;
        ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));

        ESP_LOGI(TAG, "Display LVGL Scatter Chart");
        example_lvgl_demo_ui(disp);

        while (1)
        {
            // raise the task priority of LVGL and/or reduce the handler period can improve the performance
            vTaskDelay(pdMS_TO_TICKS(10));
            // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
            lv_timer_handler();
        }
    }
}
