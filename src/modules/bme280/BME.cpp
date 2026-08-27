#include "BME.h"

#include "rom/ets_sys.h"

BME280::BME280(i2c_port_t port, uint8_t address) : _PORT(port), _ADDRESS(address) {
    _dev.intf = BME280_I2C_INTF;
    _dev.intf_ptr = this;

    _dev.read = read_cb;
    _dev.write = write_cb;
    _dev.delay_us = delay_cb;
}

void BME280::init() {
    bme280_init(&_dev);
    bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &_sett, &_dev);
    bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &_dev);
}

BME280_INTF_RET_TYPE BME280::read_cb(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    BME280* sensor = static_cast<BME280*>(intf_ptr);

    esp_err_t err = i2c_master_write_read_device(sensor->_PORT, sensor->_ADDRESS, 
        &reg_addr, 1, reg_data, len, pdMS_TO_TICKS(WAIT_TIME_MS));

    return (err == ESP_OK) ? BME280_OK : BME280_E_COMM_FAIL;
}

BME280_INTF_RET_TYPE BME280::write_cb(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    BME280* sensor = static_cast<BME280*>(intf_ptr);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (sensor->_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    if (len > 0)
    {
        i2c_master_write(cmd, reg_data, len, true);
    }
    i2c_master_stop(cmd);
    
    esp_err_t err = i2c_master_cmd_begin(sensor->_PORT, cmd, pdMS_TO_TICKS(WAIT_TIME_MS));
    i2c_cmd_link_delete(cmd);

    return (err == ESP_OK) ? BME280_OK : BME280_E_COMM_FAIL;
}

void BME280::delay_cb(uint32_t period, void *intf_ptr) {
    ets_delay_us(period);
}

bme280_data BME280::get_telemetry() {
    bme280_data data;

    bme280_get_sensor_data(BME280_ALL, &data, &_dev);

    return data;
}
