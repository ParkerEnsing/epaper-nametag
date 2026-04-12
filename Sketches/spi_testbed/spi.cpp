#include "spi.h"
#include "io.h"

void INITIALIZE_SPI_GPIO(void) {
    pinMode(DISP_SPI_COPI, OUTPUT);
    pinMode(DISP_SPI_SCK, OUTPUT);
    pinMode(DISP_SPI_CS, OUTPUT);
    pinMode(DISP_SPI_DC, OUTPUT);
    pinMode(DISP_SPI_RES, OUTPUT);
    pinMode(DISP_SPI_BUSY, INPUT);
}

void DISP_SPI_WRITE_BYTE(uint8_t data){
    uint8_t i;
    clearDispCS(); // Tell the display to start listening
    for (i = 0; i < 8; i++) {
        clearDispSCK(); // Pulls clock low for new bit
        if (data & 0x80) { // Checks MSB and sets data line accordingly
            setDispCOPI(); // Set data line high for 1
        }
        else {
            clearDispCOPI(); // Pul data line low for 0
        }
        setDispSCK(); // Sets clock high to write the bit to the display
        data <<= 1; // Shift bits one to the left (toward MSB)
    }
    setDispCS(); // Deselect the device (complete transmission)
}

void DISP_SPI_WRITE_COMMAND(uint8_t registerCommand){
    clearDispDC(); // Tell display to expect a command
    DISP_SPI_WRITE_BYTE(registerCommand);
    setDispDC(); // Return to data writing
}

void DISP_SPI_WRITE_DATA(uint8_t data){
    setDispDC(); // Tell display to expect data
    DISP_SPI_WRITE_BYTE(data);
    clearDispDC(); // Return to command writing
}