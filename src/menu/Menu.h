#pragma once

#include <stdio.h>

#include "u8g2.h"
#include "display/Display.h"
#include "rtc/RTC.h"
#include "bme280.h"

#define BUZZER_SIZE 64
#define PaToMmHg(x) (x / 113.322)

struct UIElement {
    uint8_t x;
    uint8_t y;
    const uint8_t* font;
};

// union ScreenLayout {
//     struct {
//         UIElement time;
//         UIElement dotw;
//         UIElement date;
//         UIElement telemetry;
//     } bits;
//     uint8_t raw[sizeof(bits)];
// };

struct ScreenLayout
{
    UIElement dotw;
    UIElement date;
    UIElement time;
    UIElement telemetry;
};


class Menu
{
private:
    const char* _TAG = "MENU";
    ScreenLayout _layout;
public:
    Menu(ScreenLayout layout) : _layout(layout) {};
    
    void render(Display& display, RTCDateTime& time, bme280_data& tel)
    {
        char buffer[BUZZER_SIZE];

        display.clear();

        const char* days[] = {"N/A", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
        uint8_t d_index = (time.dotw >= 1 && time.dotw <= 7) ? time.dotw : 0;
        display.draw_text(_layout.dotw.x, _layout.dotw.y, days[d_index], _layout.dotw.font);
        ESP_LOGI(_TAG, "Rendered dotw is: %s", days[d_index]);

        sprintf(buffer, "%02d-%02d-20%02d", time.day, time.month, time.year);
        display.draw_text(_layout.date.x, _layout.date.y, buffer, _layout.date.font);
        ESP_LOGI(_TAG, "Rendered date is: %s", buffer);
        
        sprintf(buffer, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
        display.draw_text(_layout.time.x, _layout.time.y, buffer, _layout.time.font);
        ESP_LOGI(_TAG, "Rendered time is: %s", buffer);

        sprintf(buffer, "T: %.1fC H: %.0f%% P: %.0fmm", tel.temperature, tel.humidity, PaToMmHg(tel.pressure));
        display.draw_text(_layout.telemetry.x, _layout.telemetry.y, buffer, _layout.telemetry.font);
        ESP_LOGI(_TAG, "Rendered telemetry is: %s", buffer);

        display.update();
    }
};

