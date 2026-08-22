#pragma once

#include <stdio.h>
#include "driver/i2c.h"
#include "etl/string.h"

#include "registers.h"

struct RTCDateTime {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t dotw;
    uint8_t day;
    uint8_t month;
    uint8_t year;
};

#pragma pack(push, 1)
struct RTCDateTimeRegistred {
    SecondsRegister seconds;
    MinutesRegister minutes;
    HoursRegister   hours;
    DaysRegister    dotw;
    DateRegister    day;
    MonthRegister   month;
    YearRegister    year;
};
#pragma pack(pop)

class RTC
{
private:
    const i2c_port_t _port;
    const uint8_t _address;

    static const RTCDateTime SETUP_TIME;
    static constexpr uint16_t WAIT_TIME = 1000;

    RTCDateTime get_setup_time();
    uint8_t calculate_day_of_week(uint16_t year, uint8_t month, uint8_t day);

    RTCDateTime decode_from_hardware(RTCDateTimeRegistred time);
    RTCDateTimeRegistred encode_to_hardware(RTCDateTime time); 

    void write_data(RTCDateTimeRegistred time);
    RTCDateTimeRegistred read_data();
public:
    RTC(i2c_port_t port = I2C_NUM_0, uint8_t address = 0x68) : _port(port), _address(address) {};

    void init() { set_time(get_setup_time()); }
    
    void set_time(RTCDateTime time);
    RTCDateTime get_time();
};
