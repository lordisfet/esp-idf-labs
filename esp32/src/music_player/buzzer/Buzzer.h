#pragma once

#include <PWM.h>

#include "freertos/task.h"
#include <etl/vector.h>
#include <etl/string.h>

#include "../note/MelodyNote.h"
#include "esp_log.h"
#include "BuzzetState.h"

#define MAX_NAME_LENGHT 20

using namespace etl;

class Buzzer
{
private:
    const char* _name;
    PWM& _pwm;
    const ivector<MelodyNote>& _melody;
    BuzzerState _state;
    uint16_t _current_note;
    uint32_t _next_action_time;
public:
    Buzzer(const char* name, PWM& pwm, const ivector<MelodyNote>& melody, uint32_t current_note = 0);

    void play();
    void start();
    void stop();
};
