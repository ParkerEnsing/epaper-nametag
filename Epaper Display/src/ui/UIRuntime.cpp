#include "ui/UIRuntime.h"
#include "LVGLEPaperAdapter.h"
#include "ui/UITheme.h"
#include <Arduino.h>
#include <lvgl.h>


namespace {
    lv_display_t* uiDisplay = nullptr;

    /*
    Divides total area into bytes.
    Those bytes are divided up by the specified buffer divisions.
    Finally, 8 bytes are added to account for the unused color palette
    */
    LV_ATTRIBUTE_MEM_ALIGN static uint8_t lvDrawBuffer[(EPaperDisplay::X * EPaperDisplay::Y / 8) + 8];

    uint32_t system_tick() {
        return millis();
    }
}


void UIRuntime::begin(EPaperDisplay* display) {
    lv_init();

    lv_tick_set_cb(system_tick);

    LVGL_EPAPER_ADAPTER::init(display);

    uiDisplay = lv_display_create( EPaperDisplay::X, EPaperDisplay::Y);
    lv_display_set_color_format( uiDisplay, LV_COLOR_FORMAT_I1);
    lv_display_set_flush_cb( uiDisplay, LVGL_EPAPER_ADAPTER::flush_cb);
    lv_display_set_buffers(uiDisplay, lvDrawBuffer, nullptr, sizeof(lvDrawBuffer), LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_add_event_cb(uiDisplay, LVGL_EPAPER_ADAPTER::rounder_cb, LV_EVENT_INVALIDATE_AREA, uiDisplay);

    UITheme::init();
}


void UIRuntime::service() {
    lv_timer_handler();
}


void UIRuntime::commit() {
    if(uiDisplay == nullptr) {
        return;
    }

    lv_refr_now(uiDisplay);
    LVGL_EPAPER_ADAPTER::refreshDisplay();
}


lv_display_t* UIRuntime::display() {
    return uiDisplay;
}