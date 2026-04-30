#include "esp_timer.h"

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

void Led::turnOn()
{
    _state = State::ON;
    gpio_set_level(_pin, 1);
    ESP_LOGI(_TAG, "LED on pin %d turned ON", _pin);
};

void Led::turnOff()
{
    _state = State::OFF;
    gpio_set_level(_pin, 0);
    ESP_LOGI(_TAG, "LED on pin %d turned OFF", _pin);
};

void Led::blink(ulong interval_ms)
{
    turnOn();
}