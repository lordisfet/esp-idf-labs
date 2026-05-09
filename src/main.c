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

#define GPIO_ADC GPIO_NUM_4
#define ADC_CLK_SRC_DEFAULT 0
#define TAG "ADC_ONESHOT_EXAMPLE"
#define ADC1_READ_DELAY_MS 1000
#define ADC1_BIT_WIDTH ADC_BITWIDTH_12
#define ADC1_MAX_RAW ((1 << ADC1_BIT_WIDTH) - 1)
#define ADC1_ATTEN ADC_ATTEN_DB_11
#define ADC1_V_REF 3.3
#define V_TO_MV 1000

int adc_manual_calculation(int raw)
{
    return raw * ADC1_V_REF * V_TO_MV / (double)ADC1_MAX_RAW;
}

int adc_cali_calculation(adc_cali_handle_t cali_handle, int raw)
{
    int calibrated_value;
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, raw, &calibrated_value));
    return calibrated_value;
}

void app_main()
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

    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_GPIO4_CHANNEL, &adc1_chan_cfg));

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
                .chan = ADC1_GPIO4_CHANNEL,
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

    while (1)
    {
        int adc_raw, adc_mv_manual, adc_mv_calibrated;
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_GPIO4_CHANNEL, &adc_raw));
        ESP_LOGI(TAG, "ADC Raw Data: %d", adc_raw);
        adc_mv_manual = adc_manual_calculation(adc_raw);
        adc_mv_calibrated = adc_cali_calculation(cali_handle, adc_raw);
        ESP_LOGI(TAG, "ADC Voltage: %d mV", adc_mv_manual);
        ESP_LOGI(TAG, "ADC Calibrated Voltage: %d mV", adc_mv_calibrated);
        ESP_LOGI(TAG, "Manual diff to calibrated: %.2f %%", fabs((adc_mv_manual - adc_mv_calibrated) * 100. / (ADC1_V_REF * V_TO_MV)));
        ESP_LOGI(TAG, "-----------------------------");

        vTaskDelay(pdMS_TO_TICKS(ADC1_READ_DELAY_MS));
    }
}