#ifndef EPAPERDISPLAY_H
#define EPAPERDISPLAY_H


#include <Arduino.h>


class EPaperDisplay {
    public:
        static const uint16_t X = 792;
        static const uint16_t Y = 272;
        static const uint8_t SOURCE_BYTES = 50; // 400 bits per SSD1683 row divided by 8
        static const uint16_t GATE_BITS = 272; // number of SSD1683 columns (equal to vertical resolution)
        static const uint16_t TOTAL_SCREEN_BYTES = 13600; // source bytes x gate bits
        EPaperDisplay();
        void begin();
        void end();
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
        void _busyHold();
        void _setRAMWindowPri();
        void _setRAMCursorPri();
        void _setRAMWindowSec();
        void _setRAMCursorSec();
        void _fillRedRAM();
};


#endif