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

#include <ESP32Encoder.h>

#define TASK_DELAY 5

#define TAG_BUTTON "BUTTON"
#define GPIO_BUTTON GPIO_NUM_16

#define TAG_SERVO "SERVO"
#define GPIO_SERVO GPIO_NUM_4
#define SERVO_LEFT_DUTY_US 500
#define SERVO_RIGHT_DUTY_US 2500
#define PWM_FREQ 50

#define TAG_HANDLE "HANDLE"
#define GPIO_HANDLE GPIO_NUM_1

#define ADC_CONV_FRAME_SIZE TASK_DELAY * SOC_ADC_DIGI_RESULT_BYTES
#define ADC_FRAME_COUNT 4
#define ADC_BUFFER_MAX_SIZE ADC_FRAME_COUNT * ADC_CONV_FRAME_SIZE
#define ADC_PATTERN_NUM 1
#define ADC_ATTEN ADC_ATTEN_DB_11
#define ADC_CHANNEL ADC_CHANNEL_0
#define ADC_SAMPLE_FREQ 1000
#define ADC_UNIT ADC_UNIT_1
#define ADC_BITWIDTH ADC_BITWIDTH_12
#define ADC_CONV_MODE ADC_CONV_SINGLE_UNIT_1
#define ADC_DIGI_OUTPUT_FORMAT ADC_DIGI_OUTPUT_FORMAT_TYPE2

extern "C" void app_main()
{
    ESP32Encoder encoder;
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = ADC_BUFFER_MAX_SIZE,
        .conv_frame_size = ADC_CONV_FRAME_SIZE,
    };

    adc_digi_pattern_config_t adc_patterns[ADC_PATTERN_NUM] = {
        {
            .atten = ADC_ATTEN,
            .channel = ADC_CHANNEL,
            .unit = ADC_UNIT,
            .bit_width = ADC_BITWIDTH
        }
    };

    adc_continuous_config_t continuous_config = {
        .pattern_num = ADC_PATTERN_NUM,
        .adc_pattern = adc_patterns,
        .sample_freq_hz = ADC_SAMPLE_FREQ,
        .conv_mode = ADC_CONV_MODE,
        .format = ADC_DIGI_OUTPUT_FORMAT,
    };

    uint8_t buffer[ADC_BUFFER_MAX_SIZE] = {0};
    ADC adc(adc_config, continuous_config, buffer, ADC_BUFFER_MAX_SIZE, 50);
    PWM pwm(GPIO_SERVO, PWM_FREQ, 0);
    int raw;

    while (true)
    {
        raw = adc.get_filtered_raw(ADC_UNIT, ADC_CHANNEL);
        uint32_t newDuty = SERVO_LEFT_DUTY_US + (SERVO_RIGHT_DUTY_US - SERVO_LEFT_DUTY_US) * ((double)raw/adc.get_max_adc_value());
        pwm.setDutyAsUs(newDuty);
        
        ESP_LOGI(TAG_HANDLE, "ADC value: %d, PWM duty in us: %lu", raw, pwm.getDutyInUs());
        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY));
    }
}
