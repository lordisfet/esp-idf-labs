#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"

#include "u8g2/u8g2.h"

#define SDA_PIN GPIO_NUM_15
#define SCL_PIN GPIO_NUM_2

#define tag "SSD1306"

void i2c_master_init()
{
	i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 15,
        .scl_io_num = 2,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
    };

	conf.master.clk_speed = 400000;
	i2c_param_config(I2C_NUM_0, &conf);
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

uint8_t write_byte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {

}

extern "C" void app_main(void)
{
    u8g2_t display;
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&display, U8G2_R0, )
	i2c_master_init();

}