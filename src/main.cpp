#include <stdio.h>
#include <PWM.h>
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
#include "hal/adc_types.h"
#include "esp_adc/adc_continuous.h"
#include "adc/ADC.h"

#include <Button.h>
#include "encoder/Encoder.h"

#define TASK_DELAY 10

#define ENCODER "ENCODER"
#define CLK "CLK"
#define GPIO_CLK GPIO_NUM_17
#define DT "DT"
#define GPIO_DT GPIO_NUM_15

#define TAG_SERVO "SERVO"
#define GPIO_SERVO GPIO_NUM_4
#define SERVO_LEFT_DUTY_US 500
#define SERVO_RIGHT_DUTY_US 2500
#define PWM_FREQ 50

extern "C" void app_main()
{
    Button clk(CLK, GPIO_CLK);
    Button dt(DT, GPIO_DT);
    Encoder encoder(clk, dt);

    PWM pwm(GPIO_SERVO, PWM_FREQ, 0);
    int raw;

    while (true)
    {
        encoder.update();
        static int old_steps = 0;
        if (encoder.getSteps() != old_steps)
        {
            old_steps = encoder.getSteps();
            ESP_LOGI(ENCODER, "current count of steps: %d", old_steps);
        }

        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY));
    }
}
