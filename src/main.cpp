#include <stdio.h>
#include <math.h>
#include <cstring>
#include <etl/vector.h>
#include <etl/delegate.h>

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

#define MELODY_MAX_SIZE 128

#define TASK_DELAY 20

extern "C" void app_main()
{
    PWM pwm(GPIO_BUZZER);
    static etl::vector<MelodyNote, MELODY_MAX_SIZE> melody = {
    // --- Разгон (Habataitara modoranai to itte) ---
    MelodyNote(Note::A5, 150, 2048, 20),
    MelodyNote(Note::B5, 150, 2048, 20),
    MelodyNote(Note::C6, 300, 2048, 30),
    MelodyNote(Note::B5, 150, 2048, 20),
    MelodyNote(Note::A5, 150, 2048, 20),
    MelodyNote(Note::G5, 150, 2048, 20),
    MelodyNote(Note::A5, 400, 2048, 50),

    // --- Набор высоты (Mezashita no wa) ---
    MelodyNote(Note::C6, 150, 2048, 20),
    MelodyNote(Note::D6, 150, 2048, 20),
    MelodyNote(Note::E6, 300, 2048, 30),
    MelodyNote(Note::D6, 150, 2048, 20),
    MelodyNote(Note::C6, 150, 2048, 20),
    MelodyNote(Note::B5, 150, 2048, 20),
    MelodyNote(Note::C6, 400, 2048, 50),

    // --- Крейсерский режим (Aoi aoi ano sora) ---
    MelodyNote(Note::E6, 200, 2048, 20),
    MelodyNote(Note::E6, 200, 2048, 20),
    MelodyNote(Note::E6, 200, 2048, 20),
    MelodyNote(Note::C6, 150, 2048, 20),
    MelodyNote(Note::D6, 500, 2048, 50),

    // --- Переход на форсаж (Прыжок в 6 и 7 октавы) ---
    MelodyNote(Note::A6, 150, 2048, 20),
    MelodyNote(Note::B6, 150, 2048, 20),
    MelodyNote(Note::C7, 300, 2048, 30),
    MelodyNote(Note::B6, 150, 2048, 20),
    MelodyNote(Note::A6, 150, 2048, 20),
    MelodyNote(Note::G6, 150, 2048, 20),
    
    // --- Захват цели (Фиксация) ---
    MelodyNote(Note::A6, 600, 2048, 0)
};

    const etl::string<MAX_NAME_LENGHT> name = "BUZZER";
    Buzzer buzzer(name, pwm, melody);
    Button button(
        TAG_BUTTON, 
        GPIO_BUTTON,
        etl::delegate<void()>::create<Buzzer, &Buzzer::start>(buzzer),
        etl::delegate<void()>::create<Buzzer, &Buzzer::stop>(buzzer)
    );
    while (1)
    {
        button.update();
        buzzer.play();
        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY));
    }
}
