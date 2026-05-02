#include "TrafficLight.h"

using enum TrafficLightInternalState;

constexpr ulong RED_DURATION_US = 5000000;
constexpr ulong RED_AND_YELLOW_DURATION_US = 2000000;
constexpr ulong GREEN_DURATION_US = 5000000;
constexpr ulong GREEN_BLINKING_DURATION_US = 500000;
constexpr int GREEN_BLINKING_COUNT = 3;
constexpr ulong YELLOW_DURATION_US = 2000000;

enum LedName
{
    RED_LED = 0,
    YELLOW_LED = 1,
    GREEN_LED = 2
};

void TrafficLight::turnOffAllLeds()
{
    for (int i = 0; i < _ledCount; i++)
    {
        _leds[i].blinkOff();
    }
}

void TrafficLight::switchState(void *trafficLight)
{
    TrafficLight *tl = static_cast<TrafficLight *>(trafficLight);
    tl->_state = !tl->_state;
    if (tl->_state == State::ON)
    {
        tl->_internalState = TrafficLightInternalState::CALM;
        tl->_lastSwitchTime = esp_timer_get_time();
    }
    else if (tl->_state == State::OFF)
    {
        tl->turnOffAllLeds();
    }

    ESP_LOGI(tl->_TAG, "Traffic light state switched to: %s", tl->_state == State::ON ? "ON" : "OFF");
}

void TrafficLight::update()
{
    ulong currentTime = esp_timer_get_time();
    switch (_internalState)
    {
    case CALM:
        turnOffAllLeds();
        _internalState = RED;
        _lastSwitchTime = currentTime;
        _leds[RED_LED].blinkOn();
        ESP_LOGI(_TAG, "Traffic light switched to RED");
        break;
    case RED:
        if (currentTime - _lastSwitchTime >= RED_DURATION_US)
        {
            _internalState = RED_AND_YELLOW;
            _lastSwitchTime = currentTime;
            _leds[YELLOW_LED].blinkOn();
            ESP_LOGI(_TAG, "Traffic light switched to RED_AND_YELLOW");
        }
        break;
    case RED_AND_YELLOW:
        if (currentTime - _lastSwitchTime >= RED_AND_YELLOW_DURATION_US)
        {
            turnOffAllLeds();
            _internalState = GREEN;
            _lastSwitchTime = currentTime;
            _leds[GREEN_LED].blinkOn();
            ESP_LOGI(_TAG, "Traffic light switched to GREEN");
        }
        break;
    case GREEN:
        if (currentTime - _lastSwitchTime >= GREEN_DURATION_US)
        {
            // turnOffAllLeds();
            _internalState = GREEN_BLINKING;
            _lastSwitchTime = currentTime;
            ESP_LOGI(_TAG, "Traffic light switched to GREEN_BLINKING");
        }
        break;
    case GREEN_BLINKING:
        if (currentTime - _lastSwitchTime >= GREEN_BLINKING_DURATION_US)
        {
            if (_leds[GREEN_LED].getState() == State::ON)
            {
                _leds[GREEN_LED].blinkOff();
                if (_leds[GREEN_LED].getBlinkCounter() >= GREEN_BLINKING_COUNT)
                {
                    _leds[GREEN_LED].setBlinkCounter(0);
                    _internalState = YELLOW;
                    _leds[YELLOW_LED].blinkOn();
                    ESP_LOGI(_TAG, "Traffic light switched to YELLOW");
                }
            }
            else if (_leds[GREEN_LED].getState() == State::OFF)
            {
                _leds[GREEN_LED].blinkOn();
                _leds[GREEN_LED].setBlinkCounter(_leds[GREEN_LED].getBlinkCounter() + 1);
                ESP_LOGI(_TAG, "GREEN LED blinked %d times from %d", _leds[GREEN_LED].getBlinkCounter(), GREEN_BLINKING_COUNT);
            }
            _lastSwitchTime = currentTime;
        }

        break;
    case YELLOW:
        if (currentTime - _lastSwitchTime >= YELLOW_DURATION_US)
        {
            turnOffAllLeds();
            _internalState = CALM;
            _lastSwitchTime = currentTime;
            ESP_LOGI(_TAG, "Traffic light switched to CALM");
        }
        break;
    default:
        break;
    }
}