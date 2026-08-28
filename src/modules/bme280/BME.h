#pragma once

#include <stdio.h>

#include "driver/i2c.h"
#include "Registers.h"
#include "bme280.h"
#include "bme280_defs.h"

class BME280
{
private:
    static constexpr uint16_t WAIT_TIME_MS = 1000;  
    const i2c_port_t _PORT;
    const uint8_t _ADDRESS;
    bme280_dev _dev;
    bme280_settings _sett;

    static BME280_INTF_RET_TYPE read_cb(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
    static BME280_INTF_RET_TYPE write_cb(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
    static void delay_cb(uint32_t period, void *intf_ptr);

public:
    BME280(i2c_port_t port, uint8_t address);

    void init();
    void set_settings(bme280_settings &sett) {_sett = sett;}

    bme280_data get_telemetry();
};

