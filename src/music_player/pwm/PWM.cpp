#include "PWM.h"

PWM::PWM(uint8_t pin, ledc_timer_t timer_num, ledc_channel_t channel, 
        uint32_t freq, uint32_t duty) : 
        _pin(pin), _timer_num(timer_num), _channel(channel), _freq(freq), _duty(duty)
{
    ledc_timer_config_t pwm_timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_12_BIT,
        .timer_num = _timer_num,
        .freq_hz = _freq,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ledc_channel_config_t buzzer_channel_config = {
        .gpio_num = _pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = _channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = _timer_num,
        .duty = _duty,
        .hpoint = 0,
        .flags = 0,
    };

    ESP_ERROR_CHECK(ledc_timer_config(&pwm_timer_config));
    ESP_ERROR_CHECK(ledc_channel_config(&buzzer_channel_config));
    pause();
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
