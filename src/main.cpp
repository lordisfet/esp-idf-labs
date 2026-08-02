#include <etl/vector.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "encoder/Encoder.h"
#include "servo/Servo.h"
#include "driver/Driver.h"
#include "music_player/buzzer/Buzzer.h"

#define SERVO                   "SERVO"
#define SERVO_PWM_TIMER         LEDC_TIMER_0
#define SERVO_PWM_MODE          LEDC_LOW_SPEED_MODE
#define SERVO_GPIO              GPIO_NUM_18
#define SERVO_PWM_CHANNEL       LEDC_CHANNEL_0
#define SERVO_PWM_FREQ          50  
#define SERVO_MIN_ANGLE         0
#define SERVO_MAX_ANGLE         180
#define SERVO_STEPS_COUNT       60
#define SERVO_MIN_PWM_DUTY_US   500
#define SERVO_MAX_PWM_DUTY_US   2500      
#define SERVO_ANGLE_BY_STEP     (SERVO_MAX_ANGLE - SERVO_MIN_ANGLE) / SERVO_STEPS_COUNT

#define ENCODER                 "ENCODER"
#define ENCODER_CLK_GPIO        GPIO_NUM_17
#define ENCODER_DT_GPIO         GPIO_NUM_15

#define BUZZER_NAME_SIZE        10
#define BUZZER_PWM_TIMER        LEDC_TIMER_1
#define BUZZER_PWM_MODE         LEDC_LOW_SPEED_MODE
#define BUZZER_GPIO             GPIO_NUM_21
#define BUZZER_PWM_CHANNEL      LEDC_CHANNEL_1
#define BUZZER_PWM_DEFAULT_FREQ 50
#define BUZZER_PWM_DEFAULT_DUTY 0
#define MELODY_SIZE             10

extern "C" void app_main() 
{
    PWM pwm_servo(SERVO_GPIO);
    Servo servo(pwm_servo, SERVO_MIN_ANGLE, SERVO_MIN_PWM_DUTY_US, 
        SERVO_MAX_ANGLE, SERVO_MAX_PWM_DUTY_US);
 
    Encoder encoder(ENCODER_CLK_GPIO, ENCODER_DT_GPIO);

    PWM pwm_buzzer(BUZZER_GPIO, BUZZER_PWM_DEFAULT_FREQ, BUZZER_PWM_DEFAULT_DUTY 
        ,BUZZER_PWM_TIMER, BUZZER_PWM_CHANNEL);
    etl::string<BUZZER_NAME_SIZE> name_buzzer = "BUZZER";
    etl::vector<MelodyNote, MELODY_SIZE> stop_signal;
    stop_signal.push_back(MelodyNote(Note::A5, 100, pwm_buzzer.getMaxDuty(), 50));
    stop_signal.push_back(MelodyNote(Note::A5, 100, pwm_buzzer.getMaxDuty(), 50));
    stop_signal.push_back(MelodyNote(Note::Ds4, 300, pwm_buzzer.getMaxDuty(), 0));
    Buzzer buzzer(name_buzzer, pwm_buzzer, stop_signal);

    Driver driver(encoder, buzzer, servo, SERVO_ANGLE_BY_STEP);
    int previous_angle = 0;

    while (true)
    {
        driver.update();
        if (previous_angle != servo.getCurrentAngle())
        {
            previous_angle = servo.getCurrentAngle();
            ESP_LOGI(SERVO, "Current servo angle: %d", previous_angle);
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
    