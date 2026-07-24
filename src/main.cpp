#include <stdio.h>
#include <math.h>
#include <cstring>
#include <etl/vector.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "soc/gpio_struct.h"
#include "soc/adc_channel.h"
#include "soc/soc_caps.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_continuous.h"
#include "Button.h"
#include ".\music_player\pwm\PWM.h"
#include ".\music_player\melody\note\MelodyNote.h"
#include ".\music_player\buzzer\Buzzer.h"

#define TAG_BUTTON "BUTTON"
#define GPIO_BUTTON GPIO_NUM_16

#define TAG_BUZZER "BUZZER"
#define GPIO_BUZZER GPIO_NUM_21
#define PWM_BUZZER_COEF 2./5

#define PWM_FREQUENCY 2000
#define PWM_MAX_DUTY ADC_MAX_RAW
#define PWM_DUTY_CYCLE (uint16_t)(0 * PWM_MAX_DUTY)

#define TASK_DELAY 20

void parse_adc_buffer(uint8_t *buffer, uint32_t lenght, adc_digi_output_data_t *raw, uint32_t *num_samples){
    uint32_t num = lenght / SOC_ADC_DIGI_RESULT_BYTES;
    for (uint32_t i = 0; i < num; i++) {
        memcpy(&raw[i], buffer + i * SOC_ADC_DIGI_RESULT_BYTES, sizeof(adc_digi_output_data_t));
    }
    *num_samples = num;  
}
uint32_t calculate_avg_raw(adc_digi_output_data_t *raw, uint8_t unit, uint8_t channel, const uint32_t num_samples) {
    uint32_t sum = 0;
    uint8_t current_samples = 0;
    for (uint32_t i = 0; i < num_samples; i++) {
        if (unit == raw[i].type2.unit && channel == raw[i].type2.channel)
        {
            sum += raw[i].type2.data;
            current_samples++;
        }
    }

    return sum/current_samples;
}
extern "C" void app_main()
{
    Button button(TAG_BUTTON, GPIO_BUTTON);

    PWM pwm(GPIO_BUZZER);
    static etl::vector<MelodyNote, MELODY_MAX_SIZE> astronomia = {

        // Intro riff
        MelodyNote(Note::E4, 250, 2048, 40),
        MelodyNote(Note::F4, 250, 2048, 40),
        MelodyNote(Note::G4, 250, 2048, 40),
        MelodyNote(Note::E4, 250, 2048, 40),

        MelodyNote(Note::D4, 250, 2048, 40),
        MelodyNote(Note::C4, 250, 2048, 40),
        MelodyNote(Note::D4, 250, 2048, 40),
        MelodyNote(Note::E4, 250, 2048, 40),

        // Repeat with slight variation
        MelodyNote(Note::G4, 250, 2048, 40),
        MelodyNote(Note::A4, 250, 2048, 40),
        MelodyNote(Note::G4, 250, 2048, 40),
        MelodyNote(Note::F4, 250, 2048, 40),

        MelodyNote(Note::E4, 250, 2048, 40),
        MelodyNote(Note::D4, 250, 2048, 40),
        MelodyNote(Note::C4, 250, 2048, 40),
        MelodyNote(Note::D4, 250, 2048, 40),

        // Main hook
        MelodyNote(Note::E4, 300, 2048, 50),
        MelodyNote(Note::G4, 300, 2048, 50),
        MelodyNote(Note::C5, 300, 2048, 50),
        MelodyNote(Note::B4, 300, 2048, 50),

        MelodyNote(Note::A4, 300, 2048, 50),
        MelodyNote(Note::G4, 300, 2048, 50),
        MelodyNote(Note::F4, 300, 2048, 50),
        MelodyNote(Note::E4, 300, 2048, 50),

        // Ending loop
        MelodyNote(Note::D4, 250, 2048, 40),
        MelodyNote(Note::E4, 250, 2048, 40),
        MelodyNote(Note::F4, 250, 2048, 40),
        MelodyNote(Note::G4, 250, 2048, 40),

        MelodyNote(Note::A4, 250, 2048, 40),
        MelodyNote(Note::G4, 250, 2048, 40),
        MelodyNote(Note::F4, 250, 2048, 40),
        MelodyNote(Note::E4, 250, 2048, 40),
    };

    Buzzer buzzer(pwm, astronomia);
    buzzer.start();
    while (1)
    {
        button.update();
        if(button.isPressed())
        {
            buzzer.play();
            ESP_LOGI(TAG_BUZZER, "ACTIVE...");
        }
        else
        {
            buzzer.stop();
            ESP_LOGI(TAG_BUZZER, "NONACTINE...");
        }

        xTaskGetTickCount();
        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY));
    }
}
