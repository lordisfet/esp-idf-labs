#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_rom_sys.h"

#include <u8g2.h>
#include <u8x8.h>

#include "menu/Menu.h"
#include "i2c_bus/I2CBus.h"
#include "modules/bme280/BME.h"

#define TASK_DELAY_TICKS pdMS_TO_TICKS(1000)
#define SDA_PIN GPIO_NUM_15
#define SCL_PIN GPIO_NUM_2
#define I2C_PORT I2C_NUM_0 
#define SSD1306_HEIGHT 64
#define SSD1306_WIDTH 128

#define BME280_ADDR 0x76
#define RTC_ADDR 0x68

extern "C" void app_main(void)
{
    I2CBus i2c_bus(I2C_PORT, I2C_MODE_MASTER, SDA_PIN, SCL_PIN);
    i2c_bus.scan();

    RTC rtc(I2C_PORT, RTC_ADDR);
    rtc.init();

    BME280 bme(I2C_PORT, BME280_ADDR);
    bme280_settings settings = {
        .osr_p = BME280_OVERSAMPLING_4X,
        .osr_t = BME280_OVERSAMPLING_4X,
        .osr_h = BME280_OVERSAMPLING_4X,
        .filter = BME280_FILTER_COEFF_8,
        .standby_time = BME280_STANDBY_TIME_1000_MS
    };
    bme.set_settings(settings);
    bme.init();

    Display display;

    ScreenLayout layout = {
        .dotw = {20, 15, u8g2_font_profont12_tf},
        .date = {50, 15, u8g2_font_profont12_tf},
        .time = {10, 46, u8g2_font_logisoso24_tn},
        .telemetry = {4, 60, u8g2_font_profont10_tf}
    };
    Menu menu(layout);
    
    while (true)
    {
        RTCDateTime now = rtc.get_time(); 
        bme280_data telemetry = bme.get_telemetry();

        ESP_LOGI("SYSTEM" , "-----------------------");
        ESP_LOGI("RTC", "Current time is %02d:%02d:%02d", now.hours, now.minutes, now.seconds);
        ESP_LOGI("BME280", "Current telemtry are: temperature = %f, humidity = %f, pressure = %f", 
            telemetry.temperature, telemetry.humidity, telemetry.pressure);
            
        menu.render(display, now, telemetry);
        
        vTaskDelay(TASK_DELAY_TICKS);
    }
}