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

public:
    myButton(const char *tag, gpio_num_t pin, const enum ButtonActiveLevel activeLevel = ACTIVE_LOW, const enum ButtonPullMode pullMode = PULL_UP,
             void (*onPress)(void) = nullptr)
        : _TAG_BUTTON(tag), _pin(pin), _ACTIVE_LEVEL(activeLevel), _PULL_MODE(pullMode), _onPress(onPress)
    {
        _internallState = IDLE;
        _lastlevelSwitchTime = 0;
    };

    esp_err_t init();
    enum ButtonState button_update();
};
