#pragma once

#include <atomic>

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
    pcnt_unit_handle_t _unit = nullptr;
    pcnt_channel_handle_t _channel_clk = nullptr;
    pcnt_channel_handle_t _channel_dt = nullptr;
    int _movingDirection = 0; // 1 clockwise turn, 0 dont move, -1 counterclockwise turn 
    // std::atomic<int> _steps{0};
    // int _previous_steps{0};

    static bool IRAM_ATTR isr_handle(pcnt_unit_handle_t unit, 
        const pcnt_watch_event_data_t *edata, void *user_ctx);
public:
    int getMovingDirection() {return _movingDirection;}

    void resetMovingDirection() {_movingDirection = 0;}

    Encoder(gpio_num_t clk, gpio_num_t dt);
};