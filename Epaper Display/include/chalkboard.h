#ifndef CHALKBOARD_H
#define CHALKBOARD_H

#include <scribe.h>
#include <primitiveGraphics.h>


class Canvas {
    public:
        enum Orientation {landscape = 0, portrait = 90, landscape_flipped = 180, portrait_flipped = 270};
        Canvas(uint8_t *canvasBuffer, uint16_t horizontalResolution, uint16_t verticalResolution, Canvas::Orientation screenOrientation, uint8_t backgroundColor);
        void begin();
        void end();
        void setPixel();
        void drawLine();
        void drawRect();
        void fillRect();
        void drawRoundRect();
        void fillRoundRect();
        void drawCircle();
        void fillCircle();
        void fill();
        void fillRegion();
        void erase();
        void erasePixel();
        void eraseRegion();
        void drawText();
        void drawPicture();
        void invert();
        void setBackgroundColor(uint8_t color);
        uint8_t getBackgroundColor();
    private:
        uint8_t* _canvas;
        uint16_t _x;
        uint16_t _y;
        uint16_t _xMemory;
        uint16_t _yMemory;
        uint16_t _xByte;
        uint16_t _yByte;
        uint8_t _backgroundColor;
        Orientation _orientation;
        Font _font;
};


class Viewport {
    public:
        Viewport();
        void begin();
        void end();
    private:
        int _x;
        int _y;
};

#endif