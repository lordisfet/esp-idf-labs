#include "Servo.h"

esp_err_t Servo::setAngle(uint16_t angle) 
{
    if (angle < _min_point.angle || angle > _max_point.angle) {return ESP_ERR_INVALID_ARG;}
    
    uint16_t new_duty = _min_point.duty_us + ((static_cast<double>(angle) - _min_point.angle) 
        / (_max_point.angle - _min_point.angle)) * (_max_point.duty_us - _min_point.duty_us);

    _pwm.setDutyAsUs(new_duty);
    _current_point.angle = angle;
    _current_point.duty_us = new_duty;
    
    return ESP_OK;
}