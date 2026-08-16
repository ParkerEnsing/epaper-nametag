#include <Arduino.h>

// #include <lvgl.h>
// #include "assets/test_images/pic_home.h"
#include "displays/drivers/EPaperDisplay.h"
#include "displays/ui/UIRuntime.h"
#include "services/SystemController.h"


EPaperDisplay EPD;

void setup() {
    Serial.begin(115200);

    EPD.begin();
    UIRuntime::begin(&EPD);
    SystemController::begin();

    if (SystemController::shouldCommit) {
        UIRuntime::commit();
        SystemController::clearCommitRequest();
    }

    Serial.println("Setup complete.");
}


void loop() {
    const uint32_t nowMs = millis();
    UIRuntime::service();
    SystemController::update(nowMs);

    if (SystemController::shouldCommit()) {
        UIRuntime::commit();
        SystemController::clearCommitRequest();
    }
    
    delay(5);
}