#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <time.h>
#include "esp_task_wdt.h"

#define PIN_BUTTON 4
#define PIN_BUTTON_MASK 1ULL << PIN_BUTTON
#define DEBOUNCE_TIME_US 50000

const char *TAG_BUTTON = "BUTTON";
volatile bool isClickedWithoutDebounce = false;

enum ButtonState
{
    IDLE,
    RELEASED,
    DEBOUNCE,
    PRESSED,
};

void IRAM_ATTR button_intterapt(void *arg)
{
    volatile int *ptr = arg;
    (*ptr)++;
    isClickedWithoutDebounce = true;
}

enum ButtonState button_update(volatile int *clickCount)
{
    static enum ButtonState internallState = RELEASED;
    static ulong lastLevelSwitchTime;
    static int lastlevel = 1;

    enum ButtonState stateToReturn = internallState;
    int currentLevel = gpio_get_level(PIN_BUTTON);
    ulong currentTime = esp_timer_get_time();

    switch (internallState)
    {
    case IDLE:
        if (currentLevel != lastlevel)
        {
            lastLevelSwitchTime = currentTime;
            internallState = DEBOUNCE;
        }
        break;
    case DEBOUNCE:
        if (currentTime - lastLevelSwitchTime >= DEBOUNCE_TIME_US && currentLevel != lastlevel)
        {
            if (currentLevel == 0)
            {
                internallState = PRESSED;
            }
            if (currentLevel == 1)
            {
                internallState = RELEASED;
            }
        }
        break;
    case PRESSED:
        ++(*clickCount);
        lastlevel = currentLevel;
        internallState = IDLE;
        break;
    case RELEASED:
        // обробка відпусканян для завдання не потрібна, але додаванян RELEASED описує всі стани кнопки
        // бо для обробки всіх станів кнопки потрібно обробляти і відпускання
        // Потім цю функцію можна додати в структуру та передавати вказівники на функції як формальний параметр,
        // що будуть виконуватися при натисканні/відпусканні кнопки
        lastlevel = currentLevel;
        internallState = IDLE;
        break;
    default:
        break;
    }

    return stateToReturn;
}

void app_main()
{
    gpio_install_isr_service(0);
    volatile int countClickWithoutDebounce = 0;
    volatile int countClickWithFSM = 0;

    const gpio_config_t configButton =
        {
            .pin_bit_mask = PIN_BUTTON_MASK,
            .mode = GPIO_MODE_INPUT,
            .pull_down_en = false,
            .pull_up_en = true,
            .intr_type = GPIO_INTR_NEGEDGE};

    ESP_ERROR_CHECK(gpio_config(&configButton));
    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_BUTTON, button_intterapt, (void *)&countClickWithoutDebounce));

    while (1)
    {
        if (isClickedWithoutDebounce)
        {
            ESP_LOGI(TAG_BUTTON, "Button clicked without debounce. Count: %d", countClickWithoutDebounce);
            isClickedWithoutDebounce = false;
        }
        if (button_update(&countClickWithFSM) == PRESSED)
        {
            ESP_LOGI(TAG_BUTTON, "Button clicked with FSM. Count: %d", countClickWithFSM);
        }

        // Знайшов цей костильний спосіб для скидання wdt, бо поки не розумію сам принцип як реалізований wdt
        // та як ним керувати, але якщо його не скинути контролер просто перезавантажуэться
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
