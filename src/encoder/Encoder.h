#pragma once

#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"  
#include "driver/pulse_cnt.h"

#define MIN_STEP_DELAY_US 1000

class Encoder
{
private:
    pcnt_unit_handle_t _pcnt_unit = nullptr;
    pcnt_channel_handle_t _pcnt_channel_clk = nullptr;
    pcnt_channel_handle_t _pcnt_channel_dt = nullptr;
    
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

    Encoder();
    int getSteps() { return _steps; }
};