#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <time.h>
#include "esp_task_wdt.h"

#define DEBOUNCE_TIME 50000 // 50 ms

enum ButtonState
{
    IDLE,
    RELEASED,
    DEBOUNCE,
    PRESSED,
};

class Button
{
    private:
    const char* _TAG;
    gpio_num_t _pin;
    unsigned long long _pinMask;
    int _debounceTime; // 50 ms

    bool _state; // true - pressed, false - released
    enum ButtonState _internalState;

    ulong lastLevelSwitchTime;
    int lastlevel;

    public:
    Button(const char* TAG, gpio_num_t pin, int debounceTime = DEBOUNCE_TIME, bool state = false)
    {
        _TAG = TAG;
        _pin = pin;
        _pinMask = 1ULL << _pin;
        _debounceTime = debounceTime;
        _state = state;
        _internalState = IDLE;
        lastLevelSwitchTime = 0;
        lastlevel = gpio_get_level(_pin);

        const gpio_config_t configButton =
        {
            .pin_bit_mask = _pinMask,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE};

        ESP_ERROR_CHECK(gpio_config(&configButton));
    }

    bool isPressed()
    {
        return _state;
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
            _state = !_state;
            lastlevel = currentLevel;
            _internalState = IDLE;
            ESP_LOGI(_TAG, "Button state: %s", _state ? "active" : "inactive");
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