#pragma once

#include "freertos/task.h"
#include <etl/vector.h>
#include <etl/vector.h>

#include "../melody/note/MelodyNote.h"
#include "../pwm/PWM.h"

#define MELODY_MAX_SIZE 128

using namespace etl;

class Buzzer
{
private:
    PWM& _pwm;
    const ivector<MelodyNote>& _melody;
    uint16_t _current_note;
    uint32_t _next_action_time;
    bool _isPlaying;
    bool _isPause;

    void updateNote() {
        MelodyNote note = _melody[_current_note];
        _pwm.updateDuty(note.getDuty());
        _pwm.updateFrequency(static_cast<uint32_t>(note.getNote()));

        _current_note++;
        if (_current_note >= MELODY_MAX_SIZE)
        {
            _current_note = 0;
        }
    }
public:
    Buzzer(PWM& pwm, const ivector<MelodyNote>& melody, uint32_t current_note = 0);

    void play();
    void start();
    void stop();
};
