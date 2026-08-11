#include <Arduino.h>
//#include <lvgl.h>
//#include "assets/test_images/pic_home.h"
#include "EPaperDisplay.h"
#include "ui/UIRuntime.h"
#include "ui/UITheme.h"
#include "ui/screens/OrgSplashScreen.h"
#include "ui/screens/ProductSplashScreen.h"
#include "ui/screens/BootScreen.h"


EPaperDisplay EPD;


void setup() {
    Serial.begin(115200);
    Serial.print("PSRAM Check: ");
    Serial.println(psramFound() ? "PASS" : "FAIL");

    Serial.print("PSRAM SIZE: ");
    Serial.print(ESP.getPsramSize());
    Serial.println(" Bytes");

    EPD.begin();
    UIRuntime::begin(&EPD);

    lv_screen_load(OrgSplashScreen::create());
    UIRuntime::commit();
    delay(1000);

    lv_screen_load(ProductSplashScreen::create());
    UIRuntime::commit();
    delay(1000);

    lv_screen_load(BootScreen::create());
    UIRuntime::commit();
    delay(5000);

    EPD.end();
}


void loop() {
    UIRuntime::service();
    delay(5);
}