#pragma once

#include <stdio.h>

#include "driver/ledc.h"

#define DEFAULT_FREQUENCY 100
#define DEFAULT_DUTY 2048
#define DEFAUL_TIMER_NUM LEDC_TIMER_0
#define DEFAULT_CHANNEL_NUM LEDC_CHANNEL_0 

class PWM
{
private:
    uint8_t _pin;
    ledc_timer_t _timer_num;
    ledc_channel_t _channel;
    uint32_t _freq;
    uint32_t _duty;
public:
    PWM(uint8_t pin, ledc_timer_t timer_num = DEFAUL_TIMER_NUM, 
    ledc_channel_t channel = DEFAULT_CHANNEL_NUM, 
    uint32_t freq = DEFAULT_FREQUENCY, uint32_t duty = DEFAULT_DUTY);

    void updateFrequency(uint32_t new_freq);
    void updateDuty(uint32_t new_duty);
    void pause() {ledc_timer_pause(LEDC_LOW_SPEED_MODE, _timer_num);}
    void resume() {ledc_timer_resume(LEDC_LOW_SPEED_MODE, _timer_num);}
};
