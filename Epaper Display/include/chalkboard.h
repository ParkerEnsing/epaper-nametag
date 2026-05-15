#ifndef CHALKBOARD_H
#define CHALKBOARD_H

#include <cstdint>


class Canvas {
    public:
        enum Orientation {landscape = 0, portrait = 90, landscape_flipped = 180, portrait_flipped = 270};
        Canvas();
        void begin();
        void initialize();
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
    private:
        uint8_t *_canvas;
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


class Point {
    public:
        Point();
    private:
        uint16_t _x;
        uint16_t _y;
};


class Shape: public Point {
    public:
        enum AnchorPosition {
            topLeft,
            topMiddle,
            topRight,
            centerLeft,
            centerMiddle,
            centerRight,
            bottomLeft,
            bottomMiddle,
            bottomRight
        };
        Shape();
    private:
        uint16_t _length;
        uint16_t _width;
        bool _isFilled;
        uint8_t _color;
        uint8_t _strokeWidth;
        uint8_t _anchorPosition;
};


class Line: public Shape {
    public:
        Line();
    private:
        Point _start;
        Point _end;
};


class Rectangle: public Shape {
    public:
        Rectangle();
    private:
        Point _topLeftCorner;
        Point _bottomRightCorner;
};


class Circle: public Shape {
    public:
        Circle();
    private:
        uint16_t _radius;
};


class Arc: public Shape {
    public:
        Arc();
    private:
        uint16_t _radius;
        uint16_t _arcLength;
};


class RoundedRect: public Rectangle {
    public:
        RoundedRect();
    private:
        uint8_t _cornerRadius;
};


class Font {
    public:
        Font();
        void setTextSize();
        void setColor();
    private:
    uint8_t _size;    
    uint8_t _color;
};

#endif