#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include <stddef.h>
#include <stdint.h>

static constexpr size_t CONFIG_APP_ID_LENGTH = 32;
static constexpr size_t CONFIG_MAX_BOOT_STEPS = 8;
static constexpr size_t CONFIG_MAX_SLIDESHOW_APPS = 8;

struct ConfigSequenceStep {
    char appId[CONFIG_APP_ID_LENGTH];
    uint32_t durationMs;
};

struct DeviceConfig {
    char homeAppId[CONFIG_APP_ID_LENGTH];

    ConfigSequenceStep bootSequence[CONFIG_MAX_BOOT_STEPS];
    size_t bootSequenceCount;

    char slideshowAppIds[CONFIG_MAX_SLIDESHOW_APPS][CONFIG_APP_ID_LENGTH];
    size_t slideshowAppCount;

    uint32_t slideshowIntervalMs;
};

#endif