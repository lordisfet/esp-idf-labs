#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Buzzer.h"


Buzzer::Buzzer(PWM& pwm, const ivector<MelodyNote>& melody, uint32_t current_note)
    : _pwm(pwm),
      _melody(melody),
      _current_note(current_note),
      _next_action_time(0),
      _isPlaying(false),
      _isPause(false)
{};

void Buzzer::start() {
    if (_melody.empty()) return; // Защита от "пустой" мелодии

    _isPlaying = true;
    _isPause = false;
    _current_note = 0;

    // Включаем первую ноту мгновенно
    const MelodyNote& note = _melody[_current_note];
    _pwm.updateFrequency(static_cast<uint32_t>(note.getNote()));
    _pwm.updateDuty(note.getDuty());

    // Запоминаем, когда нота должна выключиться
    _next_action_time = xTaskGetTickCount() + pdMS_TO_TICKS(note.getDuration());
}

void Buzzer::stop() {
    _isPlaying = false;
    _pwm.updateDuty(0); // Глушим ШИМ
}

void Buzzer::play() {
    // Если выключено — мгновенный выход (0 микросекунд потерь времени)
    if (!_isPlaying) return;

    uint32_t now = xTaskGetTickCount();

    // Если время следующего действия еще не пришло — мгновенный выход
    if (now < _next_action_time) return;

    if (_isPause) {
        // Пауза закончилась, нужно играть следующую ноту
        _current_note++;
        
        if (_current_note >= _melody.size()) {
            // Мелодия закончилась. 
            // Замени на _current_note = 0; если нужно зациклить звук
            stop(); 
            return; 
        }

        // Включаем следующую ноту
        const MelodyNote& note = _melody[_current_note];
        _pwm.updateFrequency(static_cast<uint32_t>(note.getNote()));
        _pwm.updateDuty(note.getDuty());
        
        _isPause = false;
        _next_action_time = now + pdMS_TO_TICKS(note.getDuration());
    } 
    else {
        // Нота отыграла, нужно включить паузу
        const MelodyNote& note = _melody[_current_note];
        
        _pwm.updateDuty(0); // Выключаем звук
        
        _isPause = true;
        _next_action_time = now + pdMS_TO_TICKS(note.getPause());
    }
}