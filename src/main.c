#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG_LAMP = "BLINK";
const int LAMP_CONTROL_PIN = 4;
const unsigned long timeON = 1000000;
const unsigned long timeOFF = 5000000;

bool lampState = false;
esp_timer_handle_t lamp_timer_hendler;

void switchLampState()
{
    lampState = !lampState;
    gpio_set_level(LAMP_CONTROL_PIN, lampState);

    if (lampState)
    {
        ESP_ERROR_CHECK(esp_timer_start_once(lamp_timer_hendler, timeON));
    }
    else
    {
        ESP_ERROR_CHECK(esp_timer_start_once(lamp_timer_hendler, timeOFF));
    }

    ESP_LOGI(TAG_LAMP, "Lamp state changed to %d", lampState);
}

void app_main()
{
    gpio_reset_pin(LAMP_CONTROL_PIN);
    gpio_set_direction(LAMP_CONTROL_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LAMP_CONTROL_PIN, lampState);

    const esp_timer_create_args_t lamp_timer_args = {
        .callback = switchLampState,
        .name = "lamp_timer",
    };

    ESP_ERROR_CHECK(esp_timer_create(&lamp_timer_args, &lamp_timer_hendler));

    ESP_ERROR_CHECK(esp_timer_start_once(lamp_timer_hendler, timeOFF));
}