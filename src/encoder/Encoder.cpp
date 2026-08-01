#include "Encoder.h"

#define PCNT_LOW_LIMIT -4
#define PCNT_HIGH_LIMIT 4
#define PCNT_INTR_PRIORITY 0
#define PCNT_MAX_GLITCH_TIME_NS 5000

bool Encoder::isr_handle(pcnt_unit_handle_t unit, 
    const pcnt_watch_event_data_t *edata, void *user_ctx) 
{
    Encoder* enc = static_cast<Encoder*>(user_ctx);
    if (edata->watch_point_value == PCNT_HIGH_LIMIT)
    {
        enc->_steps.fetch_add(1);
    }
    else if (edata->watch_point_value == PCNT_LOW_LIMIT)
    {
        enc->_steps.fetch_sub(1);
    }

    return false;
}

Encoder::Encoder(gpio_num_t clk, gpio_num_t dt)
{   
    pcnt_unit_config_t pcnt_unit_cfg = {
        .low_limit = PCNT_LOW_LIMIT,
        .high_limit = PCNT_HIGH_LIMIT,
        .intr_priority = PCNT_INTR_PRIORITY,
    };
    pcnt_new_unit(&pcnt_unit_cfg, &_unit);

    pcnt_chan_config_t pcnt_chan_a_cfg = {
        .edge_gpio_num = clk,
        .level_gpio_num = dt,
    };
    pcnt_new_channel(_unit, &pcnt_chan_a_cfg, &_channel_clk);

    pcnt_chan_config_t pcnt_chan_b_cfg = {
        .edge_gpio_num = dt,
        .level_gpio_num = clk,
    };
    pcnt_new_channel(_unit, &pcnt_chan_b_cfg, &_channel_dt);

    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = PCNT_MAX_GLITCH_TIME_NS,
    };
    pcnt_unit_set_glitch_filter(_unit, &filter_config);

    pcnt_channel_set_edge_action(_channel_clk, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE);
    pcnt_channel_set_level_action(_channel_clk, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);
    pcnt_channel_set_edge_action(_channel_dt, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE);
    pcnt_channel_set_level_action(_channel_dt, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);

    pcnt_unit_add_watch_point(_unit, PCNT_LOW_LIMIT);
    pcnt_unit_add_watch_point(_unit, PCNT_HIGH_LIMIT);

    pcnt_event_callbacks_t callback = {
        .on_reach = isr_handle
    };
    pcnt_unit_register_event_callbacks(_unit, &callback, this);

    _steps = 0;
    pcnt_unit_enable(_unit);
    pcnt_unit_clear_count(_unit);
    pcnt_unit_start(_unit);
}
