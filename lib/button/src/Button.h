#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <time.h>
#include "esp_task_wdt.h"
#include "etl/delegate.h"

#define DEBOUNCE_TIME 10000 // 50 ms

typedef enum {
    IDLE,
    RELEASED,
    DEBOUNCE,
    PRESSED,
} ButtonState;

class Button
{
    private:
    const char* _TAG;
    gpio_num_t _pin;
    unsigned long long _pinMask;
    int _debounceTime;

    bool _isToggled;
    etl::delegate<void()> _onToggleOn;
    etl::delegate<void()> _onToggleOff;

    ButtonState _internalState;
    ulong lastLevelSwitchTime;
    int lastlevel;

    public:
    void toggleOff() {_isToggled = false;}

    void setOnToggleOn(etl::delegate<void()> cb) {_onToggleOn = cb;}
    void setOnToggleOff(etl::delegate<void()> cb) {_onToggleOff = cb;}

    Button(const char* TAG, 
           gpio_num_t pin, 
           int debounceTime = DEBOUNCE_TIME)
        : _TAG(TAG),
          _pin(pin),
          _pinMask(1ULL << pin),
          _debounceTime(debounceTime),
          _isToggled(false),
          _internalState(IDLE),
          lastLevelSwitchTime(0),
          lastlevel(0)
    {
        // Читаем фактическое состояние пина
        lastlevel = gpio_get_level(_pin);

        const gpio_config_t configButton =
        {
            .pin_bit_mask = _pinMask, // Используем уже посчитанную маску
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE
        };

        ESP_ERROR_CHECK(gpio_config(&configButton));
    }

    bool isPressed()
    {
        return _isToggled;
    }

    esp_err_t update()
    {
        int currentLevel = gpio_get_level(_pin);
        ulong currentTime = esp_timer_get_time();

        switch (_internalState)
        {
        case IDLE:
            if (currentLevel != lastlevel)
            {
                lastLevelSwitchTime = currentTime;
                _internalState = DEBOUNCE;
            }
            break;
        case DEBOUNCE:
            if (currentLevel == lastlevel)
            {
                _internalState = IDLE;
                lastLevelSwitchTime = currentTime;
            }
            if (currentTime - lastLevelSwitchTime >= _debounceTime && currentLevel != lastlevel)
            {
                if (currentLevel == 0)
                {
                    _internalState = PRESSED;
                }
                if (currentLevel == 1)
                {
                    _internalState = RELEASED;
                }
            }
            break;
        case PRESSED:
            _isToggled = !_isToggled;
            lastlevel = currentLevel;
            _internalState = IDLE;
            ESP_LOGI(_TAG, "Button state: %s", _isToggled ? "active" : "inactive");

            if (_isToggled)
            {
                ESP_LOGI(_TAG, "IS TOGGLED ON");
                if (_onToggleOn.is_valid()) {ESP_LOGI(_TAG, "DELEGATE IS VALID"); _onToggleOn();}
            }
            else {
                ESP_LOGI(_TAG, "IS TOGGLED OFF");
                if (_onToggleOff.is_valid()) {ESP_LOGI(_TAG, "DELEGATE IS VALID"); _onToggleOff();}
            }
            break;
        case RELEASED:
            lastlevel = currentLevel;
            _internalState = IDLE;
            break;
        default:
            break;
        }

        return ESP_OK;
    }
};
