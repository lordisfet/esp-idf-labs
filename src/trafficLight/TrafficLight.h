#pragma once

#include "stdio.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "led/Led.h"

#define DEFAULT_TAG_TRAFFIC_LIGHT "TRAFFIC_LIGHT"

enum TrafficLightInternalState
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
    TrafficLightInternalState _internalState;
    State _state;

public:
    TrafficLight(const int ledCount, Led *leds = nullptr, TrafficLightInternalState internalState = IDLE)
        : _ledCount(ledCount), _leds(leds), _internalState(internalState), _TAG(DEFAULT_TAG_TRAFFIC_LIGHT) {};

    TrafficLightInternalState getState() const { return _internalState; }

    void update();
    inline void switchState()
    {
        _state = !_state;
        ESP_LOGI(_TAG, "Traffic light state switched to: %s", _state == State::ON ? "ON" : "OFF");
    }
};