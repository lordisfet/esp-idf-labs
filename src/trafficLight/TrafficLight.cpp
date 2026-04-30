#pragma once

#include "stdio.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "led/Led.h"

#define DEFAULT_TAG_TRAFFIC_LIGHT "TRAFFIC_LIGHT"

enum TrafficLightState
{
    IDLE,
    RED,
    RED_AND_YELLOW,
    YELLOW,
    GREEN,
    GREEN_BLINKING,
};

class TrafficLight
{
private:
    const char *_TAG;
    const int _ledCount;
    Led *_leds;
    TrafficLightState _state;

public:
    TrafficLight(const int ledCount, Led *leds = nullptr, TrafficLightState state = IDLE)
        : _ledCount(ledCount), _leds(leds), _state(state), _TAG(DEFAULT_TAG_TRAFFIC_LIGHT) {};

    ~TrafficLight();
};
