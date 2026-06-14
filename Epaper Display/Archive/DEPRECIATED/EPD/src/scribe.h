#ifndef SCRIBE_H
#define SCRIBE_H

#include <cstdint>

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