#pragma once

#include <stdio.h>

#include "driver/i2c.h"
#include "Registers.h"

struct BME280Data {
    float temperature;
    float pressure;
    float humidity; 
};

class BME280
{
private:
    const i2c_port_t _PORT;
    const uint8_t _ADDRESS;

    Calibration _calib;
    Config _cfg;
    CtrlMeas _ctrl_meas;
    CtrlHum _ctrl_hum;

    BME280DataRaw read_data();
    BME280Data decode_from_hardware(BME280DataRaw data);

public:
    BME280(i2c_port_t port, uint8_t address, Config cfg) : _PORT(port), _ADDRESS(address), _cfg(cfg) {};
   
    void init();

    BME280Data get_telemetry();
};

