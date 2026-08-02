#include <Button.h>

#include "servo/Servo.h"
#include "encoder/Encoder.h"
#include "music_player/buzzer/Buzzer.h"

constexpr int _defaultAngleByStep = 3;

class Driver
{
private:
    Encoder &_encoder;
    Buzzer &_buzzer;
    Button &_button;
    Servo &_servo;
    int _servoAngleRotationByStep;
public:
    Driver(Encoder &encoder, Buzzer &buzzer, Button &button, Servo &servo, int angleByStep = _defaultAngleByStep) 
        : _encoder(encoder), _buzzer(buzzer), _button(button), _servo(servo), _servoAngleRotationByStep(angleByStep) {};

    void update(); 

    void multiplyAnglebyStep() {_servoAngleRotationByStep *= 2; ESP_LOGI("DRIVER", "step was doubled");}
    void resetAngleByStepToDefault() {_servoAngleRotationByStep = _defaultAngleByStep; ESP_LOGI("DRIVER", "step was reset to default");}
};
