#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_rom_sys.h"

#include <u8g2.h>
#include <u8x8.h>

#include "menu/Menu.h"
#include "i2c_bus/I2CBus.h"

#define TASK_DELAY_TICKS pdMS_TO_TICKS(1000)
#define SDA_PIN GPIO_NUM_15
#define SCL_PIN GPIO_NUM_2
#define I2C_PORT I2C_NUM_0 
#define SSD1306_HEIGHT 64
#define SSD1306_WIDTH 128

extern "C" void app_main(void)
{
    I2CBus i2c_bus(I2C_PORT, I2C_MODE_MASTER, SDA_PIN, SCL_PIN);
    i2c_bus.scan();

    RTC rtc;
    rtc.init();

    Display display;

    ScreenLayout layout = {
        .time = {10, 35, u8g2_font_logisoso24_tn},
        .dotw = {20,  55, u8g2_font_profont12_tf},
        .date = {50, 55, u8g2_font_profont12_tf}
    };
    Menu menu(layout);
    
    while (true)
    {
        RTCDateTime now = rtc.get_time(); 
        menu.render(display, now);
        ESP_LOGI("RTC", "Current time is %02d:%02d:%02d", now.hours, now.minutes, now.seconds);
        vTaskDelay(TASK_DELAY_TICKS);
    }
}