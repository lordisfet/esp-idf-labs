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
#include ".\music_player\note\MelodyNote.h"
#include ".\music_player\buzzer\Buzzer.h"
#include ".\music_player\songs\jingle_bells.h"

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
    static etl::vector<MelodyNote, MELODY_MAX_SIZE> melody;
    load_jingle_bells(melody);

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
