#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"

#include <u8g2.h>
#include <u8x8.h>

#include "esp_rom_sys.h"

#define TASK_DELAY_MILIS 30
#define SDA_PIN GPIO_NUM_15
#define SCL_PIN GPIO_NUM_2
#define I2C_PORT I2C_NUM_0 
#define SSD1306_HEIGHT 64
#define SSD1306_WIDTH 128

void i2c_master_init()
{
    i2c_config_t conf = {}; 
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_PIN;
    conf.scl_io_num = SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;

    i2c_param_config(I2C_PORT, &conf);
    i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0);
}

uint8_t write_byte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    // U8X8_MSG_BYTE_INIT
    // U8X8_MSG_BYTE_SEND 30
    // U8X8_MSG_BYTE_SET_DC 31
    // U8X8_MSG_BYTE_START_TRANSFER
    // U8X8_MSG_BYTE_END_TRANSFER
    // U8X8_MSG_BYTE_SET_I2C_ADR (obsolete)
    // U8X8_MSG_BYTE_SET_DEVICE (obsolete)
    static i2c_cmd_handle_t handle;
    switch (msg)
    {
    case U8X8_MSG_BYTE_INIT:
        /* code */
        break;
    case U8X8_MSG_BYTE_START_TRANSFER: {
        handle = i2c_cmd_link_create();

        i2c_master_start(handle);

        uint8_t i2c_address = u8x8_GetI2CAddress(u8x8); 
        i2c_master_write_byte(handle, i2c_address | I2C_MASTER_WRITE, true);
        break;
    }
    case U8X8_MSG_BYTE_SEND:
        // 4. Добавляем в очередь массив байт (картинку или команды)
        // arg_ptr - это указатель на данные, arg_int - их длина.
        i2c_master_write(handle, (uint8_t *)arg_ptr, arg_int, true);
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        // 5. Добавляем сигнал STOP
        i2c_master_stop(handle);
        
        // 6. Выполняем всю собранную очередь физически! Ждем ответа до 1000 мс.
        i2c_master_cmd_begin(I2C_PORT, handle, pdMS_TO_TICKS(1000));
        
        // 7. Уничтожаем очередь, освобождая память
        i2c_cmd_link_delete(handle);
    break;
    default:
         break;
    }

    return 1;
}

uint8_t gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    // U8X8_MSG_GPIO_INIT
    // U8X8_MSG_DELAY_MILLI
    // U8X8_MSG_DELAY_10MICRO
    // U8X8_MSG_DELAY_100NANO
    // U8X8_MSG_DELAY_NANO
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        /* code */
        break;
    case U8X8_MSG_DELAY_MILLI:
        vTaskDelay(pdMS_TO_TICKS(arg_int));
        break;
    case U8X8_MSG_DELAY_10MICRO:
        esp_rom_delay_us(10);
        break;
    case U8X8_MSG_DELAY_100NANO:
        esp_rom_delay_us(1);
        break;    
    case U8X8_MSG_DELAY_NANO:
        /* code */
        break;
    default:
        break;
    }

    return 1;
}

extern "C" void app_main(void)
{
    u8g2_t display;

    i2c_master_init();
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&display, U8G2_R0, write_byte, gpio_and_delay);  // init u8g2 structure
    u8g2_InitDisplay(&display); // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&display, 0); // wake up display

    uint8_t start_height_point = 20;
    // uint8_t start_wigth_point = 0;

    u8g2_SetFont(&display, u8g2_font_ncenB14_tr); // Выбираем шрифт
    while (true)
    {
        u8g2_ClearBuffer(&display); // Очищаем внутреннюю память
        u8g2_DrawStr(&display, 0, start_height_point, "Hello, World"); // Пишем текст
        u8g2_SendBuffer(&display); // Отправляем всё это физически на экран
        if (start_height_point < SSD1306_HEIGHT + 14 + 2)
        {
            start_height_point++;
        }
        else {
            start_height_point = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MILIS));
    }
}