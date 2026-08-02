#include "Driver.h"

void Driver::update() {
    _buzzer.play();
    if (_encoder.getMovingDirection() != 0)
    {
        int new_angle = _servo.getCurrentAngle() + (_encoder.getMovingDirection() * _servoAngleRotationByStep);
    
        if (new_angle >= _servo.getMinAngle() && new_angle <= _servo.getMaxAngle())
        {
            _servo.setAngle(new_angle);
            _encoder.resetMovingDirection();
        }
        else {
            _buzzer.start();
        }
    }
}