#pragma once

#include <etl/vector.h>
#include "../note/MelodyNote.h"

template <size_t MaxSize>
void load_naruto_blue_bird(etl::vector<MelodyNote, MaxSize>& melody) {
    melody.clear();
    
    // ==========================================
    // [INTRO] Вступление
    // ==========================================
    // e a b C b a (Habataitara)
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 300, 2048, 50));

    // e a b C D C D E (Modoranai to itte)
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 400, 2048, 50));

    // e a b C b a (Mezashita no wa)
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 300, 2048, 50));

    // a E D a E D g g a a (Aoi aoi ano sora)
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 600, 2048, 100)); // Конец интро

    // ==========================================
    // [VERSO] Куплет
    // ==========================================
    // e a b C e C b ("Kanashimi" wa mada)
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::E5, 150, 512,  20)); // проходящая нижняя
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 300, 2048, 40));

    // a g a c d e (Oboerarezu)
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 400, 2048, 50));

    // e e e f g a g ("Setsuna sa" wa ima)
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 300, 2048, 40));

    // g g g a b C b (Tsukami hajimeta)
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 400, 2048, 50));

    // e a b C e C b (Anata e to idaku)
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::E5, 150, 512,  20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 300, 2048, 40));

    // a g a c d e (Kono kanjou mo)
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 400, 2048, 50));

    // e e e f C b a g g a a (Ima kotoba ni kawatte iku)
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 300, 2048, 40));

    // e a b C [e] C b C b (Michi naru sekai no)
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::E5, 150, 512,  20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 300, 2048, 40));

    // b b C D [b] D C b a (Yume kara mezamete)
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 512,  20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 300, 2048, 40));

    // a C D E a C D E (Kono hane o hiroge)
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 300, 2048, 20));

    // E G F E (Tobitatsu) - Выход на пик!
    melody.push_back(MelodyNote(Note::E6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::G6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::F6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::E6, 600, 2048, 100)); // Пауза перед припевом


    // ==========================================
    // [REFRAO] Припев (Полный Форсаж)
    // ==========================================
    // e a b C b a
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 300, 2048, 50));

    // e a b C D C D E
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 400, 2048, 50));

    // e a b C b a
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 300, 2048, 50));

    // a E D a E D g g a a
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 400, 2048, 50));

    // e a b C b a
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 300, 2048, 50));

    // e a b C D C D E
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::E6, 400, 2048, 50));

    // e a b C b a (Furikiru hodo)
    melody.push_back(MelodyNote(Note::E5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::B5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 300, 2048, 50));

    // a E D a E D g g a a (x3 финал)
    for (int i = 0; i < 3; i++) {
        melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
        melody.push_back(MelodyNote(Note::E6, 300, 2048, 20));
        melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
        melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
        melody.push_back(MelodyNote(Note::E6, 300, 2048, 20));
        melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
        melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
        melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
        melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
        
        // На последнем круге тянем финальную ноту
        if (i == 2) {
            melody.push_back(MelodyNote(Note::A5, 1000, 2048, 0)); 
        } else {
            melody.push_back(MelodyNote(Note::A5, 400, 2048, 50)); 
        }
    }
}