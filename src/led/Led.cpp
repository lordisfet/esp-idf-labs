#include "Led.h"

#define MULTIPLYER_FOR_MS_TO_US 1000

void Led::blinkOn(ulong interval_ms)
{
    _state = State::ON;
    gpio_set_level(_pin, 1);
    ESP_LOGI(_TAG, "LED on pin %d turned ON", _pin);
    esp_timer_start_once(_blinkTimer, interval_ms * MULTIPLYER_FOR_MS_TO_US);
};

// Переніс вимикання на переривання для того, щоб попрактуватися з апарптним таймером
void Led::blinkOff(void *arg)
{
    Led *led = static_cast<Led *>(arg);
    if (led->_state == State::ON)
    {
        led->_state = State::OFF;
        gpio_set_level(led->_pin, 0);
        // Це краще винести в окреимий файл, да і взагалі мабуть краще використовувати бібліотеку логування, але для простоти прикладу так залишимо
        ESP_LOGI(led->_TAG, "LED on pin %d turned OFF", led->_pin);
    }
}

esp_err_t Led::init()
{
    gpio_config_t conf = {};
    conf.intr_type = GPIO_INTR_DISABLE;
    conf.mode = GPIO_MODE_OUTPUT;
    conf.pin_bit_mask = (1ULL << _pin);
    conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    conf.pull_up_en = GPIO_PULLUP_DISABLE;

    const esp_timer_create_args_t timerArgs = {
        .callback = blinkOff,
        .arg = this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "LED_BLINK_TIMER"};

    esp_err_t err = gpio_config(&conf);
    if (err != ESP_OK)
    {
        ESP_LOGE(_TAG, "Failed to configure GPIO pin %d: %s", _pin, esp_err_to_name(err));
        return err;
    }

    err = esp_timer_create(&timerArgs, &_blinkTimer);
    if (err != ESP_OK)
    {
        ESP_LOGE(_TAG, "Failed to create blink timer: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(_TAG, "GPIO pin %d configured successfully", _pin);
    return ESP_OK;
}