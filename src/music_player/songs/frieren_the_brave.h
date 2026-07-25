#pragma once

#include <etl/vector.h>
#include "../note/MelodyNote.h"

template <size_t MaxSize>
void load_frieren_the_brave(etl::vector<MelodyNote, MaxSize>& melody) {
    melody.clear();

    // ==========================================
    // YOASOBI - The Brave (По твоим нотам)
    // РЕЖИМ: Гиперзвук (BPM ~170)
    // ==========================================

    // --- БЛОКИ 1-4: Главный хук (Вступление) ---
    // 5|d-d-d---c-c-c-----------c-|
    // 4|----------------g---A-----|
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::C5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 400, 2048, 40));
    melody.push_back(MelodyNote(Note::G4, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::A4, 200, 1024, 20)); // A = A# (Ля-диез)
    melody.push_back(MelodyNote(Note::C5, 200, 2048, 20));

    // Блок 2
    melody.push_back(MelodyNote(Note::A4, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::C5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::G4, 200, 1024, 20));

    // Блок 3: скачок вверх
    melody.push_back(MelodyNote(Note::A4, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::D5, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 100, 2048, 20));
    melody.push_back(MelodyNote(Note::A5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 200, 2048, 20));

    // Блок 4 & 5
    melody.push_back(MelodyNote(Note::C5, 300, 1024, 20));
    melody.push_back(MelodyNote(Note::A4, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::D5, 300, 2048, 20));
    
    // Выход в 6-ю октаву (Блок 5)
    melody.push_back(MelodyNote(Note::G5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::C6, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::F6, 300, 2048, 30));
    melody.push_back(MelodyNote(Note::D6, 150, 2048, 20)); // D = D# (Ре-диез)
    melody.push_back(MelodyNote(Note::D6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::D5, 200, 1024, 20));

    // --- БЛОКИ 11-14: Синтезаторное арпеджио (Сложный вираж) ---
    // Это быстрый "перебор" нот a-g-d-f-g-f-d. Делаем duty=512 для резкости (staccato)
    melody.push_back(MelodyNote(Note::A5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::G5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::D5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::F5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::G5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::F5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::D5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 150, 1024, 20));

    melody.push_back(MelodyNote(Note::A5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::G5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::D5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::F5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::G5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::F5, 100, 512, 15));
    melody.push_back(MelodyNote(Note::D5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 200, 1024, 20));

    // --- БЛОКИ 15-20: Подготовка к форсажу ---
    melody.push_back(MelodyNote(Note::G5, 300, 2048, 30));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 300, 2048, 20));

    melody.push_back(MelodyNote(Note::G4, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::A4, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 200, 1024, 20));

    melody.push_back(MelodyNote(Note::C5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 300, 2048, 30));
    melody.push_back(MelodyNote(Note::F5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::D5, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 100, 1024, 20));

    // --- БЛОКИ 25-28: Разгон перед припевом ---
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 100, 1024, 20));
    
    // Блок 28
    melody.push_back(MelodyNote(Note::G5, 150, 2048, 20));
    melody.push_back(MelodyNote(Note::F5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::A5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::A5, 400, 2048, 50)); // Пауза перед прыжком

    // --- БЛОКИ 29-35: ПРИПЕВ (Захват высоты 6-й октавы) ---
    // Здесь мы бьем на максимальной громкости (Duty 2048)
    melody.push_back(MelodyNote(Note::D5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::D6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 300, 2048, 20));

    melody.push_back(MelodyNote(Note::D5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::A5, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::A5, 300, 1024, 20));

    // Блок 31-32
    melody.push_back(MelodyNote(Note::C6, 150, 2048, 20));
    melody.push_back(MelodyNote(Note::A5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::C6, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D6, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::G5, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 100, 2048, 20));
    melody.push_back(MelodyNote(Note::A5, 300, 2048, 30));

    // Блок 33-35
    melody.push_back(MelodyNote(Note::F5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::C5, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::G5, 200, 2048, 20));

    melody.push_back(MelodyNote(Note::F5, 150, 2048, 20));
    melody.push_back(MelodyNote(Note::C5, 300, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::A4, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A4, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::G4, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::A4, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::A4, 300, 2048, 20));

    // --- БЛОКИ 41-46: Торможение и посадка (Outro) ---
    melody.push_back(MelodyNote(Note::D5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::A5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::A5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::G5, 300, 2048, 30));

    melody.push_back(MelodyNote(Note::G5, 150, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::F5, 300, 2048, 20));
    melody.push_back(MelodyNote(Note::D5, 300, 2048, 30));
    
    // Финальные аккорды
    melody.push_back(MelodyNote(Note::D5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::C5, 200, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::D5, 200, 2048, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::D5, 100, 1024, 20));
    melody.push_back(MelodyNote(Note::C5, 400, 1024, 50));
    
    // Глушение турбин
    melody.push_back(MelodyNote(Note::D5, 150, 512, 20));
    melody.push_back(MelodyNote(Note::D5, 150, 512, 20));
    melody.push_back(MelodyNote(Note::D5, 150, 512, 20));
    melody.push_back(MelodyNote(Note::C5, 800, 2048, 0));
}