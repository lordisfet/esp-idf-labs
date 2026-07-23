#include "./Note.h"

class MelodyNote
{
private:
    Note _note;
    uint16_t _duration;
    uint16_t _duty;
    uint16_t _pause;
public:
    MelodyNote(Note note, uint16_t duration, uint16_t duty, uint16_t pause) : 
        _note(note), _duration(duration), _duty(duty), _pause(pause) {};
    ~MelodyNote();

    Note getNote() const {return _note;}
    uint16_t getDuration() const {return _duration;}
    uint16_t getDuty() const {return _duty;}
    uint16_t getPause() const {return _pause;}
};