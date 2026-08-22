#pragma once

#include <stdio.h>
#include "driver/i2c.h"
#include "u8g2.h"

class Display
{
private:
    u8g2_t _u8g2;

    // const i2c_port_t _port;
    // const uint8_t _address;
    const char* _TAG;
public:
    Display(u8g2_t& u8g2, const char* TAG = "DISPLAY") : _u8g2(u8g2), _TAG(TAG) {};

    void clear() { u8g2_ClearBuffer(&_u8g2); }
    void update() { u8g2_SendBuffer(&_u8g2); }
    void draw_text(uint8_t x, uint8_t y, const char* text, const uint8_t* font) {
        u8g2_SetFont(&_u8g2, font);
        u8g2_DrawStr(&_u8g2, x, y, text);
    }
};

