#ifndef EPAPERDISPLAY_H
#define EPAPERDISPLAY_H


#include <Arduino.h>


class EPaperDisplay {
    public:
        EPaperDisplay(uint16_t xResolution, uint16_t yResolution);
        uint16_t getXResolution();
        void setXResolution(uint16_t x);
        uint16_t getYResolution();
        void setYResolution(uint16_t y);
        void begin();
        void display(const uint8_t *ImageBW);
        void update();
        void partialUpdate();
        void fastModeInit();
        void fastUpdate();
        void fastInvert(const unsigned char *data);
        void clear();
        void hwReset();
        void deepSleep();
    private:
        uint16_t _x;
        uint16_t _y;
        uint16_t _sourceBytes;
        uint16_t _gateBytes;
        uint32_t _totalScreenBytes;
        void _setSourceBytes(uint16_t numberOfBytes);
        void _setGateBytes(uint16_t numberOfBytes);
        void _busyHold();
        void _setRAMWindowPri();
        void _setRAMCursorPri();
        void _setRAMWindowSec();
        void _setRAMCursorSec();
        void _fillRedRAM();
};


#endif