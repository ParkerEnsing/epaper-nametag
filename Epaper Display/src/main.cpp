#include <Arduino.h>
#include <lvgl.h>

#include "assets/test_images/pic_home.h"
#include "EPaperDisplay.h"
#include "LVGLEPaperAdapter.h"
#include "UITheme.h"

const int LV_BUFFER_DIVISIONS = 1;

EPaperDisplay EPD;

/*
Divides total area into bytes.
Those bytes are divided up by the specified buffer divisions.
Finally, 8 bytes are added to account for the unused color palette
*/
LV_ATTRIBUTE_MEM_ALIGN static uint8_t lv_buf[EPD.X * EPD.Y / 8 / LV_BUFFER_DIVISIONS + 8];


static uint32_t lv_system_tick(void) {
    return millis();
}


void setup() {
    Serial.begin(115200);
    Serial.print("PSRAM Check: ");
    Serial.println(psramFound() ? "PASS" : "FAIL");

    Serial.print("PSRAM SIZE: ");
    Serial.print(ESP.getPsramSize());
    Serial.println("Bytes");

    EPD.begin();

    LVGL_EPAPER_ADAPTER::init(&EPD);

    Serial.print("LVGL ");
    Serial.print(lv_version_major());
    Serial.print(".");
    Serial.print(lv_version_minor());
    Serial.print(".");
    Serial.println(lv_version_patch());

    lv_init();
    lv_tick_set_cb(lv_system_tick);

    lv_display_t* disp = lv_display_create(EPD.X, EPD.Y);

    lv_display_set_color_format(disp, LV_COLOR_FORMAT_I1);
    lv_display_set_flush_cb(disp, LVGL_EPAPER_ADAPTER::flush_cb);
    lv_display_set_buffers(disp, lv_buf, NULL, sizeof(lv_buf), LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_add_event_cb(disp, LVGL_EPAPER_ADAPTER::rounder_cb, LV_EVENT_INVALIDATE_AREA, disp);

    Serial.println("Setup complete");

    UITheme::init();

    lv_obj_t* screen = lv_obj_create(nullptr);
    lv_obj_add_style(screen, UITheme::screen(), LV_PART_MAIN);


    lv_obj_t* label = lv_label_create(screen);
    lv_label_set_text(label, "Hello world");
    lv_obj_add_style(label, UITheme::titleLabel(), LV_PART_MAIN);
    lv_obj_center(label);
    

    lv_screen_load(screen);

    lv_timer_handler();
    LVGL_EPAPER_ADAPTER::commitUI(disp);

    delay(5000);

    EPD.end();
}


void loop() {
    lv_timer_handler();

    delay(5);
}