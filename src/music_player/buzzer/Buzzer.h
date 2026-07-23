#include <etl/vector.h>

#include "../melody/note/MelodyNote.h"

#define MELODY_MAX_SIZE 4096

using namespace etl;

class Buzzer
{
private:
    vector<MelodyNote, MELODY_MAX_SIZE> _melody;
public:
    Buzzer(/* args */);
    ~Buzzer();
};

Buzzer::Buzzer(/* args */)
{
}

Buzzer::~Buzzer()
{
}
