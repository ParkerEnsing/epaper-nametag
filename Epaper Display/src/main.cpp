#include <Arduino.h>

// #include <lvgl.h>
// #include "assets/test_images/pic_home.h"
#include "displays/drivers/EPaperDisplay.h"
#include "displays/ui/UIRuntime.h"

// #include "displays/ui/screens/OrgSplashScreen.h"
// #include "displays/ui/screens/ProductSplashScreen.h"
// #include "displays/ui/screens/BootScreen.h"
// #include "displays/ui/screens/NametagScreen.h"

#include "services/AppContext.h"
#include "services/apps/DiagnosticApp.h"
#include "services/apps/UptimeApp.h"


EPaperDisplay EPD;

static AppContext appContext;
static UptimeApp uptimeApp;
// static DiagnosticApp diagnosticApp;

static App* activeApp = &uptimeApp;
// static App* activeApp = &diagnosticApp;


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

    // lv_screen_load(OrgSplashScreen::create());
    // UIRuntime::commit();
    // delay(1000);

    // lv_screen_load(ProductSplashScreen::create());
    // UIRuntime::commit();
    // delay(1000);

    // lv_screen_load(BootScreen::create());
    // UIRuntime::commit();

    // lv_screen_load(NametagScreen::create());
    // UIRuntime::commit();
    // delay(5000);

    // EPD.end();
}


void loop() {
    // UIRuntime::service();
    // delay(5);
    const uint32_t nowMs = millis();
    UIRuntime::service();

    activeApp->onUpdate(appContext, nowMs);
    processAppContext();

    delay(5);
}