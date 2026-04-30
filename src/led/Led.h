#pragma once

#include "stdio.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

enum LedState
{
    OFF,
    ON,
};

class Led
{
private:
    gpio_num_t _pin;
    enum LedState _state;

public:
    Led(gpio_num_t pin, LedState state) : _pin(pin), _state(state) {};

    esp_err_t init();
    void blinking();
};
