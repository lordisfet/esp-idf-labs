#include "TrafficLight.h"

using enum TrafficLightInternalState;

constexpr ulong CALM_DURATION_MS = 2000000;

void TrafficLight::update()
{
    ulong currentTime = esp_timer_get_time();
    switch (_internalState)
    {
    case CALM:
        turnOffAllLeds();
        _internalState = RED;
        _lastSwitchTime = currentTime;
        _leds[0].blinkOn();
        ESP_LOGI(_TAG, "Traffic light switched to RED");

        break;
    case RED:
        if (currentTime - _lastSwitchTime >= CALM_DURATION_MS)
        {
            turnOffAllLeds();
            _internalState = YELLOW;
            _lastSwitchTime = currentTime;
            _leds[1].blinkOn();
            ESP_LOGI(_TAG, "Traffic light switched to YELLOW");
        }

        break;
    case RED_AND_YELLOW:
        break;
    case YELLOW:
        if (currentTime - _lastSwitchTime >= CALM_DURATION_MS)
        {
            turnOffAllLeds();
            _internalState = GREEN;
            _lastSwitchTime = currentTime;
            _leds[2].blinkOn();
            ESP_LOGI(_TAG, "Traffic light switched to GREEN");
        }

        break;
    case GREEN:
        if (currentTime - _lastSwitchTime >= CALM_DURATION_MS)
        {
            _internalState = CALM;
            _lastSwitchTime = currentTime;
            turnOffAllLeds();
            ESP_LOGI(_TAG, "Traffic light switched to CALM");
        }

        break;
    case GREEN_BLINKING:
        break;
    default:
        break;
    }
}