#ifndef EPAPERDISPLAY_H
#define EPAPERDISPLAY_H


#include <Arduino.h>


class EPaperDisplay {
    public:
        EPaperDisplay(uint16_t x, uint16_t y);
        uint16_t getXResolution();
        void setXResolution(uint16_t x);
        uint16_t getYResolution();
        void setYResolution(uint16_t y);

        void begin();
        void busyHold();
        void update();
        void hwReset();
        void partialUpdate();
        void fastUpdate();
        void deepSleep();
        void fastModeInit();
        void setRAMWindowPri();
        void setRAMCursorPri();
        void setRAMWindowSec();
        void setRAMCursorSec();
        void fillRedRAM();
        void clear();
        void display(const uint8_t *ImageBW);
        void fastInvert(const unsigned char *data);
    private:
        uint16_t _x;
        uint16_t _y;
        uint16_t _sourceBytes;
        uint16_t _gateBytes;
        uint32_t _totalScreenBytes;
        void _setSourceBytes(uint16_t numberOfBytes);
        void _setGateBytes(uint16_t numberOfBytes);
};


#endif