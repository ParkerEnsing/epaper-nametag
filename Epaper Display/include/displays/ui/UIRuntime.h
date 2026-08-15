#ifndef UI_RUNTIME_H
#define UI_RUNTIME_H

#include <lvgl.h>
#include "displays/drivers/EPaperDisplay.h"


namespace UIRuntime {
    void begin(EPaperDisplay* display);
    void service();
    void commit();
    lv_display_t* display();
}

#endif