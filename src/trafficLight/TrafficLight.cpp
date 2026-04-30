#include "TrafficLight.h"

using enum TrafficLightInternalState;

void TrafficLight::update()
{
    switch (_internalState)
    {
    case CALM:
        break;
    case RED:
        break;
    case RED_AND_YELLOW:
        break;
    case YELLOW:
        break;
    case GREEN:
        break;
    case GREEN_BLINKING:
        break;
    default:
        break;
    }
}