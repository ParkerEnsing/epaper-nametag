#include "EPaperDisplay.h"
#include "io.h"
#include <SPI.h>


EPaperDisplay::EPaperDisplay() {}


void EPaperDisplay::begin() {
    pinMode(DISP_POWER, OUTPUT);
    digitalWrite(DISP_POWER, HIGH);

    SPI.begin();

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


void EPaperDisplay::_busyHold() {

}


void EPaperDisplay::_setRAMWindowPri() {

}


void EPaperDisplay::_setRAMWindowSec() {
    
}


void EPaperDisplay::_setRAMCursorPri() {
    
}


void EPaperDisplay::_setRAMCursorSec() {
    
}