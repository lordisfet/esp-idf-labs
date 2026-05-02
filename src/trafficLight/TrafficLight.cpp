#include "TrafficLight.h"

using enum TrafficLightInternalState;

constexpr ulong CALM_DURATION_MS = 2000000;

void TrafficLight::update()
{
    ulong currentTime = esp_timer_get_time();
    static ulong lastSwitchTime = 0;
    switch (_internalState)
    {
    case CALM:
        if (currentTime - lastSwitchTime >= CALM_DURATION_MS)
        {
            turnOffAllLeds();
            _internalState = RED;
            lastSwitchTime = currentTime;
            _leds[0].blinkOn();
            ESP_LOGI(_TAG, "Traffic light switched to RED");
        }

        break;
    case RED:
        if (currentTime - lastSwitchTime >= CALM_DURATION_MS)
        {
            turnOffAllLeds();
            _internalState = YELLOW;
            lastSwitchTime = currentTime;
            _leds[1].blinkOn();
            ESP_LOGI(_TAG, "Traffic light switched to YELLOW");
        }

        break;
    case RED_AND_YELLOW:
        break;
    case YELLOW:
        if (currentTime - lastSwitchTime >= CALM_DURATION_MS)
        {
            turnOffAllLeds();
            _internalState = GREEN;
            lastSwitchTime = currentTime;
            _leds[2].blinkOn();
            ESP_LOGI(_TAG, "Traffic light switched to GREEN");
        }

        break;
    case GREEN:
        if (currentTime - lastSwitchTime >= CALM_DURATION_MS)
        {
            _internalState = CALM;
            lastSwitchTime = currentTime;
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