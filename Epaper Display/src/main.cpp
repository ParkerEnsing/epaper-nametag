#include <Arduino.h>

// #include <lvgl.h>
// #include "assets/test_images/pic_home.h"
#include "displays/drivers/EPaperDisplay.h"
#include "displays/ui/UIRuntime.h"

#include "services/App.h"
#include "services/AppContext.h"
#include "services/AppRegistry.h"


EPaperDisplay EPD;

static AppContext appContext;
static App* activeApp = nullptr;
static lv_obj_t* loadedScreen = nullptr;

static constexpr const char* STARTUP_APP_ID = "uptime";
// static constexpr const char* STARTUP_APP_ID = "diagnostic";


static void processAppContext() {
    if (activeApp == nullptr) {
        return;
    }

    if (appContext.isCommitRequested()) {
        lv_obj_t* appScreen = activeApp->screen();
        if (appScreen != nullptr && loadedScreen != appScreen) {
            lv_screen_load(appScreen);
            loadedScreen = appScreen;
        }
        UIRuntime::commit();
        appContext.clearCommitRequest();
    }
}


void setup() {
    Serial.begin(115200);

    EPD.begin();
    UIRuntime::begin(&EPD);
    AppRegistry::begin();

    activeApp = AppRegistry::getById(STARTUP_APP_ID);

    if (activeApp == nullptr) {
        Serial.println("Failed to find the startup app.");
        return;
    }

    Serial.print("Launching app: ");
    Serial.println(activeApp->name());

    activeApp->onEnter(appContext);
    processAppContext();

    Serial.println("Setup complete.");
}


void loop() {
    const uint32_t nowMs = millis();
    UIRuntime::service();

    if (activeApp != nullptr) {
        activeApp->onUpdate(appContext, nowMs);
        processAppContext();
    }
    
    delay(5);
}