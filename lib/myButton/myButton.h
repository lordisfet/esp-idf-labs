#pragma once

#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

enum ButtonState
{
    IDLE,
    RELEASED,
    DEBOUNCE,
    PRESSED,
};

enum ButtonActiveLevel
{
    ACTIVE_LOW = 0,
    ACTIVE_HIGH = 1,
};

enum ButtonPullMode
{
    PULL_UP = 0,
    PULL_DOWN = 1,
    NO_PULL = 2,
};

class MyButton
{
private:
    static constexpr uint16_t _DEBOUNCE_TIME_US = 50000;

    gpio_num_t _pin;

    const char *_TAG_BUTTON;
    const enum ButtonActiveLevel _ACTIVE_LEVEL;
    const enum ButtonPullMode _PULL_MODE;

    void (*_onPress)(void);

    enum ButtonState _internallState;
    int _lastlevel;
    uint64_t _lastlevelSwitchTime;

public:
    MyButton(gpio_num_t pin, const char *tag, const enum ButtonActiveLevel ACTIVE_LEVEL = ACTIVE_LOW, const enum ButtonPullMode PULL_MODE = PULL_UP,
             void (*onPress)() = nullptr)
        : _pin(pin), _TAG_BUTTON(tag), _ACTIVE_LEVEL(ACTIVE_LEVEL), _PULL_MODE(PULL_MODE), _onPress(onPress)
    {
        _internallState = IDLE;
        _lastlevelSwitchTime = 0;
    };

    esp_err_t init();
    void button_update();
};
