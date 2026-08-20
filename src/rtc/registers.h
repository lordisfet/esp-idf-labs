#pragma once

#include <stdio.h>

union SecondsRegister {
    uint8_t raw;
    struct {
        uint8_t units_seconds : 4;
        uint8_t tens_seconds  : 3;
        uint8_t clock_halt    : 1;
    } bits;
};

union MinutesRegister {
    uint8_t raw;
    struct {
        uint8_t units_minutes : 4;
        uint8_t tens_minutes  : 3;
        uint8_t reserved      : 1;
    } bits;
};

//TODO: Add support of 12-hours and 24-hours time models
union HoursRegister {
    uint8_t raw;
    struct {
        uint8_t units_hours : 4;
        uint8_t tens_hours  : 2;
        uint8_t mode_12_24  : 1;
        uint8_t reserved    : 1;
    } bits;
};

union DaysRegister {
    uint8_t raw;
    struct {
        uint8_t day         : 3;
        uint8_t reserved    : 5;
    } bits;
};

union DateRegister {
    uint8_t raw;
    struct {
        uint8_t units_day  : 4;
        uint8_t tens_day   : 2;
        uint8_t reserved    : 2;    
    } bits;
};

union MonthRegister {
    uint8_t raw;
    struct {
        uint8_t units_month : 4;
        uint8_t tens_month  : 1;
        uint8_t reserved    : 3;
    } bits;
};

union YearRegister {
    uint8_t raw;
    struct {
        uint8_t units_year  : 4;
        uint8_t tens_year   : 4;
    } year;
};