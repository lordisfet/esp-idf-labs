#include <stdio.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "soc/gpio_struct.h"
#include "soc/adc_channel.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "sma/sma.h"

#define GPIO_ADC GPIO_NUM_8
#define GPIO_LED GPIO_NUM_40

#define ADC_CLK_SRC_DEFAULT ADC_RTC_CLK_SRC_DEFAULT
#define TAG "ADC_ONESHOT"
#define ADC1_CHANNEL ADC_CHANNEL_7
#define ADC1_READ_DELAY pdMS_TO_TICKS(50)
#define ADC1_BIT_WIDTH ADC_BITWIDTH_12
#define ADC1_MAX_RAW ((1 << ADC1_BIT_WIDTH) - 1)
#define ADC1_ATTEN ADC_ATTEN_DB_11
#define ADC1_V_REF 3.3

#define V_TO_MV 1000
#define SMA_SIZE 10

bool led_update(int voltage_mv, gpio_num_t led_gpio, int &smoothed_voltage_mv)
{
    static SMA<SMA_SIZE> sma;
    const int threshold_mv = 1600;
    const int hysteresis_mv = 400;

    sma.add(voltage_mv);
    smoothed_voltage_mv = static_cast<int>(sma.get_average());

    if (smoothed_voltage_mv > threshold_mv)
    {
        gpio_set_level(led_gpio, 0);
        return false;
    }
    else if (smoothed_voltage_mv < threshold_mv - hysteresis_mv)
    {
        gpio_set_level(led_gpio, 1);
    }

    return true;
}

extern "C" void app_main()
{
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t adc1_config = {
        .unit_id = ADC_UNIT_1,
        .clk_src = ADC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc1_config, &adc1_handle));

    adc_oneshot_chan_cfg_t adc1_chan_cfg = {
        .atten = ADC1_ATTEN,
        .bitwidth = ADC1_BIT_WIDTH,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_7, &adc1_chan_cfg));

    adc_cali_scheme_ver_t supported_schemes;
    adc_cali_handle_t cali_handle;

    esp_err_t ret = adc_cali_check_scheme(&supported_schemes);
    if (ret == ESP_OK)
    {
        if (supported_schemes & ADC_CALI_SCHEME_VER_CURVE_FITTING)
        {
            ESP_LOGI(TAG, "This MCU provides Curve Fitting.");
            adc_cali_curve_fitting_config_t curve_fitting_config = {
                .unit_id = ADC_UNIT_1,
                .chan = ADC_CHANNEL_7,
                .atten = ADC1_ATTEN,
                .bitwidth = ADC1_BIT_WIDTH,
            };
            ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&curve_fitting_config, &cali_handle));
        }
        else if (supported_schemes & ADC_CALI_SCHEME_VER_LINE_FITTING)
        {
            ESP_LOGI(TAG, "This MCU provides Line Fitting.");
        }
    }
    else
    {
        ESP_LOGE(TAG, "Error checking calibration schemes: %s", esp_err_to_name(ret));
    }

    gpio_reset_pin(GPIO_LED);
    gpio_config_t led_conf = {
        .pin_bit_mask = (1ULL << GPIO_LED),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&led_conf));
    gpio_set_level(GPIO_LED, 0);

    int raw, voltage_calibrated, smoothed_voltage_mv;
    bool led_status = false;
    while (1)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_CHANNEL, &raw));
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, raw, &voltage_calibrated));
        ESP_LOGI(TAG, "Raw: %d, Calibrated Voltage: %d mV, Smoothed Voltage: %d mV", raw, voltage_calibrated, smoothed_voltage_mv);
        if (led_status != led_update(voltage_calibrated, GPIO_LED, smoothed_voltage_mv))
        {
            led_status = !led_status;
            ESP_LOGI(TAG, "LED status changed: %s", led_status ? "ON" : "OFF");
        }

        vTaskDelay(ADC1_READ_DELAY);
    }
}