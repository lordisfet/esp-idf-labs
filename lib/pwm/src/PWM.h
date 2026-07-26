#pragma once

#include <stdio.h>
#include <cmath>

#include "driver/ledc.h"
#include "esp_log.h"

#define DEFAULT_FREQUENCY 50
#define DEFAULT_DUTY 2048
#define DEFAUL_TIMER_NUM LEDC_TIMER_0
#define DEFAULT_CHANNEL_NUM LEDC_CHANNEL_0 
#define TO_US 1000000

class PWM
{
private:
    uint8_t _pin;
    uint32_t _freq;
    uint32_t _duty;
    ledc_timer_t _timer_num;
    ledc_channel_t _channel;
    ledc_intr_type_t _intr_type;
    ledc_mode_t _speed_mode;
    ledc_timer_bit_t _duty_res;
    double _max_duty;
    ledc_clk_cfg_t _clk_cfg;
public:
    PWM(uint8_t pin, uint32_t freq = DEFAULT_FREQUENCY, 
        uint32_t duty = DEFAULT_DUTY, ledc_timer_t timer_num = DEFAUL_TIMER_NUM, 
        ledc_channel_t channel = DEFAULT_CHANNEL_NUM, 
        ledc_intr_type_t intr_type = LEDC_INTR_DISABLE,
        ledc_mode_t speed_mode = LEDC_LOW_SPEED_MODE, 
        ledc_timer_bit_t duty_res = LEDC_TIMER_12_BIT, 
        ledc_clk_cfg_t clk_cfg = LEDC_AUTO_CLK);

    uint32_t getPeriod() {return TO_US/_freq;}   
    uint32_t getDutyInUs() {return (_duty/_max_duty) * getPeriod();} 

    void updateFrequency(uint32_t new_freq);
    void updateDuty(uint32_t new_duty);
    void pause() {ledc_timer_pause(_speed_mode, _timer_num);}
    void resume() {ledc_timer_resume(_speed_mode, _timer_num);}

    void setDutyAsUs(uint32_t ms);
};
