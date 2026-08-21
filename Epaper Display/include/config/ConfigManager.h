#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H


#include "config/DeviceConfig.h"


namespace ConfigManager {
    void begin();

    const DeviceConfig &current();
    DeviceConfig &editable();
    void loadDefaults(DeviceConfig &config);
}


#endif