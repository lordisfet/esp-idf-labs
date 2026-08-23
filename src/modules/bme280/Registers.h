#pragma once

#include <stdio.h>

#pragma pack(push, 1)

#define REG_RESET 0xE0
#define CMD_RESET 0x00

struct Calibration {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;
};

union CtrlMeas {
    uint8_t raw;
    struct {
        uint8_t mode        : 2;
        uint8_t osrs_p      : 3;
        uint8_t osrs_t      : 3;
    } bits;
};

union CtrlHum {
    uint8_t raw;
    struct {
        uint8_t osrs_h      : 3;
        uint8_t             : 5;
    } bits;
};

union Config {
    uint8_t raw;
    struct {
        uint8_t spi3w_en    : 1;
        uint8_t             : 1;
        uint8_t filter      : 3;
        uint8_t t_sb        : 3; 
    } bits;
};

union BME280DataRaw {
    uint64_t raw;
    struct {
        uint8_t press_msb;
        uint8_t press_lsb;
        uint8_t             : 4;
        uint8_t press_xlsb  : 4;
        uint8_t temp_msb;
        uint8_t temp_lsb;
        uint8_t             : 4;
        uint8_t temp_xlsb   : 4;
        uint8_t hum_msb;
        uint8_t hum_lsb;
    } bits;
};

#pragma pack(pop)
