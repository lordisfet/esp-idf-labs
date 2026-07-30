#include "Encoder.h"

void IRAM_ATTR Encoder::isr_handler(void* arg) 
{
   
}

Encoder::Encoder(gpio_num_t clk, gpio_num_t dt)
{   
    pcnt_unit_config_t pcnt_unit_cfg = {
        .low_limit = -180,
        .high_limit = 180,
        .intr_priority = 0,
    };
    pcnt_new_unit(&pcnt_unit_cfg, &_pcnt_unit);

    pcnt_chan_config_t pcnt_chan_a_cfg = {
        .edge_gpio_num = clk,
        .level_gpio_num = dt,
    };
    pcnt_new_channel(_pcnt_unit, &pcnt_chan_a_cfg, &_pcnt_channel_clk);

    pcnt_channel_set_edge_action(_pcnt_channel_clk, PCNT_CHANNEL_EDGE_ACTION_DECREASE, 
        PCNT_CHANNEL_EDGE_ACTION_INCREASE);
    pcnt_channel_set_level_action(_pcnt_channel_clk, PCNT_CHANNEL_LEVEL_ACTION_KEEP, 
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE);
}