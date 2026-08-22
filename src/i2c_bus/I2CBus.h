#pragma once

#include <stdio.h>

#include "driver/i2c.h"
#include "esp_log.h"

#define CLK_SPEED_DEFAUL 400000 

class I2CBus
{
private:
    const i2c_port_t _PORT;
    const i2c_mode_t _MODE;
    const uint8_t _SDA_PIN;
    const uint8_t _SCL_PIN;
    uint32_t _clk_speed;
public:
    I2CBus(i2c_port_t port, i2c_mode_t mode, uint8_t sda, uint8_t scl, 
        uint32_t clk_speed = CLK_SPEED_DEFAUL);
        
    void scan();
};
