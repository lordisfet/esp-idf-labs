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
struct RTCDateTimeRegistgred {
    SecondsRegister seconds;
    MinutesRegister minutes;
    HoursRegister   hours;
    DaysRegister    dotw;
    DateRegister    day;
    MonthRegister   month;
    YearRegister    year;
} _raw_data;
#pragma pack(pop)

class RTC
{
private:
    i2c_port_t _port;
    const uint8_t _address = 0x68;
    static const RTCDateTime _setup_time;

    RTCDateTime get_setup_time(); 
    uint8_t calculate_day_of_week(uint16_t year, uint8_t month, uint8_t day);
    RTCDateTimeRegistgred code_t(RTCDateTime time);

public:
    RTC(i2c_port_t port) : _port(port) {
        get_setup_time();
    };

    
    void set_time_now() {
        
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();

        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (_address << 1) | 0x7F, true);

    }
};

