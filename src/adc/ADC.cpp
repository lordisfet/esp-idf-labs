#include "ADC.h"

void ADC::parse_adc_buffer(){
    adc_continuous_read(_handle, _buffer, _max_length_byte, &_out_length_byte, _timeout_ms);
    _num_samples = _out_length_byte / SOC_ADC_DIGI_RESULT_BYTES;
    for (uint32_t i = 0; i < _num_samples; i++) {
        memcpy(&_raw[i], _buffer + i * SOC_ADC_DIGI_RESULT_BYTES, sizeof(adc_digi_output_data_t));
    }
}

uint32_t ADC::get_filtered_raw(uint8_t unit, uint8_t channel) {
    parse_adc_buffer();
    uint32_t sum = 0;
    uint8_t current_samples = 0;
    for (uint32_t i = 0; i < _num_samples; i++) {
        if (unit == _raw[i].type2.unit && channel == _raw[i].type2.channel)
        {
            sum += _raw[i].type2.data;
            current_samples++;
        }
    }

    return sum/current_samples;
}