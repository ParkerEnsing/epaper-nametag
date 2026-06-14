#ifndef PRIMITIVE_GRAPHICS_H
#define PRIMITIVE_GRAPHICS_H


#include <cstdint>


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

#endif