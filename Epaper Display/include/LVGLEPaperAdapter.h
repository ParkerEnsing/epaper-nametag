#ifndef LVGLEPAPERADAPTER_H
#define LVGLEPAPERADAPTER_H


#include "EPaperDisplay.h"
#include <lvgl.h>


class LVGL_EPAPER_ADAPTER {
    public:
        static void init(EPaperDisplay* display);
        static void flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);
        static void refreshDisplay();
    private:
        static EPaperDisplay* _display;
        static uint8_t _framebuffer[EPaperDisplay::EPaperDisplay::Y * EPaperDisplay::SOURCE_BYTES * 2];
};

#endif