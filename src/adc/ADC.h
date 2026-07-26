#pragma once

#include <stdio.h>

#include "esp_adc/adc_continuous.h"
#include <etl/vector.h>

class ADC
{
private:
    adc_continuous_handle_t _handle;
    etl::ivector<uint8_t>& _buffer;
    uint32_t _out_length;
    uint32_t _timeout_ms;

public:
    ADC(adc_continuous_handle_t handle, etl::ivector<uint8_t>& buffer, uint32_t timeout_ms) : 
        _handle(handle), _buffer(buffer), _timeout_ms(timeout_ms) {};
    ~ADC();
};
