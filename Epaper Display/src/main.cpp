#include <Arduino.h>
#include "pic_home.h"
#include "EPaperDisplay.h"
#include <lvgl.h>

const int LV_BUFFER_DIVISIONS = 10;
const int LV_WIDTH_PX = 792;
const int LV_HEIGHT_PX = 272;

uint8_t canvasBuffer[27200];
EPaperDisplay EPD;

uint32_t lv_buf[LV_WIDTH_PX * LV_HEIGHT_PX / LV_BUFFER_DIVISIONS / 8 + 8];


#if LV_USE_LOG != 0
void lv_print_log_cb(lv_log_level_t level, const char* buf) {
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif


static void lv_custom_display_flush(lv_display_t* disp, const lv_area_t* area, uint8_t*  px_map) {
  px_map += 8; // skip the color palette

  uint32_t width  = area->x2 - area->x1 + 1;
  uint32_t height = area->y2 - area->y1 + 1;
  uint16_t bytes_per_row = width >> 3;

  //EPD.wakeUp();
  //EPD.setWindow(
  //   area->x1,
  //   area->y1,
  //   area->x2,
  //   area->y2
  // );

  const uint8_t* buffer = px_map; // alias px_map in case we need it later

  for (uint32_t y = 0; y < height; y++) {
    // EPD.writeData(buffer, bytes_per_row); // send one row of bytes to display RAM
    buffer += bytes_per_row;
  }

  //EPD.refresh();
  //EPD.deepSleep();

  lv_display_flush_ready(disp);
}


static void lv_rounder_cb(lv_event_t* e) {
  lv_area_t* area = lv_event_get_invalidated_area(e);
  area->x1 = (area->x1 & ~0x7);
  area->x2 = (area->x2 | 0x7);
}


static uint32_t system_tick(void) {
  return millis();
}


void setup() {
  Serial.begin(115200);
  EPD.begin();

  // do stuff
  String LVGL_Arduino = "Hello! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Arduino);

  lv_init();
  lv_tick_set_cb(system_tick);

#if LV_USE_LOG != 0
  lv_log_register_print_cb(lv_print_log_cb);
#endif

  lv_display_t* disp;
  disp = lv_display_create(LV_WIDTH_PX, LV_HEIGHT_PX);
  lv_display_set_color_format(disp, LV_COLOR_FORMAT_I1);
  lv_display_set_flush_cb(disp, lv_custom_display_flush);
  lv_display_set_buffers(disp, lv_buf, NULL, sizeof(lv_buf), LV_DISPLAY_RENDER_MODE_FULL);
  lv_display_add_event_cb(disp, lv_rounder_cb, LV_EVENT_INVALIDATE_AREA, disp); // ensures redrawn areas are restricted to byte coordinates

  Serial.println("Setup complete");
  delay(1000);

  lv_obj_t* label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello world");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_black(), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}


void loop() {
  lv_timer_handler();
  delay(5);
}