#include <Arduino.h>

// #include <lvgl.h>
// #include "assets/test_images/pic_home.h"
#include "displays/drivers/EPaperDisplay.h"
#include "displays/ui/UIRuntime.h"

#include "services/AppContext.h"
#include "services/apps/DiagnosticApp.h"
#include "services/apps/UptimeApp.h"


EPaperDisplay EPD;

static AppContext appContext;
// static UptimeApp uptimeApp;
static DiagnosticApp diagnosticApp;

// static App* activeApp = &uptimeApp;
static App* activeApp = &diagnosticApp;


static void processAppContext() {
    if (appContext.isCommitRequested()) {
        lv_screen_load(activeApp->screen());
        UIRuntime::commit();
        appContext.clearCommitRequest();
    }
}


void setup() {
    EPD.begin();
    UIRuntime::begin(&EPD);

    activeApp->onEnter(appContext);
    processAppContext();
}


void loop() {
    const uint32_t nowMs = millis();
    UIRuntime::service();

    activeApp->onUpdate(appContext, nowMs);
    processAppContext();

    delay(5);
}