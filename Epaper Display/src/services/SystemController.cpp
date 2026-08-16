#include "services/SystemController.h"

#include "services/App.h"
#include "services/AppContext.h"
#include "services/AppRegistry.h"

#include <Arduino.h>
#include <lvgl.h>


namespace {
    static constexpr const char* DEFAULT_HOME_APP_ID = "diagnostic";

    AppContext appContext;

    App* currentApp = nullptr;
    lv_obj_t* loadedScreen = nullptr;
    SystemMode currentMode = SystemMode::Home;

    const char* configuredHomeAppID = DEFAULT_HOME_APP_ID;

    bool commitRequested = false;


    void requestCommit() {
        commitRequested = true;
    }


    void consumeCommitRequest() {
        if (appContext.isCommitRequested()) {
            requestCommit();
            appContext.clearCommitRequest();
        }
    }


    bool loadAppScreen(App* app) {
        if (app == nullptr) {
            return false;
        }

        lv_obj_t* nextScreen = app->screen();

        if (nextScreen == nullptr) {
            Serial.println("SystemController: app returned null screen.");
            currentMode = SystemMode::Error;
            return false;
        }

        if (loadedScreen != nextScreen) {
            lv_screen_load(nextScreen);
            loadedScreen = nextScreen;
        }

        return true;
    }


    bool activateApp(const char* appId, SystemMode nextMode) {
        if (appId == nullptr) {
            Serial.println("SystemController: null app ID.");
            currentMode = SystemMode::Error;
            return false;
        }

        App* nextApp = AppRegistry::getById(appId);

        if (nextApp == nullptr) {
            Serial.print("SystemController: app not found: ");
            Serial.println(appId);
            currentMode = SystemMode::Error;
            return false;
        }

        if (currentApp != nullptr && currentApp != nextApp) {
            appContext.clear();
            currentApp->onExit(appContext);
        }

        /*
            For now, ignore AppContext requests made during onExit().
            Later, if apps need to save state or request actions during
            exit, process those requests here.
        */

        appContext.clear();

        currentApp = nextApp;
        currentMode = nextMode;

        currentApp->onEnter(appContext);

        if (!loadAppScreen(currentApp)) {
            return false;
        }

        requestCommit();
        consumeCommitRequest();

        Serial.print("SystemController: active app = ");
        Serial.println(currentApp->name());

        return true;
    }


    void handleCommand(const AppCommand &command) {
        switch (command.type) {
            case AppCommandType::LaunchApp:
                if (command.appId != nullptr) {
                    SystemController::launchApp(command.appId);
                }
                else {
                    Serial.println("SystemController: LaunchApp command missing app ID.");
                    currentMode = SystemMode::Error;
                }
                break;
            case AppCommandType::GoHome:
                SystemController::goHome();
                break;
            case AppCommandType::OpenLauncher:
                SystemController::openLauncher();
                break;
            case AppCommandType::StartSlideshow:
                SystemController::startSlideshow();
                break;
            case AppCommandType::StopSlideshow:
                SystemController::stopSlideshow();
                break;
            case AppCommandType::None:
            default:
                break;
        }
    }


    void processAppContext() {
        if (appContext.hasCommand()) {
            AppCommand command = appContext.command();
            appContext.clearCommand();

            handleCommand(command);
        }

        consumeCommitRequest(); // why isn't this in the if statement?
    }
}


void SystemController::begin() {
    AppRegistry::begin();

    currentApp = nullptr;
    loadedScreen = nullptr;
    currentMode = SystemMode::Home;
    commitRequested = false;

    appContext.clear();

    goHome();
}


void SystemController::update(uint32_t nowMs) {
    if (currentApp == nullptr) {
        return;
    }

    currentApp->onUpdate(appContext, nowMs);

    processAppContext();
}


void SystemController::handleInput(const InputEvent &event) {
    if (currentApp == nullptr) {
        return;
    }

    currentApp->onInput(appContext, event);

    processAppContext();
}


bool SystemController::launchApp(const char* appId) {
    return activateApp(appId, SystemMode::AppRunning);
}


bool SystemController::goHome() {
    return activateApp(configuredHomeAppID, SystemMode::Home);
}


// TODO: replace with launchApp("launcher") after launcher app exists
void SystemController::openLauncher() {
    Serial.println("SystemController:launcher not implemented yet.");
}


// TODO: implement slideshow mode after the basic app launch path is stable.
void SystemController::startSlideshow() {
    Serial.println("SystemController: slideshow not implemented yet.");
}


// TODO: implement slideshow mode after the basic app launch path is stable.
void SystemController::stopSlideshow() {
    Serial.println("SystemController: stop slideshow not implemented yet.");
}


bool SystemController::setHomeAppId(const char* appId) {
    if (appId == nullptr) {
        return false;
    }

    if (AppRegistry::getById(appId) == nullptr) {
        Serial.print("SystemController: cannot set unknown home app: ");
        Serial.println(appId);
        return false;
    }

    configuredHomeAppID = appId;
    return true;
}


const char* SystemController::homeAppId() {
    return configuredHomeAppID;
}


bool SystemController::shouldCommit() {
    return commitRequested;
}


void SystemController::clearCommitRequest() {
    commitRequested = false;
}


SystemMode SystemController::mode() {
    return currentMode;
}


App* SystemController::activeApp() {
    return currentApp;
}