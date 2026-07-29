#pragma once

#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"  

#define MIN_STEP_DELAY_US 1000

class Encoder
{
private:
    gpio_num_t _clk;
    gpio_num_t _dt;
    
    enum {
        IDLE,
        RIGHT_STEP,
        LEFT_STEP,
        RELEASE
    } _state;

    volatile uint64_t _last_step_time;
    volatile int _steps;

    static void IRAM_ATTR isr_handler(void* arg);
public:
    QueueHandle_t step_queue; 

    Encoder(gpio_num_t clk, gpio_num_t dt);
    int getSteps() { return _steps; }
};