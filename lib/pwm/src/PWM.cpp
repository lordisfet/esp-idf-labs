#include "./PWM.h"

PWM::PWM(uint8_t pin, uint32_t freq, uint32_t duty, 
        ledc_timer_t timer_num, ledc_channel_t channel, 
        ledc_intr_type_t intr_type, 
        ledc_mode_t speed_mode, ledc_timer_bit_t duty_res, ledc_clk_cfg_t clk_cfg) : 
        _pin(pin),  _freq(freq), _duty(duty), _timer_num(timer_num), 
        _channel(channel), _intr_type(intr_type), _speed_mode(speed_mode), 
        _duty_res(duty_res), _clk_cfg(clk_cfg)
{
    _max_duty = pow(2, duty_res) - 1;

    ledc_timer_config_t pwm_timer_config = {
        .speed_mode = _speed_mode,
        .duty_resolution = _duty_res,
        .timer_num = _timer_num,
        .freq_hz = _freq,
        .clk_cfg = _clk_cfg,
    };

    ledc_channel_config_t buzzer_channel_config = {
        .gpio_num = _pin,
        .speed_mode = _speed_mode,
        .channel = _channel,
        .intr_type = _intr_type,
        .timer_sel = _timer_num,
        .duty = _duty,
        .hpoint = 0,
        .flags = 0,
    };

    ESP_ERROR_CHECK(ledc_timer_config(&pwm_timer_config));
    ESP_ERROR_CHECK(ledc_channel_config(&buzzer_channel_config));
}

void PWM::updateFrequency(uint32_t new_freq) {
    _freq = new_freq;
    ledc_set_freq(_speed_mode, _timer_num, _freq);
}

void PWM::updateDuty(uint32_t new_duty) {
    _duty = new_duty;
    ledc_set_duty(_speed_mode, _channel, _duty);
    ledc_update_duty(_speed_mode, _channel);
}

void PWM::setDutyAsUs(uint32_t us){
    if (us > getPeriod())
    {
        ESP_LOGE("PWM", "duty in us is greater than pwm period, max us`s value is %lu", getPeriod());
        return;
    }
    
    uint32_t new_duty = _max_duty * ((double)us / getPeriod());
    updateDuty(new_duty);
}
