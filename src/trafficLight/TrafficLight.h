#pragma once

#include "stdio.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

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

public:
    TrafficLight(const int ledCount, Led *leds = nullptr, const char *tag = DEFAULT_TAG_TRAFFIC_LIGHT,
                 TrafficLightInternalState internalState = TrafficLightInternalState::CALM, State state = State::OFF)
        : _ledCount(ledCount), _leds(leds), _TAG(tag), _internalState(internalState), _state(state) {};

    State getState() const { return _state; }

    void update();
    static void switchState(void *trafficLight)
    {
        TrafficLight *tl = static_cast<TrafficLight *>(trafficLight);
        tl->_state = !tl->_state;
        ESP_LOGI(tl->_TAG, "Traffic light state switched to: %s", tl->_state == State::ON ? "ON" : "OFF");
    }

    void turnOffAllLeds()
    {
        for (int i = 0; i < _ledCount; i++)
        {
            _leds[i].blinkOff();
        }
    }
};