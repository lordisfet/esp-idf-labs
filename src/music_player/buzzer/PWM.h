#include <stdio.h>

#include "driver/ledc.h"

#define DEFAULT_FREQUENCY 0
#define DEFAULT_DUTY 0
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
    PWM(uint8_t pin, ledc_timer_t timer_num, ledc_channel_t channel, uint32_t freq, uint32_t duty);
    void updateFrequency(uint32_t new_freq);
    void updateDuty(uint32_t new_duty) {_duty = new_duty;}
    ~PWM();
};

PWM::PWM(uint8_t pin, ledc_timer_t = DEFAUL_TIMER_NUM, 
    ledc_channel_t channel = DEFAULT_CHANNEL_NUM, 
    uint32_t freq = DEFAULT_FREQUENCY, uint32_t duty = DEFAULT_DUTY)
{
    ledc_timer_config_t pwm_timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_12_BIT,
        .timer_num = _timer_num,
        .freq_hz = _freq,
        .clk_cfg = LEDC_USE_XTAL_CLK,
    };

    ledc_channel_config_t buzzer_channel_config = {
        .gpio_num = _pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = _channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = _timer_num,
        .duty = _duty,
    };
}

void PWM::updateFrequency(uint32_t new_freq) {
    _freq = new_freq;
    ledc_set_freq(LEDC_LOW_SPEED_MODE, _timer_num, _freq);
}

void PWM::updateDuty(uint32_t new_duty) {
    _duty = new_duty;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, _channel, _duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, _channel);
}