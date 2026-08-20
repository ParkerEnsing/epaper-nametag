#ifndef INPUT_SERVICE_H
#define INPUT_SERVICE_H


#include <stdint.h>
#include "input/InputEvent.h"


namespace InputService {
    void begin();
    void update(uint32_t nowMs);

    // Returns true when an input event is available
    // Call this repeatedly until it returns false
    bool read(InputEvent &event);
}


#endif