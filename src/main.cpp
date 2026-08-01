#include <PWM.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "encoder/Encoder.h"
#include "servo/Servo.h"

#define SERVO                   "SERVO"
#define SERVO_PWM_TIMER         LEDC_TIMER_0
#define SERVO_PWM_MODE          LEDC_LOW_SPEED_MODE
#define SERVO_GPIO              GPIO_NUM_18
#define SERVO_PWM_CHANNEL       LEDC_CHANNEL_0
#define SERVO_PWM_FREQ          50        

#define ENCODER                 "ENCODER"
#define ENCODER_CLK_GPIO        GPIO_NUM_17
#define ENCODER_DT_GPIO         GPIO_NUM_15


extern "C" void app_main() 
{
    PWM pwm(SERVO_GPIO);
    Servo servo(pwm, 0, 500, 180, 2500);
    Encoder encoder(ENCODER_CLK_GPIO, ENCODER_DT_GPIO);
    uint16_t angle = 0;
    bool goForward = true;
    int  previus_count = 0;

    while (true)
    {
        if (previus_count != encoder.getSteps())
        {
            previus_count = encoder.getSteps();
            ESP_LOGI(ENCODER, "Current step is: %d", previus_count);
        }
        
        // if (goForward)
        // {
        //     if (angle < 180)
        //     {
        //         angle++;
        //     }
        //     else {goForward = false;}
        // }
        // else if (!goForward)
        // {
        //     if (angle > 0)
        //     {
        //         angle--;
        //     }
        //     else {goForward = true;}
        // }
        
        // ESP_ERROR_CHECK(servo.setAngle(angle));
        // ESP_LOGI(SERVO, "Current angle: %d", angle);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
    