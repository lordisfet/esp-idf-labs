#pragma once

#include <stdio.h>
#include "driver/i2c.h"
#include "u8g2.h"

class Display
{
private:
    const static i2c_port_t PORT = I2C_NUM_0;
    u8g2_t _u8g2;
    const char* _TAG;

    static uint8_t write_byte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
    static uint8_t gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
public:
    Display(const u8g2_cb_t* rotate = U8G2_R0, const char* TAG = "DISPLAY");

    void clear() { u8g2_ClearBuffer(&_u8g2); }
    void update() { u8g2_SendBuffer(&_u8g2); }
    void draw_text(uint8_t x, uint8_t y, const char* text, const uint8_t* font) {
        u8g2_SetFont(&_u8g2, font);
        u8g2_DrawStr(&_u8g2, x, y, text);
    }
};

