#include "RTC.h"

RTCDateTime RTC::decode_from_hardware(RTCDateTimeRegistgred time){
    RTCDateTime decoded_time;

    decoded_time.seconds = 10 * time.seconds.bits.tens_seconds + time.seconds.bits.units_seconds;
    decoded_time.minutes = 10 * time.minutes.bits.tens_minutes + time.minutes.bits.units_minutes;
    decoded_time.hours = 10 * time.hours.bits.tens_hours + time.hours.bits.units_hours;
    decoded_time.dotw = time.dotw.bits.day;
    decoded_time.day = 10 * time.day.bits.tens_day + time.day.bits.units_day;
    decoded_time.month = 10 * time.month.bits.tens_month + time.month.bits.units_month;
    decoded_time.year = 10 * time.year.bits.tens_year + time.year.bits.units_year;

    return decoded_time;
}

RTCDateTimeRegistgred RTC::encode_to_hardware(RTCDateTime time) {
    RTCDateTimeRegistgred encoded_time;
    
    encoded_time.seconds.bits.tens_seconds = time.seconds / 10;
    encoded_time.seconds.bits.units_seconds = time.seconds % 10;
    encoded_time.seconds.bits.clock_halt = 0;

    encoded_time.minutes.bits.tens_minutes = time.minutes / 10;
    encoded_time.minutes.bits.units_minutes = time.minutes % 10;
    encoded_time.minutes.bits.reserved = 0;

    encoded_time.hours.bits.mode_12_24 = false;
    encoded_time.hours.bits.tens_hours = time.hours / 10;
    encoded_time.hours.bits.units_hours = time.hours % 10;
    encoded_time.hours.bits.reserved = 0;

    encoded_time.dotw.bits.day = time.dotw;
    encoded_time.dotw.bits.reserved = 0;
    
    encoded_time.day.bits.tens_day = time.day / 10;
    encoded_time.day.bits.units_day = time.day % 10;
    encoded_time.day.bits.reserved = 0;

    encoded_time.month.bits.tens_month = time.month / 10;
    encoded_time.month.bits.units_month = time.month % 10;
    encoded_time.month.bits.reserved = 0;

    encoded_time.year.bits.tens_year = time.year / 10;
    encoded_time.year.bits.units_year = time.year % 10;
    
    return encoded_time;
}

uint8_t RTC::calculate_day_of_week(uint16_t year, uint8_t month, uint8_t day) {
    static const uint8_t t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    
    if (month < 3) {
        year -= 1;
    }
    int dow = (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
    
    return (dow == 0) ? 7 : dow;
}

RTCDateTime RTC::get_setup_time() {
    RTCDateTime now = {};

    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char month_str[4];
    int year_full, day, hour, min, sec;
    
    sscanf(__DATE__, "%3s %d %d", month_str, &day, &year_full);
    sscanf(__TIME__, "%d:%d:%d", &hour, &min, &sec);

    now.seconds = sec;
    now.minutes = min;
    now.hours = hour;
    now.day = day;
    for (int i = 0; i < 12; i++) {
        if (strncmp(month_str, months[i], 3) == 0) {
            now.month = i + 1;
            break;
        }
    }
    now.year = year_full % 100;
    now.dotw = calculate_day_of_week(year_full, now.month, now.day);

    return now;
}