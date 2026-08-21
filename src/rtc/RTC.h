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
} _raw_data;
#pragma pack(pop)

class RTC
{
private:
    i2c_port_t _port;
    const uint8_t _address = 0x68;

    static const RTCDateTime SETUP_TIME;
    static constexpr uint16_t WAIT_TIME = 1000;

    RTCDateTime get_setup_time();
    RTCDateTime decode_from_hardware(RTCDateTimeRegistred time);
    RTCDateTimeRegistred encode_to_hardware(RTCDateTime time); 
    uint8_t calculate_day_of_week(uint16_t year, uint8_t month, uint8_t day);

    void write_data(RTCDateTimeRegistred time);
    RTCDateTimeRegistred read_data();

    void set_time(RTCDateTime time);
    RTCDateTime get_time();
    
public:
    RTC(i2c_port_t port) : _port(port) {
        
    };

    //TODO: add SETUP_TIME in build
    void set_time(RTCDateTime time) {
        RTCDateTimeRegistred raw_time = encode_to_hardware(time);
        write_data(raw_time);
    }

    RTCDateTime get_time() {
        RTCDateTimeRegistred raw_time = read_data();
        return decode_from_hardware(raw_time);
    }

    //TODO: render info, send data to OLED-display
};

