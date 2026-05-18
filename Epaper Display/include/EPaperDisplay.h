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
        static const uint32_t SPI_MAX_SPEED = 20000000; // Hz
        static const uint8_t SPI_DATA_ORDER = MSBFIRST;
        static const uint8_t SPI_DATA_MODE = SPI_MODE3;
        EPaperDisplay();
        void begin();
        void end();
        void initializeGPIO();
        void hwReset();
        void clear();
        void initialize();
        void initializeFastMode();
        void display(const uint8_t *imageAddress);
        void update();
        void partialUpdate();
        void fastUpdate();
        void render(const uint8_t *imageAddress);
        void renderAndSleep(const uint8_t *imageAddress);
        void fastRender(const uint8_t *imageAddress);
        void fastRenderAndSleep(const uint8_t *imageAddress);
        void partialRender(const uint8_t *imageAddress);
        void partialRenderAndSleep(const uint8_t *imageAddress);
        void fastInvert(const unsigned char *data);
        void deepSleep();
    private:
        void _writeByte(uint8_t byteToWrite);
        void _writeCommand(uint8_t commandByte);
        void _writeData(uint8_t dataByte);
        void _busyHold();
        void _setRAMWindowPri();
        void _setRAMCursorPri();
        void _setRAMWindowSec();
        void _setRAMCursorSec();
        void _fillRedRAM();
};

#endif