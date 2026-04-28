#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define PIN_BUTTON 4

enum ButtonState
{
    IDLE,
    RELEASED,
    DEBOUNCE,
    PRESSED,
};

enum ButtonActiveLevel
{
    ACTIVE_LOW = 0,
    ACTIVE_HIGH = 1,
};

enum ButtonPullMode
{
    PULL_UP = 0,
    PULL_DOWN = 1,
    NO_PULL = 2,
};

class myButton
{
private:
    static constexpr uint16_t _DEBOUNCE_TIME_US = 50000;

    const char *_TAG_BUTTON;
    const enum ButtonActiveLevel _ACTIVE_LEVEL;
    const enum ButtonPullMode _PULL_MODE;
    gpio_num_t _pin;

    void (*_onPress)(void);

    enum ButtonState _internallState;
    int _lastlevel;
    uint64_t _lastlevelSwitchTime;

    esp_err_t init()
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
            ESP_LOGE(_TAG_BUTTON, "Failed to configure GPIO: %d", err);
            return err;
        }

        _lastlevel = gpio_get_level(_pin);
        return ESP_OK;
    }

public:
    myButton(const char *tag, gpio_num_t pin, const enum ButtonActiveLevel activeLevel = ACTIVE_LOW, const enum ButtonPullMode pullMode = PULL_UP,
             void (*onPress)(void) = nullptr)
        : _TAG_BUTTON(tag), _pin(pin), _ACTIVE_LEVEL(activeLevel), _PULL_MODE(pullMode), _onPress(onPress)
    {
        _internallState = IDLE;
        _lastlevelSwitchTime = 0;
    };

    enum ButtonState button_update()
    {
        enum ButtonState stateToReturn = _internallState;
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
                _onPress();
            }

            _lastlevel = currentLevel;
            _internallState = IDLE;
            break;
        case RELEASED:

            _lastlevel = currentLevel;
            _internallState = IDLE;
            break;
        default:
            break;
        }

        return stateToReturn;
    }
};
