#pragma once

#include <stdio.h>

#include "esp_log.h"
#include "esp_adc/adc_continuous.h"
#include <etl/vector.h>

class ADC
{
private:
    adc_continuous_handle_t _handle = nullptr;
    uint8_t* _buffer;
    uint32_t _max_length_byte;
    uint32_t _out_length_byte;
    uint32_t _timeout_ms;
    uint32_t _max_adc_value;

    adc_digi_output_data_t* _raw = nullptr;
    uint32_t _num_samples;

    void parse_adc_buffer();

public:
    ADC(adc_continuous_handle_cfg_t adc_config, 
        adc_continuous_config_t continuous_config, uint8_t* buffer, uint32_t max_length_byte, 
        uint32_t timeout_ms, uint8_t bitwidth = ADC_BITWIDTH_12) : 
        _buffer(buffer), _max_length_byte(max_length_byte), _timeout_ms(timeout_ms) 
        {
            _raw = (adc_digi_output_data_t*)malloc(_max_length_byte);
            _max_adc_value = pow(2, bitwidth) - 1;
            ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &_handle));
            ESP_ERROR_CHECK(adc_continuous_config(_handle, &continuous_config));
            ESP_ERROR_CHECK(adc_continuous_start(_handle));
        };

    uint32_t get_max_adc_value() {return _max_adc_value;}

    ~ADC()
    {
        if (_handle) {
            adc_continuous_stop(_handle);
            adc_continuous_deinit(_handle);
            _handle = nullptr;
        }
        if (_raw) {
            free(_raw);
            _raw = nullptr;
        }
    }
    
    uint32_t get_filtered_raw(uint8_t unit, uint8_t channel);
};
