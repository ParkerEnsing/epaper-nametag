#include "chalkboard.h"


#define WHITE 0xFF
#define BLACK 0x00


Canvas::Canvas(uint8_t *canvasBuffer, uint16_t horizontalResolution, uint16_t verticalResolution, Canvas::Orientation screenOrientation, uint8_t backgroundColor):
    _canvas(canvasBuffer),
    _xMemory(horizontalResolution),
    _yMemory(verticalResolution),
    _backgroundColor(backgroundColor),
    _orientation(screenOrientation) {
        if (_orientation == Canvas::landscape || _orientation == Canvas::landscape_flipped) {
            _x = horizontalResolution;
            _y = verticalResolution;
        } else {
            _x = verticalResolution;
            _y = horizontalResolution;
        }
        _xByte = (horizontalResolution % 8 == 0) ? horizontalResolution / 8 : horizontalResolution / 8 + 1;
        _yByte = verticalResolution;
        // assign default font
}


void Canvas::begin() {
    erase();
}


void Canvas::end() {}


void Canvas::setPixel() {}


void Canvas::drawLine() {}


void Canvas::drawRect() {}


void Canvas::fillRect() {}


void Canvas::drawRoundRect() {}


void Canvas::fillRoundRect() {}


void Canvas::drawCircle() {}


void Canvas::fillCircle() {}


void Canvas::fill() {}


void Canvas::fillRegion() {}


void Canvas::erase() {
    for (int y = 0; y < _yByte; y++) {
        for (int x = 0; x < _xByte; x++) {
            _canvas[x + y * _xByte] = _backgroundColor;
        }
    }
}


void Canvas::erasePixel() {}


void Canvas::eraseRegion() {}


void Canvas::drawText() {}


void Canvas::drawPicture() {}


void Canvas::invert() {}


void Canvas::setBackgroundColor(uint8_t color) {
    _backgroundColor = color;
}


uint8_t Canvas::getBackgroundColor() {
    return _backgroundColor;
}