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

    etl::delegate<void()> _onClick;

    ButtonState _internalState;
    ulong _lastLevelSwitchTime;
    int _lastlevel;

    public:
    void setOnClick(etl::delegate<void()> cb) {_onClick = cb;}

    Button(const char* TAG, 
           gpio_num_t pin, 
           int debounceTime = DEBOUNCE_TIME)
        : _TAG(TAG),
          _pin(pin),
          _pinMask(1ULL << pin),
          _debounceTime(debounceTime),
          _internalState(IDLE),
          _lastLevelSwitchTime(0),
          _lastlevel(0)
    {
        // Читаем фактическое состояние пина
        _lastlevel = gpio_get_level(_pin);

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

    esp_err_t update()
    {
        int currentLevel = gpio_get_level(_pin);
        ulong currentTime = esp_timer_get_time();

        switch (_internalState)
        {
        case IDLE:
            if (currentLevel != _lastlevel)
            {
                _lastLevelSwitchTime = currentTime;
                _internalState = DEBOUNCE;
            }
            break;
        case DEBOUNCE:
            if (currentLevel == _lastlevel)
            {
                _internalState = IDLE;
                _lastLevelSwitchTime = currentTime;
            }
            if (currentTime - _lastLevelSwitchTime >= _debounceTime && currentLevel != _lastlevel)
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
            if (_onClick.is_valid()) 
            {
                _onClick();
            }
            _lastlevel = currentLevel;
            _internalState = IDLE;
            ESP_LOGI(_TAG, "Button clicked");
            break;
        case RELEASED:
            _lastlevel = currentLevel;
            _internalState = IDLE;
            break;
        default:
            break;
        }

        return ESP_OK;
    }
};
