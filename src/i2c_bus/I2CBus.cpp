#include "I2CBus.h"


I2CBus::I2CBus(i2c_port_t port, i2c_mode_t mode, uint8_t sda, uint8_t scl, uint32_t clk_speed) 
: _PORT(port), _MODE(mode), _SDA_PIN(sda), _SCL_PIN(scl), _clk_speed(clk_speed) {
    i2c_config_t conf = {};

    conf.mode = _MODE;
    conf.sda_io_num = _SDA_PIN;
    conf.scl_io_num = _SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = _clk_speed;

    i2c_param_config(_PORT, &conf);
    i2c_driver_install(_PORT, _MODE, 0, 0, 0);
}
void I2CBus::scan() {
    ESP_LOGI("I2C_SCAN", "Starting I2C bus scan...");

    int devices_found = 0;
    for (uint8_t address = 1; address < 127; address++) {

        // Создаем короткую "пингующую" транзакцию
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);

        i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);

        esp_err_t err = i2c_master_cmd_begin(_PORT, cmd, pdMS_TO_TICKS(10));
        i2c_cmd_link_delete(cmd);

        if (err == ESP_OK) {
            ESP_LOGI("I2C_SCAN", "Found device at address: 0x%02X", address);
            devices_found++;
        }
    }
    if (devices_found == 0) {
        ESP_LOGE("I2C_SCAN", "No I2C devices found! Check wiring (SDA/SCL) and power.");
    } else {
        ESP_LOGI("I2C_SCAN", "Scan complete. Total devices: %d", devices_found);
    }
}