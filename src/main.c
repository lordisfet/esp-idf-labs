#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "BLINK";
const int LAMP_CONTROL_PIN = 4;

void app_main()
{
    ESP_LOGI(TAG, "app_main initial");

    gpio_reset_pin(LAMP_CONTROL_PIN);
    gpio_set_direction(LAMP_CONTROL_PIN, GPIO_MODE_OUTPUT);

    bool ledState = false;

    // ESP_LOGI(TAG, "start main cycle");

    while (1)
    {
        ledState = !ledState;
        gpio_set_level(LAMP_CONTROL_PIN, ledState);
        ESP_LOGI(TAG, "swirch led state to %d", ledState);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}