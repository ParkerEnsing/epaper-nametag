#include <Arduino.h>
//#include <lvgl.h>
//#include "assets/test_images/pic_home.h"
#include "EPaperDisplay.h"
#include "ui/UIRuntime.h"
#include "ui/UITheme.h"


EPaperDisplay EPD;


void setup() {
    Serial.begin(115200);
    Serial.print("PSRAM Check: ");
    Serial.println(psramFound() ? "PASS" : "FAIL");

    Serial.print("PSRAM SIZE: ");
    Serial.print(ESP.getPsramSize());
    Serial.println("Bytes");

    EPD.begin();
    UIRuntime::begin(&EPD);

    lv_obj_t* screen = lv_obj_create(nullptr);
    lv_obj_add_style(screen, UITheme::screen(), LV_PART_MAIN);

    lv_obj_t* label = lv_label_create(screen);
    lv_label_set_text(label, "Hello world");
    lv_obj_add_style(label, UITheme::titleLabel(), LV_PART_MAIN);
    lv_obj_center(label);
    
    lv_screen_load(screen);

    UIRuntime::commit();
    
    delay(5000);
    EPD.end();
}


void loop() {
    UIRuntime::service();
    delay(5);
}