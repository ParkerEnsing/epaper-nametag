#include <Arduino.h>
#include "EPD_init.h"
#include "spi.h"

// https://www.elecrow.com/download/product/DIS08792E/SSD1683_Datasheet.PDF

// Wait until not busy
// Note: Busy == 1
void EPD_BUSY_HOLD(void) {
    while (1) {
        if (readDispBusy == 0) {
            break;
        }
    }
}

// Resets hardware
// Note: HW reset is required after entering deep sleep
void EPD_HW_RESET(void) {
    delay(10);
    clearDispRES();
    delay(10);
    setDispRES();
    delay(10);
    EPD_BUSY_HOLD();
}

void EPD_Update(void) {
    DISP_SPI_WRITE_COMMAND(0x22); // Command: Display Update Control 2
    DISP_SPI_WRITE_DATA(0xF7); // operating sequence parameter
    DISP_SPI_WRITE_COMMAND(0x20); // Command: Master Activation
    EPD_BUSY_HOLD();
}

// Local brush update
// Note: works in local brush mode
void EPD_PartialUpdate(void) {
    DISP_SPI_WRITE_COMMAND(0x22); // Command: Display Update Control 2
    DISP_SPI_WRITE_DATA(0xDC); // operating sequence parameter
    DISP_SPI_WRITE_COMMAND(0x20); // Command: Master Activation
    EPD_BUSY_HOLD();
}

void EPD_FastUpdate(void) {
    DISP_SPI_WRITE_COMMAND(0x22); // Command: Display Update Control 2
    DISP_SPI_WRITE_DATA(0xC7); // operating sequence parameter
    DISP_SPI_WRITE_COMMAND(0x20); // Command: Master Activation
    EPD_BUSY_HOLD();
}