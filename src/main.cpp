#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <time.h>
#include "esp_task_wdt.h"

#include "MyButton.h"

gpio_num_t PIN_BUTTON = GPIO_NUM_4;
const char *TAG_BUTTON = "BUTTON";

bool trafficLightLevel = false;

void switchTrafficLight()
{
    trafficLightLevel = !trafficLightLevel;
    ESP_LOGI(TAG_BUTTON, "Traffic light switched! to %s", trafficLightLevel ? "ON" : "OFF");
}

extern "C" void app_main()
{
    MyButton button(PIN_BUTTON, TAG_BUTTON, ACTIVE_LOW, PULL_UP, switchTrafficLight);
    button.init();
    while (1)
    {
        button.update();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
