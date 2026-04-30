#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <time.h>
#include "esp_task_wdt.h"

#include "MyButton.h"
#include "led/Led.h"
#include "trafficLight/TrafficLight.h"

constexpr int ledcount = 3;

gpio_num_t PIN_BUTTON = GPIO_NUM_4;
const char *TAG_BUTTON = "BUTTON";
gpio_num_t PIN_RED_LED = GPIO_NUM_1;
const char *TAG_RED_LED = "RED_LED";
gpio_num_t PIN_YELLOW_LED = GPIO_NUM_40;
const char *TAG_YELLOW_LED = "YELLOW_LED";
gpio_num_t PIN_GREEN_LED = GPIO_NUM_37;
const char *TAG_GREEN_LED = "GREEN_LED";

volatile bool trafficLightLevel = false;

extern "C" void app_main()
{
    Led const leds[] = {
        Led(PIN_RED_LED, TAG_RED_LED),
        Led(PIN_YELLOW_LED, TAG_YELLOW_LED),
        Led(PIN_GREEN_LED, TAG_GREEN_LED),
    };

    TrafficLight trafficLight(ledcount, const_cast<Led *>(leds));

    MyButton button(PIN_BUTTON, TAG_BUTTON, ACTIVE_LOW, PULL_UP, TrafficLight::switchState, &trafficLight);
    button.init();
    while (1)
    {
        button.update();
        if (trafficLight.getState() == State::ON)
        {
            trafficLight.update();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
