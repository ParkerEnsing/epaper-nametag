#ifndef APP_REGISTRY_H
#define APP_REGISTRY_H

#include "services/App.h"


namespace AppRegistry {
    void begin();
    App* getById(const char* id);
    App* getByIndex(uint8_t index);
    uint8_t count();
}


#endif