#include "Encoder.h"

void IRAM_ATTR Encoder::isr_handler(void* arg) 
{
    Encoder* enc = static_cast<Encoder*>(arg);
    
    uint8_t clk = gpio_get_level(enc->_clk);
    uint8_t dt = gpio_get_level(enc->_dt);

    BaseType_t high_task_wakeup = pdFALSE; 

    switch (enc->_state)
    {
    case IDLE:
        if (clk == 1 && dt == 0) {
            enc->_state = RIGHT_STEP;
        }
        else if (clk == 0 && dt == 1) {
            enc->_state = LEFT_STEP;
        }
        break;

    case RIGHT_STEP:
        if (clk == 0 && dt == 0)
        {
            uint64_t now = esp_timer_get_time();
            
            if (now - enc->_last_step_time > MIN_STEP_DELAY_US) 
            {
                enc->_steps = enc->_steps + 1;
                enc->_last_step_time = now;
                
                int current = enc->_steps;
                xQueueSendFromISR(enc->step_queue, &current, &high_task_wakeup);
            }
            enc->_state = IDLE;
        }
        else if (clk == 1 && dt == 1) { enc->_state = RELEASE; }
        break;

    case LEFT_STEP:
        if (clk == 0 && dt == 0)
        {
            uint64_t now = esp_timer_get_time();

            if (now - enc->_last_step_time > MIN_STEP_DELAY_US)
            {
                enc->_steps = enc->_steps - 1;
                enc->_last_step_time = now;
                
                int current = enc->_steps;
                xQueueSendFromISR(enc->step_queue, &current, &high_task_wakeup);
            }
            enc->_state = IDLE;
        }
        else if (clk == 1 && dt == 1) { enc->_state = RELEASE; }
        break;

        case RELEASE:
        if (clk == 1 && dt == 1) 
        {
            enc->_state = IDLE;
        }
        break;

    default:
        enc->_state = IDLE;
        break;
    }

    if (high_task_wakeup) {
        portYIELD_FROM_ISR();
    }
}

Encoder::Encoder(gpio_num_t clk, gpio_num_t dt) 
    : _clk(clk), _dt(dt)
{   
    step_queue = xQueueCreate(20, sizeof(int));

    gpio_config_t cfg = {};
    cfg.pin_bit_mask = (1ULL << _clk) | (1ULL << _dt); 
    cfg.mode = GPIO_MODE_INPUT;
    cfg.pull_up_en = GPIO_PULLUP_ENABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_ANYEDGE; 
    gpio_config(&cfg);

    _steps = 0;
    _state = IDLE;

    gpio_isr_handler_add(_clk, isr_handler, (void*)this);
    gpio_isr_handler_add(_dt, isr_handler, (void*)this);
}