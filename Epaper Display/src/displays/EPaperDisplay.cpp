#include "EPaperDisplay.h"
#include "io.h"
#include <SPI.h>

// https://www.elecrow.com/download/product/DIS08792E/SSD1683_Datasheet.PDF


#define readBusy digitalRead(DISP_SPI_BUSY)
#define setCS() digitalWrite(DISP_SPI_CS, HIGH)
#define clearCS() digitalWrite(DISP_SPI_CS, LOW)
#define setDC() digitalWrite(DISP_SPI_DC, HIGH)
#define clearDC() digitalWrite(DISP_SPI_DC, LOW)
#define setRES() digitalWrite(DISP_SPI_RES, HIGH)
#define clearRES() digitalWrite(DISP_SPI_RES, LOW)


EPaperDisplay::EPaperDisplay() {}


void EPaperDisplay::begin() {
    pinMode(DISP_POWER, OUTPUT);
    digitalWrite(DISP_POWER, HIGH);

    pinMode(DISP_SPI_BUSY, INPUT);
    pinMode(DISP_SPI_DC, OUTPUT);
    pinMode(DISP_SPI_RES, OUTPUT);
    SPI.begin(DISP_SPI_SCK, -1, DISP_SPI_COPI, DISP_SPI_CS);
}


void EPaperDisplay::end() {
    digitalWrite(DISP_POWER, LOW);
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


void EPaperDisplay::_writeByte(uint8_t byteToWrite) {
    clearCS();
    SPI.beginTransaction(SPISettings(SPI_MAX_SPEED, SPI_DATA_ORDER, SPI_DATA_MODE));
    SPI.transfer(byteToWrite);
    SPI.endTransaction();
    setCS();
}


void EPaperDisplay::_writeCommand(uint8_t commandByte) {
    clearDC();
    _writeByte(commandByte);
    setDC();
}


void EPaperDisplay::_writeData(uint8_t dataByte) {
    setDC();
    _writeByte(dataByte);
    clearDC();
}


void EPaperDisplay::_busyHold() {
    while (1) {
        if (readBusy == LOW) {
            break;
        }
    }
}


void EPaperDisplay::_setRAMWindowPri() {

}


void EPaperDisplay::_setRAMWindowSec() {
    
}


void EPaperDisplay::_setRAMCursorPri() {
    
}


void EPaperDisplay::_setRAMCursorSec() {
    
}