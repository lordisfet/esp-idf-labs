#pragma once 

#include <stdint.h>
#include <PWM.h>

class Servo
{
private:
    class Point
    {
    public:
        uint16_t angle;
        uint32_t duty_us;
        Point(uint16_t angle, uint32_t duty) :  angle(angle), duty_us(duty) {};
        Point() {};
    };

    PWM _pwm;
    const Point _min_point;
    const Point _max_point;
    const uint16_t _start_angle = (_min_point.angle + _max_point.angle) / 2;
    
    Point _current_point;

public:
    Servo(PWM pwm, uint16_t min_angle, uint32_t min_duty, uint16_t max_angle, uint32_t max_duty) 
        : _pwm(pwm), _min_point(min_angle, min_duty), _max_point(max_angle, max_duty) 
        {setAngle(_start_angle);}

    int getCurrentAngle() {return _current_point.angle;}
    int getMinAngle() {return _min_point.angle;}
    int getMaxAngle() {return _max_point.angle;}

    esp_err_t setAngle(uint16_t angle);
};
