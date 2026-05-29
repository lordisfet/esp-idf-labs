#include "esp_adc/adc_oneshot.h"
#include "soc/gpio_struct.h"
#include "soc/adc_channel.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

template <size_t WINDOW_SIZE>
class SMA
{
private:
    int _buffer[WINDOW_SIZE];
    size_t _index = 0;
    bool is_buffer_filled = false;

public:
    SMA()
    {
        for (size_t i = 0; i < WINDOW_SIZE; ++i)
        {
            _buffer[i] = 0;
        }
    };

    void add(int value)
    {
        if (!is_buffer_filled && _index == WINDOW_SIZE - 1)
        {
            is_buffer_filled = true;
        }

        _buffer[_index] = value;
        _index = (_index + 1) % WINDOW_SIZE;
    }

    /**
     * @brief Get the average of the values in the buffer
     *
     * @return The average of the values in the buffer.
     */
    double get_average() const
    {
        int sum = 0;
        for (size_t i = 0; i < WINDOW_SIZE; ++i)
        {
            sum += _buffer[i];
        }

        return static_cast<double>(sum) / (is_buffer_filled ? WINDOW_SIZE : _index + 1);
    }
};