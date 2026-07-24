#pragma once

#include <stdio.h>

#include "./Note.h"

class MelodyNote
{
private:
    Note _note;
    uint32_t _duration;
    uint32_t _duty;
    uint32_t _pause;
public:
    MelodyNote(Note note, uint32_t duration, uint32_t duty, uint32_t pause) : 
        _note(note), _duration(duration), _duty(duty), _pause(pause) {};

    Note getNote() const {return _note;}
    uint32_t getDuration() const {return _duration;}
    uint32_t getDuty() const {return _duty;}
    uint32_t getPause() const {return _pause;}
};