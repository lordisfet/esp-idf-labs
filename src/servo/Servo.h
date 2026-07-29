#pragma once 

#include <stdint.h>
#include <PWM.h>

class Servo
{
private:
    class Boundary
    {
    public:
        uint16_t angle;
        uint32_t duty_us;
        Boundary(uint16_t angle, uint32_t duty) :  angle(angle), duty_us(duty) {};
    };

    PWM _pwm;
    Boundary _min_point;
    Boundary _max_point;

public:
    Servo(PWM pwm, uint16_t min_angle, uint32_t min_duty, uint16_t max_angle, uint32_t max_duty) 
        : _pwm(pwm), _min_point(min_angle, min_duty), _max_point(max_angle, max_duty) {};

    esp_err_t setAngle(uint16_t angle);
};
