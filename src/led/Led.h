#pragma once

#include "stdio.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "State.h"

#define DEFAULT_TAG_LED "LED"

enum LedState
{
    OFF,
    ON,
};

class Led
{
private:
    const char *_TAG;
    gpio_num_t _pin;
    enum LedState _state;

public:
    Led() {};
    Led(const char *tag = DEFAULT_TAG_LED, gpio_num_t pin, LedState state)
        : _TAG(tag), _pin(pin), _state(state) {};

    esp_err_t init();
    void blinking();
};
