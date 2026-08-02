#include "servo/Servo.h"
#include "encoder/Encoder.h"
#include "music_player/buzzer/Buzzer.h"

class Driver
{
private:
    Encoder &_encoder;
    Buzzer &_buzzer;
    Servo &_servo;
    int _servoAngleRotationByStep;
public:
    Driver(Encoder &encoder, Buzzer &buzzer, Servo &servo, int angleByStep) : _encoder(encoder), 
        _buzzer(buzzer), _servo(servo), _servoAngleRotationByStep(angleByStep) {};
    void update(); 
};
