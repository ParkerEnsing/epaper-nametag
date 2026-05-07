#include "EPaperDisplay.h"

#define SSD1683_COUNT 2
#define SSD1683_X 400
#define SSD1683_Y 300

#define WHITE 0xFF
#define BLACK 0x00


EPaperDisplay::EPaperDisplay(uint16_t xResolution, uint16_t yResolution) {
    setXResolution(xResolution);
    setYResolution(yResolution);
}


uint16_t EPaperDisplay::getXResolution() {
    return _x;
}


void EPaperDisplay::setXResolution(uint16_t x) {
    _x = x;
    if (x < SSD1683_X * SSD1683_COUNT) {
        _setSourceBytes(x / SSD1683_COUNT / 8);
    } else {
        _setSourceBytes(SSD1683_X * SSD1683_COUNT / 8);
    }
}


uint16_t EPaperDisplay::getYResolution() {
    return _y;
}


void EPaperDisplay::setYResolution(uint16_t y) {
    _y = y;
    if (y < SSD1683_Y) {
        _setGateBytes(y);
    } else {
        _setGateBytes(SSD1683_Y);
    }
}


void EPaperDisplay::begin() {

}


void EPaperDisplay::display(const uint8_t *ImageBW) {

}


void EPaperDisplay::update() {

}


void EPaperDisplay::partialUpdate() {

}


void EPaperDisplay::fastModeInit() {

}


void EPaperDisplay::fastUpdate() {

}


void EPaperDisplay::fastInvert(const unsigned char *data) {

}


void EPaperDisplay::clear() {

}


void EPaperDisplay::hwReset() {

}


void EPaperDisplay::deepSleep() {

}


void EPaperDisplay::_setSourceBytes(uint16_t numberOfBytes) {
    _sourceBytes = numberOfBytes;
    _totalScreenBytes = _sourceBytes * _gateBytes;
}


void EPaperDisplay::_setGateBytes(uint16_t numberOfBytes) {
    _gateBytes = numberOfBytes;
    _totalScreenBytes = _sourceBytes * _gateBytes;
}


void EPaperDisplay::_busyHold() {

}


void EPaperDisplay::_setRAMWindowPri() {

}


void EPaperDisplay::_setRAMWindowSec() {
    
}


void EPaperDisplay::_setRAMCursorPri() {
    
}


void EPaperDisplay::_setRAMCursorSec() {
    
}