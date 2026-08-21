#include "config/ConfigManager.h"

#include <stdio.h>
#include <string.h>


namespace {
    DeviceConfig activeConfig;

    void copyAppId(char* destination, size_t destinationSize, const char* source) {
        if(destination == nullptr || destinationSize == 0) {
            return;
        }

        if (source == nullptr) {
            destination[0] = '\0';
            return;
        }

        snprintf(destination, destinationSize, "%s", source);
    }


    bool addBootStep(DeviceConfig &config, const char* appId, uint32_t durationMs) {
        if (config.bootSequenceCount >= CONFIG_MAX_BOOT_STEPS) {
            return false;
        }

        ConfigSequenceStep &step = config.bootSequence[config.bootSequenceCount];
        copyAppId(step.appId, sizeof(step.appId), appId);

        step.durationMs = durationMs;

        config.bootSequenceCount++;
        
        return true;
    }


    bool addSlideshowApp(DeviceConfig &config, const char* appId) {
        if (config.slideshowAppCount >= CONFIG_MAX_SLIDESHOW_APPS) {
            return false;
        }

        copyAppId(
            config.slideshowAppIds[config.slideshowAppCount],
            sizeof(config.slideshowAppIds[config.slideshowAppCount]),
            appId
        );

        config.slideshowAppCount++;

        return true;
    }
}


void ConfigManager::begin() {
    loadDefaults(activeConfig);
}


const DeviceConfig& ConfigManager::current() {
    return activeConfig;
}


DeviceConfig& ConfigManager::editable() {
    return activeConfig;
}


// TODO: update with actual defaults later once they're decided
void ConfigManager::loadDefaults(DeviceConfig &config) {
    memset(&config, 0, sizeof(config));

    copyAppId(config.homeAppId, sizeof(config.homeAppId), "diagnostic");

    addBootStep(config, "diagnostic", 3000);
    addBootStep(config, "uptime", 3000);

    config.slideshowIntervalMs = 5000;
    addSlideshowApp(config, "diagnostic");
    addSlideshowApp(config, "uptime");
    addSlideshowApp(config, "launcher");
}