#pragma once

#include "stdio.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_task_wdt.h"

#include "led/Led.h"

#define DEFAULT_TAG_TRAFFIC_LIGHT "TRAFFIC_LIGHT"

enum class TrafficLightInternalState
{
    CALM,
    RED,
    RED_AND_YELLOW,
    YELLOW,
    GREEN,
    GREEN_BLINKING,
};

class TrafficLight
{
private:
    const int _ledCount;
    Led *_leds;
    const char *_TAG;
    TrafficLightInternalState _internalState;
    State _state;
    ulong _lastSwitchTime = 0;

public:
    TrafficLight(const int ledCount, Led *leds = nullptr, const char *tag = DEFAULT_TAG_TRAFFIC_LIGHT,
                 TrafficLightInternalState internalState = TrafficLightInternalState::CALM, State state = State::OFF)
        : _ledCount(ledCount), _leds(leds), _TAG(tag), _internalState(internalState), _state(state) {};

    State getState() const { return _state; }

    void update();
    static void switchState(void *trafficLight);
    void turnOffAllLeds();
};