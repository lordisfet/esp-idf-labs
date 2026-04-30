#pragma once

#include "stdio.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "State.h"

#define DEFAULT_TAG_LED "LED"

class Led
{
private:
    gpio_num_t _pin;
    const char *_TAG;
    State _state;

public:
    Led(gpio_num_t pin, const char *tag = DEFAULT_TAG_LED, State state = State::OFF)
        : _pin(pin), _TAG(tag), _state(state) {};

    esp_err_t init();
    void turnOn();
    void turnOff();
    void blink(ulong interval_ms);
};
