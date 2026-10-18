#include "services/SystemController.h"

#include "config/DeviceConfig.h"
#include "config/ConfigManager.h"

#include "services/App.h"
#include "services/AppContext.h"
#include "services/AppRegistry.h"

#include "input/InputEvent.h"

#include <Arduino.h>
#include <lvgl.h>
// #include <stdio.h>


namespace {
    AppContext appContext;

    App* currentApp = nullptr;
    lv_obj_t* loadedScreen = nullptr;
    SystemMode currentMode = SystemMode::Home;

    struct SequenceStep {
        const char* appId;
        uint32_t durationMs;
    };

    size_t bootSequenceIndex = 0;
    uint32_t bootStepStartMs = 0;

    bool startBootSequence(uint32_t nowMs);
    bool startBootSequenceStep(size_t index, uint32_t nowMs);
    void updateBootSequence(uint32_t nowMs);

    size_t slideshowSequenceIndex = 0;
    uint32_t slideshowStepStartMs = 0;

    bool activateApp(const char* appId, SystemMode nextMode); // redundant forward declaration. Useful if sequence functions need to move in the future.

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


    bool startBootSequence(uint32_t nowMs) {
        const DeviceConfig &config = ConfigManager::current();
        
        bootSequenceIndex = 0;
        bootStepStartMs = nowMs;

        if (config.bootSequenceCount == 0) {
            return SystemController::goHome();
        }
        return startBootSequenceStep(bootSequenceIndex, nowMs);
    }


    bool startBootSequenceStep(size_t index, uint32_t nowMs) {
        const DeviceConfig &config = ConfigManager::current();
        
        if (index >= config.bootSequenceCount) {
            return SystemController::goHome();
        }

        const ConfigSequenceStep &step = config.bootSequence[index];

        if (step.appId[0] == '\0') {
            Serial.println("SystemController: boot sequence step has empty app ID.");
            currentMode = SystemMode::Error;
            return false;
        }

        bootSequenceIndex = index;
        bootStepStartMs = nowMs;

        return activateApp(step.appId, SystemMode::BootSequence);
    }


    void updateBootSequence(uint32_t nowMs) {
        if (currentMode != SystemMode::BootSequence) {
            return;
        }

        const DeviceConfig &config = ConfigManager::current();

        if (bootSequenceIndex >= config.bootSequenceCount) {
            SystemController::goHome();
            return;
        }

        const ConfigSequenceStep &step = config.bootSequence[bootSequenceIndex];

        if (nowMs - bootStepStartMs < step.durationMs) {
            return;
        }

        const size_t nextIndex = bootSequenceIndex + 1;

        if (nextIndex >= config.bootSequenceCount) {
            SystemController::goHome();
            return;
        }

        startBootSequenceStep(nextIndex, nowMs);
    }


    bool showSlideshowStep(size_t index, uint32_t nowMs) {
        const DeviceConfig &config = ConfigManager::current();

        if (config.slideshowAppCount == 0) {
            Serial.println("SystemController: slideshow has no apps.");
            return SystemController::goHome();
        }

        slideshowSequenceIndex = index % config.slideshowAppCount;
        slideshowStepStartMs = nowMs;

        const char* appId = config.slideshowAppIds[slideshowSequenceIndex];

        if (appId[0] == '\0') {
            Serial.println("SystemController: slideshow step has empty app ID.");
            currentMode = SystemMode::Error;
            return false;
        }

        return activateApp(appId, SystemMode::Slideshow);
    }


    bool nextSlideshowStep(uint32_t nowMs) {
        const DeviceConfig &config = ConfigManager::current();
        
        if (config.slideshowAppCount == 0) {
            return false;
        }

        const size_t nextIndex = (slideshowSequenceIndex + 1) % config.slideshowAppCount;
        return showSlideshowStep(nextIndex, nowMs);
    }


    bool previousSlideshowStep(uint32_t nowMs) {
        const DeviceConfig &config = ConfigManager::current();
        
        if (config.slideshowAppCount == 0) {
            return false;
        }

        const size_t previousIndex = (slideshowSequenceIndex == 0) ? (config.slideshowAppCount - 1) : (slideshowSequenceIndex - 1);
        return showSlideshowStep(previousIndex, nowMs);
    }


    void updateSlideshow(uint32_t nowMs) {
        if (currentMode != SystemMode::Slideshow) {
            return;
        }

        const DeviceConfig &config = ConfigManager::current();

        if (config.slideshowAppCount == 0) {
            SystemController::goHome();
            return;
        }

        const uint32_t intervalMs = config.slideshowIntervalMs;

        if (intervalMs == 0) {
            return;
        }

        if (nowMs - slideshowStepStartMs < intervalMs) {
            return;
        }

        nextSlideshowStep(nowMs);
    }


    bool handleSlideshowInput(const InputEvent &event) {
        switch (event.action) {
            case InputAction::Up:
            case InputAction::Left:
                previousSlideshowStep(millis());
                return true;
            case InputAction::Down:
            case InputAction::Right:
                nextSlideshowStep(millis());
                return true;
            case InputAction::Back:
                SystemController::goHome();
                return true;
            case InputAction::Select:
                // Stop cycling and keep the current app active
                currentMode = SystemMode::AppRunning;
                return true;
            default:
                return false;
        }
    }
}


void SystemController::begin() {
    AppRegistry::begin();

    currentApp = nullptr;
    loadedScreen = nullptr;
    currentMode = SystemMode::BootSequence;
    commitRequested = false;

    bootSequenceIndex = 0;
    bootStepStartMs = 0;

    slideshowSequenceIndex = 0;
    slideshowStepStartMs = 0;

    appContext.clear();

    startBootSequence(millis());
}


void SystemController::update(uint32_t nowMs) {
    if (currentApp != nullptr) {
        currentApp->onUpdate(appContext, nowMs);
        processAppContext();
    }

    updateBootSequence(nowMs);
    updateSlideshow(nowMs);
}


void SystemController::handleInput(const InputEvent &event) {
    // Global/system-level inputs are handled before the active app sees the event.
    switch (event.action) {
        case InputAction::OpenLauncher:
            openLauncher();
            return;
        case InputAction::Home:
            goHome();
            return;
        case InputAction::StartSlideshow:
            startSlideshow();
            return;
        case InputAction::StopSlideshow:
            stopSlideshow();
            return;
        default:
            break;
    }

    switch (currentMode) {
        case SystemMode::Slideshow:
            if (handleSlideshowInput(event)) {
                return;
            }
        default:
            break;
    }

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
    return activateApp(ConfigManager::current().homeAppId, SystemMode::Home);
}


void SystemController::openLauncher() {
    activateApp("launcher", SystemMode::Launcher);
}


void SystemController::startSlideshow() {
    showSlideshowStep(0, millis());
}


void SystemController::stopSlideshow() {
    goHome();
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

    DeviceConfig &config = ConfigManager::editable();
    snprintf(config.homeAppId, sizeof(config.homeAppId), "%s", appId);
    return true;
}


const char* SystemController::homeAppId() {
    return ConfigManager::current().homeAppId;
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