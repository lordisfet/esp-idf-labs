#pragma once

#include "freertos/task.h"
#include <etl/vector.h>
#include <etl/string.h>

#include "../note/MelodyNote.h"
#include "../pwm/PWM.h"
#include "esp_log.h"
#include "BuzzetState.h"

#define MAX_NAME_LENGHT 20

using namespace etl;

class Buzzer
{
private:
    const string<MAX_NAME_LENGHT>& _name;
    PWM& _pwm;
    const ivector<MelodyNote>& _melody;
    BuzzerState _state;
    uint16_t _current_note;
    uint32_t _next_action_time;
public:
    Buzzer(const string<MAX_NAME_LENGHT>& name, PWM& pwm, const ivector<MelodyNote>& melody, uint32_t current_note = 0);

    void play();
    void start();
    void stop();
};
