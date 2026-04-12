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
    DISP_SPI_WRITE_DATA(0xF7); // operating sequence parameter (should probably be 0xFF)
    DISP_SPI_WRITE_COMMAND(0x20); // Command: Master Activation
    EPD_BUSY_HOLD();
}

// Local brush update
// Note: works in local brush mode
void EPD_PartialUpdate(void) {
    DISP_SPI_WRITE_COMMAND(0x22); // Command: Display Update Control 2
    DISP_SPI_WRITE_DATA(0xDC); // operating sequence parameter (Don't know what this does)
    DISP_SPI_WRITE_COMMAND(0x20); // Command: Master Activation
    EPD_BUSY_HOLD();
}

// Update with faster operating sequence but no temp data or LUT
// Note: works in quick brush mode
void EPD_FastUpdate(void) {
    DISP_SPI_WRITE_COMMAND(0x22); // Command: Display Update Control 2
    DISP_SPI_WRITE_DATA(0xC7); // operating sequence parameter (should probably be 0xCF)
    DISP_SPI_WRITE_COMMAND(0x20); // Command: Master Activation
    EPD_BUSY_HOLD();
}

// Place the display in a low power mode
void EPD_DeepSleep(void){
    DISP_SPI_WRITE_COMMAND(0x10); // Command: Deep Sleep Mode
    DISP_SPI_WRITE_DATA(0x01); // enter deep sleep mode 1
    delay(5);
}

// Initialize the display by resetting the hardware and then software
void EPD_Init(void) {
    EPD_HW_RESET();
    EPD_BUSY_HOLD();
    DISP_SPI_WRITE_COMMAND(0x12); // Command: SW Reset
    EPD_BUSY_HOLD();
}


void EPD_FastMode1Init(void);
void EPD_SetRAMMP(void);
void EPD_SetRAMMA(void);
void EPD_SetRAMSP(void);
void EPD_SetRAMSA(void);
void EPD_Clear_R26A6H(void);
void EPD_Display_Clear(void);
void EPD_Display(const uint8_t *ImageBW);
void EPD_WhiteScreen_ALL_Fast(const unsigned char *data);