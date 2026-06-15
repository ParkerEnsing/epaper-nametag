#include <Arduino.h>
#include <lvgl.h>

#include "assets/test_images/pic_home.h"
#include "EPaperDisplay.h"
#include "LVGLEPaperAdapter.h"

const int LV_BUFFER_DIVISIONS = 1;

const int LV_WIDTH_PX  = 792;
const int LV_HEIGHT_PX = 272;

EPaperDisplay EPD;

uint32_t lv_buf[LV_WIDTH_PX * LV_HEIGHT_PX / 8 / LV_BUFFER_DIVISIONS + 8];

#if LV_USE_LOG != 0
void lv_print_log_cb(
    lv_log_level_t level,
    const char* buf)
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

static void lv_rounder_cb(lv_event_t* e) {
    lv_area_t* area = lv_event_get_invalidated_area(e);
    area->x1 &= ~0x7;
    area->x2 |=  0x7;
}

static uint32_t system_tick(void) {
    return millis();
}

void setup() {
    Serial.begin(115200);
    Serial.print("PSRAM found: ");
    Serial.println(psramFound() ? "yes" : "no");

    Serial.print("PSRAM size: ");
    Serial.println(ESP.getPsramSize());

    EPD.begin();

    LVGL_EPAPER_ADAPTER::init(&EPD);

    Serial.print("LVGL ");
    Serial.print(lv_version_major());
    Serial.print(".");
    Serial.print(lv_version_minor());
    Serial.print(".");
    Serial.println(lv_version_patch());

    lv_init();

    lv_tick_set_cb(system_tick);

#if LV_USE_LOG != 0
    lv_log_register_print_cb(
        lv_print_log_cb
    );
#endif

    lv_display_t* disp = lv_display_create(LV_WIDTH_PX, LV_HEIGHT_PX);

    lv_display_set_color_format(disp, LV_COLOR_FORMAT_I1);
    lv_display_set_flush_cb(disp, LVGL_EPAPER_ADAPTER::flush_cb);
    lv_display_set_buffers(disp, lv_buf, NULL, sizeof(lv_buf), LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_add_event_cb(disp, lv_rounder_cb, LV_EVENT_INVALIDATE_AREA, disp);

    lv_obj_t* label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Good morning, Parker");
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(lv_screen_active(), LV_OPA_COVER, LV_PART_MAIN);

    /*
        Force initial render.
    */
    lv_timer_handler();

    /*
        Push framebuffer to display.
    */
    lv_refr_now(disp);
    LVGL_EPAPER_ADAPTER::refreshDisplay();

    Serial.println("Setup complete");
}

void loop()
{
    lv_timer_handler();

    delay(5);
}