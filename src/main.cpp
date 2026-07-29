#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "encoder/Encoder.h"


#define SERVO_PWM_TIMER         LEDC_TIMER_0
#define SERVO_PWM_MODE          LEDC_LOW_SPEED_MODE
#define SERVO_OUTPUT_IO         (GPIO_NUM_18) 
#define SERVO_PWM_CHANNEL       LEDC_CHANNEL_0
#define SERVO_PWM_FREQ          50            

void init_servo() 
{
    ledc_timer_config_t timer_conf = {
        .speed_mode       = SERVO_PWM_MODE,
        .duty_resolution  = LEDC_TIMER_12_BIT, 
        .timer_num        = SERVO_PWM_TIMER,
        .freq_hz          = SERVO_PWM_FREQ,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

    ledc_channel_config_t channel_conf = {
        .gpio_num         = SERVO_OUTPUT_IO,
        .speed_mode       = SERVO_PWM_MODE,
        .channel          = SERVO_PWM_CHANNEL,
        .intr_type        = LEDC_INTR_DISABLE,
        .timer_sel        = SERVO_PWM_TIMER,
        .duty             = 0,
        .hpoint           = 0
    };
    ledc_channel_config(&channel_conf);
}

uint32_t angle_to_duty(int angle) 
{
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    
    uint32_t duty = 102 + ((angle * (512 - 102)) / 180);
    return duty;
}

extern "C" void app_main() 
{
    gpio_install_isr_service(0);
    init_servo();

    Encoder encoder(GPIO_NUM_17, GPIO_NUM_15);
    int current_steps = 0;

    ESP_LOGI("SERVO", "Система управления рулями активирована.");

    while (true)
    {
        if (xQueueReceive(encoder.step_queue, &current_steps, portMAX_DELAY))
        {
            int target_angle = current_steps * 5;

            if (target_angle < 0) target_angle = 0;
            if (target_angle > 180) target_angle = 180;

            uint32_t duty = angle_to_duty(target_angle);
            ledc_set_duty(SERVO_PWM_MODE, SERVO_PWM_CHANNEL, duty);
            ledc_update_duty(SERVO_PWM_MODE, SERVO_PWM_CHANNEL);

            ESP_LOGI("SERVO", "Шаги: %d | Угол серво: %d град.", current_steps, target_angle);
        }
    }
}