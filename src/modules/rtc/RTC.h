#pragma once

#include <stdio.h>
#include "driver/i2c.h"
#include "etl/string.h"

#include "Registers.h"

struct RTCDateTime {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t dotw;
    uint8_t day;
    uint8_t month;
    uint8_t year;
};

class RTC
{
private:
    const i2c_port_t _PORT;
    const uint8_t _ADDRESS;

    static const RTCDateTime SETUP_TIME;
    static constexpr uint16_t WAIT_TIME = 1000;

    RTCDateTime get_setup_time();
    uint8_t calculate_day_of_week(uint16_t year, uint8_t month, uint8_t day);

    void write_data(RTCDateTImeRaw time);
    RTCDateTImeRaw read_data();

    RTCDateTime decode_from_hardware(RTCDateTImeRaw time);
    RTCDateTImeRaw encode_to_hardware(RTCDateTime time); 
public:
    RTC(i2c_port_t port, uint8_t address) : _PORT(port), _ADDRESS(address) {};

    void init() { set_time(get_setup_time()); }
    
    void set_time(RTCDateTime time);
    RTCDateTime get_time();
};
