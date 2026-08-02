#pragma once

#include <etl/vector.h>
#include "../note/MelodyNote.h"

template <size_t MaxSize>
void load_jingle_bells(etl::vector<MelodyNote, MaxSize>& melody) {
    melody.clear();
    
    // --- Джингл Белз (Jingle Bells) ---
    // Куплет
    melody.push_back(MelodyNote(Note::E5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::E5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::E5, 500, 2048, 50));
    
    melody.push_back(MelodyNote(Note::E5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::E5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::E5, 500, 2048, 50));
    
    melody.push_back(MelodyNote(Note::E5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::G5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::C5, 350, 2048, 30));
    melody.push_back(MelodyNote(Note::D5, 150, 2048, 20));
    melody.push_back(MelodyNote(Note::E5, 800, 2048, 100));
    
    // Продолжение темы
    melody.push_back(MelodyNote(Note::F5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::F5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::F5, 350, 2048, 30));
    melody.push_back(MelodyNote(Note::F5, 150, 2048, 20));
    
    melody.push_back(MelodyNote(Note::F5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::E5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::E5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::E5, 125, 2048, 15));
    melody.push_back(MelodyNote(Note::E5, 125, 2048, 15));
    
    melody.push_back(MelodyNote(Note::E5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::D5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::D5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::E5, 250, 2048, 30));
    melody.push_back(MelodyNote(Note::D5, 500, 2048, 50));
    melody.push_back(MelodyNote(Note::G5, 500, 2048, 0));
}