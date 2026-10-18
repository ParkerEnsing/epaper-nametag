#include "services/AppRegistry.h"

#include "services/apps/MainMenuApp.h"
#include "services/apps/LauncherApp.h"
#include "services/apps/DiagnosticApp.h"
#include "services/apps/UptimeApp.h"
#include "services/apps/NametagApp.h"
#include "services/apps/FileBrowserApp.h"
#include "services/apps/SettingsApp.h"
#include "services/apps/DisplayTestApp.h"
#include "services/apps/SplashApp.h"

// #include <string.h>


namespace {
    MainMenuApp mainMenuApp;
    LauncherApp launcherApp;
    DiagnosticApp diagnosticApp;
    UptimeApp uptimeApp;
    NametagApp nametagApp;
    FileBrowserApp fileBrowserApp;
    SettingsApp settingsApp;
    DisplayTestApp displayTestApp;
    SplashApp splashApp;

    App* const registeredApps[] = {
        &mainMenuApp,
        &launcherApp,
        &diagnosticApp,
        &uptimeApp,
        &nametagApp,
        &fileBrowserApp,
        &settingsApp,
        &displayTestApp,
        &splashApp,
    };

    constexpr size_t registeredAppCount = sizeof(registeredApps) / sizeof(registeredApps[0]);
}


void AppRegistry::begin() {}


App* AppRegistry::getById(const char* appId) {
    if (appId == nullptr) {
        return nullptr;
    }

    for (size_t i = 0; i < registeredAppCount; i++) {
        App* app = registeredApps[i];

        if (app == nullptr) {
            continue;
        }

        if (strcmp(app->id(), appId) == 0) {
            return app;
        }
    }

    return nullptr;
}


App* AppRegistry::getByIndex(size_t index) {
    if (index >= registeredAppCount) {
        return nullptr;
    }

    return registeredApps[index];
}


size_t AppRegistry::count() {
    return registeredAppCount;
}


int AppRegistry::indexOf(const char* appId) {
    if (appId == nullptr) {
        return -1;
    }

    for (size_t i = 0; i < registeredAppCount; i++) {
        App* app = registeredApps[i];

        if (app == nullptr) {
            continue;
        }

        if (strcmp(app->id(), appId) == 0) {
            return static_cast<int>(i);
        }
    }

    return -1;
}