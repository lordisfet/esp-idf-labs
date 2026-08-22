#pragma once

#include <stdio.h>

#include "u8g2.h"
#include "display/Display.h"
#include "rtc/RTC.h"

struct UIElement {
    uint8_t x;
    uint8_t y;
    const uint8_t* font;
};

struct ScreenLayout {
    UIElement time;
    UIElement dotw;
    UIElement date;
};

class Menu
{
private:
    ScreenLayout _layout;
public:
    Menu(ScreenLayout layout) : _layout(layout) {};
    
    void render(Display& display, RTCDateTime& time) {
        char buffer[16];

        display.clear();
        
        sprintf(buffer, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
        display.draw_text(_layout.time.x, _layout.time.y, buffer, _layout.time.font);

        sprintf(buffer, "%02d-%02d-20%02d", time.day, time.month, time.year);
        display.draw_text(_layout.date.x, _layout.date.y, buffer, _layout.date.font);

        const char* days[] = {"N/A", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
        uint8_t d_index = (time.dotw >= 1 && time.dotw <= 7) ? time.dotw : 0;
        display.draw_text(_layout.dotw.x, _layout.dotw.y, days[d_index], _layout.dotw.font);

        display.update();
    }
};

