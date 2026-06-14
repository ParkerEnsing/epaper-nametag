#include "LVGLEPaperAdapter.h"
#include <cstring>

EPaperDisplay* LVGL_EPAPER_ADAPTER::_display = nullptr;

uint8_t LVGL_EPAPER_ADAPTER::_framebuffer[EPaperDisplay::Y * EPaperDisplay::SOURCE_BYTES * 2] = {0xFF};

void LVGL_EPAPER_ADAPTER::init(EPaperDisplay* display) {
    _display = display;

    // initialize framebuffer to white
    memset(_framebuffer, 0xFF, sizeof(_framebuffer));
}

void LVGL_EPAPER_ADAPTER::flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
    /*
        LVGL I1 buffers contain an 8-byte palette/header.
        Current LVGL examples skip it.
        Verify against your LVGL version.
    */
    px_map += 8;

    const uint32_t width = area->x2 - area->x1 + 1;
    const uint32_t height = area->y2 - area->y1 + 1;
    const uint32_t srcBytesPerRow = width >> 3;

    for(uint32_t row = 0; row < height; row++) {
        const uint32_t dstRow = area->y1 + row;
        uint8_t* dst = &_framebuffer[dstRow * 100 + (area->x1 >> 3)];
        const uint8_t* src = px_map + row * srcBytesPerRow;
        memcpy(dst, src, srcBytesPerRow);
    }
    lv_display_flush_ready(disp);
}

void LVGL_EPAPER_ADAPTER::refreshDisplay() {
    if (_display == nullptr) {
        return;
    }

    _display->display(_framebuffer);
    _display->update();
}