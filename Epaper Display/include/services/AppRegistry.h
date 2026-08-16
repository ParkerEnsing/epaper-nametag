#ifndef APP_REGISTRY_H
#define APP_REGISTRY_H

#include "services/App.h"


namespace AppRegistry {
    void begin();
    App* getById(const char* id);
    App* getByIndex(size_t index);
    size_t count();
    int indexOf(const char* appId);
}


#endif