#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

#include "Buzzer.h"

Buzzer::Buzzer(const char* name, PWM& pwm, const ivector<MelodyNote>& melody, uint32_t current_note)
    : _name(name),
      _pwm(pwm),
      _melody(melody),
      _state(BuzzerState::IDLE),
      _current_note(current_note),
      _next_action_time(0)
{}

void Buzzer::start() {
    if (_melody.empty()) {
        ESP_LOGE("MELODY", "Melody is empty");
        return;
    }
    
    _current_note = 0;
    _state = BuzzerState::START;
}

void Buzzer::stop() {
    _state = BuzzerState::STOP;
}

void Buzzer::play() {
    uint32_t now = xTaskGetTickCount();
    switch (_state)
    {
    case BuzzerState::IDLE:
        break;
    case BuzzerState::START:
        ESP_LOGI(_name, "Start next note");
        _next_action_time = now + pdMS_TO_TICKS(_melody[_current_note].getDuration());
        _pwm.setFrequency(static_cast<uint32_t>(_melody[_current_note].getNote()));
        _pwm.setDutyAsUs(_melody[_current_note].getDuty());
        _pwm.resume();
        _state = BuzzerState::PLAY;
        break;
    case BuzzerState::PLAY:
        if (now > _next_action_time)
        {
            _next_action_time += pdMS_TO_TICKS(_melody[_current_note].getPause());
            _pwm.pause();
            _state = BuzzerState::PAUSE;
            ESP_LOGI(_name, "Played note: %lu", static_cast<uint32_t>(_melody[_current_note].getNote()));
        }
        break;
    case BuzzerState::PAUSE:
        if (now > _next_action_time)
        {
            _current_note++;
            if (_current_note >= _melody.size())
            {
                _state = BuzzerState::STOP;
            }
            else {
                _state = BuzzerState::START;
                ESP_LOGI(_name, "Paused");
            }
        }
        break;
    case BuzzerState::STOP:
        _pwm.pause();
        _state = BuzzerState::IDLE;
        break;
    default:
        break;
    }
}