#include "services/AppRegistry.h"

#include "services/apps/DiagnosticApp.h"
#include "services/apps/UptimeApp.h"

// #include <string.h>


namespace {
    DiagnosticApp diagnosticApp;
    UptimeApp uptimeApp;

    App* const registeredApps[] = {
        &diagnosticApp,
        &uptimeApp,
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