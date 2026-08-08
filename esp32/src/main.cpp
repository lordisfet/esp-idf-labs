#include <etl/vector.h>
#include <etl/delegate.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/uart.h"

#include "Button.h"
#include "protocol.h"

uint8_t uart_num = UART_NUM_1;
constexpr char* BTN_TAG = "BTN_BOOT";
constexpr gpio_num_t BTN_GPIO = GPIO_NUM_0;
constexpr gpio_num_t LED_GPIO = GPIO_NUM_37;
constexpr gpio_num_t TX_GPIO = GPIO_NUM_4;
constexpr gpio_num_t RX_GPIO = GPIO_NUM_5;
const uint rx_buffer_size = 128;
// constexpr gpio_port_t BTN_PORT = GPIO_PORT_0;

void sent_toggle_comand() {
    uint8_t src = CMD_TOGGLE_LED;
    uart_write_bytes(uart_num, &src, sizeof(src));
}

extern "C" void app_main() 
{
    Button btn_boot(BTN_TAG, BTN_GPIO);
    btn_boot.setOnClick(etl::delegate<void()>::create<sent_toggle_comand>());

    uart_config_t uart_cfg = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB
    };
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_cfg));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, TX_GPIO, RX_GPIO, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    constexpr int uart_buffer_size = (1024 * 2);
    uint data[rx_buffer_size];
    ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size, uart_buffer_size, 0, NULL, 0));

    gpio_reset_pin(LED_GPIO);
    gpio_config_t led_gpio_cfg = {
        .pin_bit_mask = 1ULL << LED_GPIO,
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&led_gpio_cfg));

    while (true)
    {
        btn_boot.update();
        if (uart_read_bytes(uart_num, data, rx_buffer_size, pdMS_TO_TICKS(10)) != 0)
        {
            if (gpio_get_level(LED_GPIO) == 0)
            {
                gpio_set_level(LED_GPIO, 1);
            }
            else 
            {
                gpio_set_level(LED_GPIO, 0);
            }
        }
        
        // vTaskDelay(pdMS_TO_TICKS(200));
        // length = uart_read_bytes(uart_num, data, max_len, pdMS_TO_TICKS(10));
        // data[length] = '\0';
        // ESP_LOGI("RX_DATA", "recieved string: %s", (const char*)data);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
