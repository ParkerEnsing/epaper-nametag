#include "LVGLEPaperAdapter.h"
#include <cstring>

/*
    LVGL uses 792 visible horizontal pixels.

    The physical panel memory is 800 pixels wide because the two SSD1683
    controllers each expose 400 horizontal pixels. The 8 hidden pixels are
    located at the center seam, 4 hidden pixels per controller.

    Therefore the LVGL byte covering x = 392..399 must be split across
    two physical panel bytes.
*/

// 1 px = 1 bit, 1 byte = 8 pxs
static constexpr uint16_t PANEL_ROW_BYTES = EPaperDisplay::SOURCE_BYTES * 2; // 100 bytes
static constexpr uint16_t PANEL_WIDTH_PX = PANEL_ROW_BYTES * 8; // 800 bits
static constexpr uint16_t VISIBLE_WIDTH_PX = EPaperDisplay::X; // 792 bits
static constexpr uint16_t HIDDEN_WIDTH_PX = PANEL_WIDTH_PX - VISIBLE_WIDTH_PX; // 8 bits
static constexpr uint16_t HIDDEN_PER_CHIP_PX = HIDDEN_WIDTH_PX >> 1; // 4 bits
static constexpr uint16_t VISIBLE_LEFT_WIDTH_PX = VISIBLE_WIDTH_PX >> 1; // 396 bits

static constexpr uint16_t VISIBLE_ROW_BYTES = VISIBLE_WIDTH_PX >> 3; // 99 bytes
static constexpr uint16_t SEAM_SRC_BYTE_INDEX = VISIBLE_LEFT_WIDTH_PX >> 3; // 49 px (technically 49.5 but it gets truncated intentionally)
static constexpr uint16_t SEAM_SRC_X_PX = SEAM_SRC_BYTE_INDEX << 3; // 392 px
static constexpr uint16_t LEFT_PANEL_SEAM_BYTE_INDEX = SEAM_SRC_BYTE_INDEX; // 49th byte
static constexpr uint16_t RIGHT_PANEL_SEAM_BYTE_INDEX = SEAM_SRC_BYTE_INDEX + 1; // 50th byte
static constexpr uint8_t UPPER_NIBBLE_MASK = 0xF0;
static constexpr uint8_t LOWER_NIBBLE_MASK = 0x0F;


static void copy_lvgl_row_to_epd_row(uint8_t* dstRow, const uint8_t* srcRow, uint16_t startX, uint16_t widthPx) {
    const uint16_t srcBytes = widthPx >> 3;

    for(uint16_t i = 0; i < srcBytes; i++) {
        const uint16_t x = startX + i * 8;
        const uint8_t src = srcRow[i];

        if(x < SEAM_SRC_X_PX) {
            // Entire byte is on the left side, before the seam
            dstRow[x >> 3] = src;
        }
        else if(x == SEAM_SRC_X_PX) {
            /*
                Split the byte that crosses the visible center seam.
                LVGL byte:
                    src bits 7..4 = left visible pixels
                    src bits 3..0 = right visible pixels

                Panel bytes:
                    dst[49] bits 7..4 = left visible pixels
                    dst[49] bits 3..0 = hidden left-side pixels, forced white

                    dst[50] bits 7..4 = hidden right-side pixels, forced white
                    dst[50] bits 3..0 = right visible pixels
            */
            dstRow[LEFT_PANEL_SEAM_BYTE_INDEX] = (src & UPPER_NIBBLE_MASK) | LOWER_NIBBLE_MASK;
            dstRow[RIGHT_PANEL_SEAM_BYTE_INDEX] = UPPER_NIBBLE_MASK | (src & LOWER_NIBBLE_MASK);
        }
        else {
            /*
                Bytes after the seam shift right by one physical byte
                because the 800-pixel panel memory contains one extra
                hidden byte compared to LVGL's 792-pixel visible space.
            */
            dstRow[(x >> 3) + 1] = src;
        }
    }
}


EPaperDisplay* LVGL_EPAPER_ADAPTER::_display = nullptr;

uint8_t LVGL_EPAPER_ADAPTER::_framebuffer[EPaperDisplay::Y * EPaperDisplay::SOURCE_BYTES * 2] = {0xFF};

bool LVGL_EPAPER_ADAPTER::_isFramebufferDirty = false;


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
        const uint32_t dstRowIndex = area->y1 + row;
        uint8_t* dstRow = &_framebuffer[dstRowIndex * PANEL_ROW_BYTES];
        const uint8_t* srcRow = px_map + row * srcBytesPerRow;
        copy_lvgl_row_to_epd_row(dstRow, srcRow, area->x1, width);
    }
    _isFramebufferDirty = true;
    lv_display_flush_ready(disp);
}


void LVGL_EPAPER_ADAPTER::rounder_cb(lv_event_t* e) {
    lv_area_t* area = lv_event_get_invalidated_area(e);
    area->x1 &= ~0x7;
    area->x2 |=  0x7;
}


void LVGL_EPAPER_ADAPTER::refreshDisplay() {
    if (_display == nullptr) {
        return;
    }
    if (!_isFramebufferDirty) {
        return;
    }
    _display->render(_framebuffer);
    _isFramebufferDirty = false;
}


void LVGL_EPAPER_ADAPTER::invertDisplay() {
    if (_display == nullptr) {
        return;
    }
    _display->fastInvert(_framebuffer);
}


void LVGL_EPAPER_ADAPTER::commitUI(lv_display_t* disp) {
    lv_refr_now(disp);
    refreshDisplay();
}


bool LVGL_EPAPER_ADAPTER::isDirty() {
    return _isFramebufferDirty;
}