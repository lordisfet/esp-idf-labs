#include "Display.h"

Display::Display(const u8g2_cb_t* rotate, const char* TAG) : _TAG(TAG) {
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&_u8g2, rotate, write_byte, gpio_and_delay);
    u8g2_InitDisplay(&_u8g2); 
    u8g2_SetPowerSave(&_u8g2, 0);
}

uint8_t Display::write_byte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
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
        i2c_master_cmd_begin(PORT, handle, pdMS_TO_TICKS(1000));
        
        // 7. Уничтожаем очередь, освобождая память
        i2c_cmd_link_delete(handle);
    break;
    default:
         break;
    }

    return 1;
}

uint8_t Display::gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
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