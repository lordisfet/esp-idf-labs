#pragma once

#include "stdio.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "led/Led.h"

using enum TrafficLightInternalState;
using enum State;

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
                 TrafficLightInternalState internalState = CALM, State state = OFF)
        : _ledCount(ledCount), _leds(leds), _TAG(tag), _internalState(internalState), _state(state) {};

    State getState() const { return _state; }

    void update();
    static void switchState(void *trafficLight)
    {
        TrafficLight *tl = static_cast<TrafficLight *>(trafficLight);
        tl->_state = !tl->_state;
        if (tl->_state == ON)
        {
            tl->_internalState = CALM;
            tl->_lastSwitchTime = esp_timer_get_time();
        }
        else if (tl->_state == OFF)
        {
            tl->turnOffAllLeds();
        }

        ESP_LOGI(tl->_TAG, "Traffic light state switched to: %s", tl->_state == ON ? "ON" : "OFF");
    }

    void turnOffAllLeds()
    {
        for (int i = 0; i < _ledCount; i++)
        {
            _leds[i].blinkOff();
        }
    }
};