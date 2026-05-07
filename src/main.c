#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "soc/gpio_struct.h"
#include "soc/adc_channel.h"

#define GPIO_ADC GPIO_NUM_4
#define ADC_CLK_SRC_DEFAULT 0
#define TAG "ADC_ONESHOT_EXAMPLE"
#define ADC1_READ_DELAY_MS 1000

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
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_GPIO4_CHANNEL, &adc1_chan_cfg));

    while (1)
    {
        int adc_raw;
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_GPIO4_CHANNEL, &adc_raw));
        ESP_LOGI(TAG, "ADC Raw Data: %d", adc_raw);
        vTaskDelay(pdMS_TO_TICKS(ADC1_READ_DELAY_MS));
    }
}