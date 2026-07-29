#include <stdio.h>
#include <math.h>
#include <cstring>

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

#define TAG_BUTTON "BUTTON"
#define GPIO_BUTTON GPIO_NUM_16

#define TAG_LED_ADC "LED_ADC"
#define GPIO_LED_ADC GPIO_NUM_9
#define ADC_CHANNEL_LED ADC1_GPIO9_CHANNEL
#define TAG_LED "LED"
#define GPIO_LED GPIO_NUM_21

#define TAG_MOTOR_ADC "MOTOR_ADC"
#define GPIO_MOTOR_ADC GPIO_NUM_4
#define ADC_CHANNEL_MOTOR ADC1_GPIO4_CHANNEL
#define TAG_MOTOR "MOTOR"
#define GPIO_MOTOR GPIO_NUM_37

#define ADC_UNIT ADC_UNIT_1
#define ADC_READ_DELAY pdMS_TO_TICKS(20)
#define ADC_BITWIDTH ADC_BITWIDTH_12
#define ADC_MAX_RAW ((1 << ADC_BITWIDTH) - 1)
#define ADC_ATTEN ADC_ATTEN_DB_11
#define ADC_V_REF 3.3
#define ADC_PATTERN_NUM 2
#define ADC_SAMPLE_FREQ_HZ 1000 * ADC_PATTERN_NUM
#define ADC_RESULT_SET_SIZE 20
#define ADC_FRAME_SET_SIZE 8
#define ADC_CONV_FRAME_SIZE ADC_RESULT_SET_SIZE * SOC_ADC_DIGI_RESULT_BYTES
#define ADC_MAX_BUFFER_SIZE ADC_FRAME_SET_SIZE * ADC_CONV_FRAME_SIZE

#define PWM_FREQUENCY 2000
#define PWM_MAX_DUTY ADC_MAX_RAW
#define PWM_DUTY_CYCLE (uint16_t)(0 * PWM_MAX_DUTY)
 
#define V_TO_MV 1000
#define SMA_SIZE 10

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

    gpio_config_t led_conf = {
        .pin_bit_mask = (1ULL << GPIO_LED),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&led_conf));

    gpio_config_t motor_conf = {
        .pin_bit_mask = (1ULL << GPIO_MOTOR),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&motor_conf));

    // --- ADC configuration and allocation --- 
    adc_continuous_handle_t adc_handle = nullptr;
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = ADC_MAX_BUFFER_SIZE,
        .conv_frame_size = ADC_CONV_FRAME_SIZE,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &adc_handle));

    adc_digi_pattern_config_t adc_patterns[ADC_PATTERN_NUM] = {
        {
            .atten = ADC_ATTEN,
            .channel = ADC_CHANNEL_LED,
            .unit = ADC_UNIT_1,
            .bit_width = ADC_BITWIDTH
        },
        {
            .atten = ADC_ATTEN,
            .channel = ADC_CHANNEL_MOTOR,
            .unit = ADC_UNIT_1,
            .bit_width = ADC_BITWIDTH
        }
    };

    adc_continuous_config_t continuous_config = {
        .pattern_num = ADC_PATTERN_NUM,
        .adc_pattern = adc_patterns,
        .sample_freq_hz = ADC_SAMPLE_FREQ_HZ,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
    };
    ESP_ERROR_CHECK(adc_continuous_config(adc_handle, &continuous_config));
    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));


    // --- PWM configuration and allocation ---
    ledc_timer_config_t led_timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_12_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = PWM_FREQUENCY,
        .clk_cfg = LEDC_USE_XTAL_CLK,
    };

    ledc_channel_config_t led_channel_config = {
        .gpio_num = GPIO_LED,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = PWM_DUTY_CYCLE,
    };

    ledc_channel_config_t motor_channel_config = {
        .gpio_num = GPIO_MOTOR,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = PWM_DUTY_CYCLE,
    };

    ESP_ERROR_CHECK(ledc_timer_config(&led_timer_config));
    ESP_ERROR_CHECK(ledc_channel_config(&led_channel_config));
    ESP_ERROR_CHECK(ledc_channel_config(&motor_channel_config));

    static adc_digi_output_data_t raw[ADC_RESULT_SET_SIZE];
    static uint8_t buffer[ADC_CONV_FRAME_SIZE] = {0};

    uint32_t length;
    uint32_t samples;
    uint32_t avg_raw_data;

    while (1)
    {
        button.update();
        if(button.isPressed())
        {
            
            ledc_timer_resume(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
            adc_continuous_read(adc_handle, buffer, ADC_CONV_FRAME_SIZE, &length, ADC_MAX_DELAY);
            parse_adc_buffer(buffer, length, raw, &samples);

            avg_raw_data = calculate_avg_raw(raw, ADC_UNIT, ADC_CHANNEL_LED, samples);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, avg_raw_data);
            ESP_LOGI(TAG_LED_ADC, "ADC value: %lu, amount of measurements %lu", avg_raw_data, samples);
            ESP_LOGI(TAG_LED, "PWM duty cycle: %lu", ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

            avg_raw_data = calculate_avg_raw(raw, ADC_UNIT, ADC_CHANNEL_MOTOR, samples);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, avg_raw_data);
            ESP_LOGI(TAG_MOTOR_ADC, "ADC value: %lu, amount of measurements %lu", avg_raw_data, samples);
            ESP_LOGI(TAG_LED, "PWM duty cycle: %lu", ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        }
        else
        {
            ledc_timer_pause(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
            gpio_set_level(GPIO_LED, 0);
            gpio_set_level(GPIO_MOTOR, 0);
        }

        vTaskDelay(ADC_READ_DELAY);
    }
}