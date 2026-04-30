#include "MyButton.h"

esp_err_t MyButton::init()
{
    const gpio_config_t configButton =
        {
            .pin_bit_mask = 1ULL << _pin,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = _PULL_MODE == PULL_UP ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
            .pull_down_en = _PULL_MODE == PULL_DOWN ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE};

    esp_err_t err = gpio_config(&configButton);
    if (err != ESP_OK)
    {
        ESP_LOGE(_TAG, "Failed to configure GPIO pin %d: %s", _pin, esp_err_to_name(err));
        return err;
    }

    _lastlevel = gpio_get_level(_pin);
    return ESP_OK;
}

void MyButton::update()
{
    int currentLevel = gpio_get_level(_pin);
    uint64_t currentTime = esp_timer_get_time();

    switch (_internallState)
    {
    case IDLE:
        if (currentLevel != _lastlevel)
        {
            _lastlevelSwitchTime = currentTime;
            _internallState = DEBOUNCE;
        }
        break;
    case DEBOUNCE:
        if (currentTime - _lastlevelSwitchTime >= _DEBOUNCE_TIME_US && currentLevel != _lastlevel)
        {
            if (currentLevel == _ACTIVE_LEVEL)
            {
                _internallState = PRESSED;
            }
            else
            {
                _internallState = RELEASED;
            }
        }
        break;
    case PRESSED:
        if (_onPress != nullptr)
        {
            _onPress(_argForPress);
            ESP_LOGI(_TAG, "Button on pin %d pressed", _pin);
        }

        _lastlevel = currentLevel;
        _internallState = IDLE;
        break;
    case RELEASED:
        _lastlevel = currentLevel;
        _internallState = IDLE;
        ESP_LOGI(_TAG, "Button on pin %d released", _pin);
        break;
    default:
        break;
    }
}