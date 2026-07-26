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

#define TASK_DELAY 5

#define TAG_BUTTON "BUTTON"
#define GPIO_BUTTON GPIO_NUM_16

#define TAG_SERVO "SERVO"
#define GPIO_SERVO GPIO_NUM_4
#define SERVO_LEFT_DUTY_MS 1
#define SERVO_RIGHT_DUTY_MS 2
#define PWM_FREQ 50

#define TAG_HANDLE "HANDLE"
#define GPIO_HANDLE GPIO_NUM_1

#define ADC_CONV_FRAME_SIZE TASK_DELAY * SOC_ADC_DIGI_RESULT_BYTES
#define ADC_FRAME_COUNT 4
#define ADC_BUFFER_MAX_SIZE ADC_FRAME_COUNT * ADC_CONV_FRAME_SIZE
#define ADC_PATTERN_NUM 1
#define ADC_ATTEN ADC_ATTEN_DB_11
#define ADC_CHANNEL ADC_CHANNEL_0
#define ADC_UNIT ADC_UNIT_1
#define ADC_BITWIDTH ADC_BITWIDTH_12
#define ADC_CONV_MODE ADC_CONV_SINGLE_UNIT_1
#define ADC_DIGI_OUTPUT_FORMAT ADC_DIGI_OUTPUT_FORMAT_TYPE2

extern "C" void app_main()
{
    PWM pwm(GPIO_SERVO, PWM_FREQ, 0);
    adc_oneshot_unit_handle_t handle =nullptr;
    adc_oneshot_unit_init_cfg_t cfg = {
        .unit_id = ADC_UNIT, 
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE 
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&cfg, &handle));

    adc_oneshot_chan_cfg_t channel_cfg = {
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(handle, ADC_CHANNEL, &channel_cfg));
    int raw;

    while (true)
    {
        adc_oneshot_read(handle, ADC_CHANNEL, &raw);
        // pwm.updateDuty();
        ESP_LOGI(TAG_HANDLE, "ADC value: %d", raw);
        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY));
    }
}
