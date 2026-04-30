#include "Led.h"

esp_err_t Led::init()
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << _pin);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

    esp_err_t err = gpio_config(&io_conf);
    if (err != ESP_OK)
    {
        ESP_LOGE(_TAG, "Failed to configure GPIO pin %d: %s", _pin, esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(_TAG, "GPIO pin %d configured successfully", _pin);
    return ESP_OK;
}

void Led::blinking()
{
    _state = !_state;
    gpio_set_level(_pin, _state == State::ON ? 1 : 0);
    ESP_LOGI(_TAG, "LED on pin %d is now %s", _pin, _state == State::ON ? "ON" : "OFF");
}