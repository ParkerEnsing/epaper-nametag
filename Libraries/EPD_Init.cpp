#include "EPD_init.h"
#include "disp_spi.h"

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

// Updates the display with full operating sequence
void EPD_Update(void) {
    SPI_WRITE_COMMAND_DISP(0x22); // Command: Display Update Control 2
    SPI_WRITE_DATA_DISP(0xF7); // operating sequence parameter (should probably be 0xFF)
    SPI_WRITE_COMMAND_DISP(0x20); // Command: Master Activation
    EPD_BUSY_HOLD();
}

// Updates the display with partial operating sequence
void EPD_PartialUpdate(void) {
    SPI_WRITE_COMMAND_DISP(0x22); // Command: Display Update Control 2
    /*
    The datasheet annoyingly does not specify what 0xDC does, but ChatGPT suggests that it does
    the following based on the PDF, which I provided to it:
        1) Turn on clock signal
        2) Analog Block
        3) Load waveform LUT
        4) Display update
    It seems to be a streamlined version of running 0xF7 after 0x22 that
    reuses the existing LUT instead of loading a new one. It also disables full waveform
    sequencing
    */
    SPI_WRITE_DATA_DISP(0xDC); // operating sequence parameter
    SPI_WRITE_COMMAND_DISP(0x20); // Command: Master Activation
    EPD_BUSY_HOLD();
}

// Update with faster operating sequence but no temp data or LUT
// Note: works in quick brush mode
void EPD_FastUpdate(void) {
    SPI_WRITE_COMMAND_DISP(0x22); // Command: Display Update Control 2
    SPI_WRITE_DATA_DISP(0xC7); // operating sequence parameter (should probably be 0xCF)
    SPI_WRITE_COMMAND_DISP(0x20); // Command: Master Activation
    EPD_BUSY_HOLD();
}

// Place the display in a low power mode
void EPD_DeepSleep(void){
    SPI_WRITE_COMMAND_DISP(0x10); // Command: Deep Sleep Mode
    SPI_WRITE_DATA_DISP(0x01); // enter deep sleep mode 1
    delay(5);
}

// Initialize the display by resetting the hardware and then software
void EPD_Init(void) {
    EPD_HW_RESET();
    EPD_BUSY_HOLD();
    SPI_WRITE_COMMAND_DISP(0x12); // Command: SW Reset
    EPD_BUSY_HOLD();
}

// Initialize the display in fast mode 1
void EPD_FastMode1Init(void) {
    EPD_Init();

    SPI_WRITE_COMMAND_DISP(0x18); // Command: Temperature Sensor Control
    SPI_WRITE_DATA_DISP(0x80); // Select built-in temperature sensor

    SPI_WRITE_COMMAND_DISP(0x22); // Command: Display Update Control 2
    SPI_WRITE_DATA_DISP(0xB1); // operating sequence parameter (should probably be B9)
    SPI_WRITE_COMMAND_DISP(0x20); // Comand: Master Activation
    EPD_BUSY_HOLD();

    SPI_WRITE_COMMAND_DISP(0x1A); // Command: Temperature Sensor Control (Write to temp register)
    SPI_WRITE_DATA_DISP(0x64); // Seems to set internal temp sensor max range to 100C, per Table 6-8
    SPI_WRITE_DATA_DISP(0x00); // Seems to set internal temp sensor min range to 0C, per Table 6-8

    SPI_WRITE_COMMAND_DISP(0x22); // Command: Display Update Control 2
    SPI_WRITE_DATA_DISP(0x91); // operating sequence parameter (should probably be 99)
    SPI_WRITE_COMMAND_DISP(0x20); // Comand: Master Activation
    EPD_BUSY_HOLD();

    SPI_WRITE_COMMAND_DISP(0x3C); // Command: Border Waveform Control
    SPI_WRITE_DATA_DISP(0x3); // GS Transition, VSS for VBD, select LUT3
    EPD_BUSY_HOLD();
}

// Sets Primary Chip RAM X/Y Window Ranges (i.e. sets RAM addressing to match display resolution)
void EPD_SET_RAM_WINDOW_PRI(void) {
    SPI_WRITE_COMMAND_DISP(0x11); // Command: Data Entry Mode Setting 00010001
    SPI_WRITE_DATA_DISP(0x05); // Y decrement, X increment. Address counter is updated in Y direction 00000101
    SPI_WRITE_COMMAND_DISP(0x44); // Command: Set RAM X Address Start/End Position 01000100
    // 0x00 -> 0x31 = 0 -> 49 = 50 bytes = 400 bits = 400 pixels
    SPI_WRITE_DATA_DISP(0x00); // Sets starting RAM X Range Address
    SPI_WRITE_DATA_DISP(0x31); // Sets ending RAM X Range Address
    SPI_WRITE_COMMAND_DISP(0x45); // Command: Set RAM Y Address Start/End Position
    // Sets vertical pixel range to 272
    SPI_WRITE_DATA_DISP(0x0F); // Sets A[7:0] bits for RAM Y Range Start Address
    SPI_WRITE_DATA_DISP(0x01); // Sets A[8] bit for RAM Y Range Start Address
    SPI_WRITE_DATA_DISP(0x00); // Sets B[7:0] bits for RAM Y Range End Address
    SPI_WRITE_DATA_DISP(0x00); // Sets B[8] bit for RAM Y Range End Address
}

// Sets Primary Chip address for RAM Window Counter (i.e Tells the "cursor" where to be in the RAM window - same as picking a pixel)
void EPD_SET_RAM_CURSOR_PRI(void) {
    SPI_WRITE_COMMAND_DISP(0x4E); // Command: Set RAM X Address Counter
    SPI_WRITE_DATA_DISP(0x00); // Sets initial RAM x address counter value
    SPI_WRITE_COMMAND_DISP(0x4F); // Command: Set RAM Y Address Counter
    SPI_WRITE_DATA_DISP(0x0F); // Sets A[7:0] bits for RAM y address counter value
    SPI_WRITE_DATA_DISP(0x01); // Sets A[8] bit for RAM y address counter value
}

// Sets Secondary Chip RAM X/Y Window Ranges (i.e. sets RAM addressing to match display resolution)
// Note: code for secondary chip uses the same commands/data, but addresses have A[7] = 1 (likely for chip selection)
void EPD_SET_RAM_WINDOW_SEC(void) {
    SPI_WRITE_COMMAND_DISP(0x91); // 10010001
    SPI_WRITE_DATA_DISP(0x04); // 00000100
    SPI_WRITE_COMMAND_DISP(0xC4); // 11000100
    SPI_WRITE_DATA_DISP(0x31); // 00110001
    SPI_WRITE_DATA_DISP(0x00); // 00000000
    SPI_WRITE_COMMAND_DISP(0xC5); // 11000101
    SPI_WRITE_DATA_DISP(0x0F); // 00001111
    SPI_WRITE_DATA_DISP(0x01); // 00000001
    SPI_WRITE_DATA_DISP(0x00); // 00000000
    SPI_WRITE_DATA_DISP(0x00); // 00000000
}

// Sets Secondary Chip address for RAM Window Counter (i.e Tells the "cursor" where to be in the RAM window - same as picking a pixel)
// Note: code for secondary chip uses the same commands/data, but addresses have A[7] = 1 (likely for chip selection)
void EPD_SET_RAM_CURSOR_SEC(void) {
    SPI_WRITE_COMMAND_DISP(0xCE); // 11001110
    SPI_WRITE_DATA_DISP(0x31); // 11000001
    SPI_WRITE_COMMAND_DISP(0xCF); // 11001111
    SPI_WRITE_DATA_DISP(0x0F); // 00001111
    SPI_WRITE_DATA_DISP(0x01); // 00000001
}

// Fills Red RAM on primary and secondary chips with white
void EPD_RED_RAM_FILL_WHITE(void) {
    uint16_t i, j;
    EPD_SET_RAM_CURSOR_PRI();
    SPI_WRITE_COMMAND_DISP(0x26); // Command: Write RAM (RED) / RAM 0x26 00100110
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            SPI_WRITE_DATA_DISP(WHITE);
        }
    }
    EPD_SET_RAM_CURSOR_SEC();
    SPI_WRITE_COMMAND_DISP(0xA6); // 10100110
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            SPI_WRITE_DATA_DISP(WHITE);
        }
    }
}

/*
The SSD1683 has a max resolution of 400 x 300, but the display is 792 x 272.
Therefore, there are two SSD1683s that each control half of the display.
They each have their own BW and Red RAM, so 4 units of RAM will have to be set individually.
Refer to Table 6-4 for more information on what the following code does.
*/
void EPD_Display_Clear(void) {
    uint16_t i, j;
    EPD_SET_RAM_WINDOW_PRI(); // Set primary RAM window
    EPD_SET_RAM_CURSOR_PRI(); // Set primary RAM cursor
    SPI_WRITE_COMMAND_DISP(0x24); // Command: Write RAM (Black White) / RAM 0x24
    // Iterate over primary RAM bits and set to white
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            SPI_WRITE_DATA_DISP(WHITE); // Sets the pixel to white
        }
    }
    EPD_SET_RAM_CURSOR_PRI(); // Move cursor back to beginning
    SPI_WRITE_COMMAND_DISP(0x26); // Command: Write RAM (RED) / RAM 0x26
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            SPI_WRITE_DATA_DISP(BLACK); // Sets the pixel to black
        }
    }
    EPD_SET_RAM_WINDOW_SEC(); // Set secondary RAM window
    EPD_SET_RAM_CURSOR_SEC(); // Set secondary RAM cursor
    SPI_WRITE_COMMAND_DISP(0xA4); // Command (secondary): Write RAM (Black White) / RAM 0x24
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            SPI_WRITE_DATA_DISP(WHITE); // Sets the pixel to white
        }
    }
    EPD_SET_RAM_CURSOR_SEC(); // Move cursor back to beginning
    SPI_WRITE_COMMAND_DISP(0xA6); // Command (secondary): Write RAM (RED) / RAM 0x26
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            SPI_WRITE_DATA_DISP(BLACK); // Sets the pixel to black
        }
    }
}

// Write data from an image to display pixels
// Note: Vertically scans from top to bottom, left to right
void EPD_Display(const uint8_t *ImageBW) {
    uint32_t i;
    uint8_t tempOriginal;
    uint32_t tempCol = 0;
    uint32_t tempLine = 0;
    EPD_SET_RAM_WINDOW_PRI();
    EPD_SET_RAM_CURSOR_PRI();
    SPI_WRITE_COMMAND_DISP(0x24); // Command: Write RAM (Black White) / RAM 0x24 00100100
    // Scan across image and write values to RAM
    for (i = 0; i < ALLSCREEN_BYTES; i++) {
        tempOriginal = *(ImageBW + tempLine * SOURCE_BYTES * 2 + tempCol); // Retrieve image pixel address
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        SPI_WRITE_DATA_DISP(tempOriginal); // Write image pixel address to display pixel
    }
    // Repeat for secondary SSD1683
    EPD_SET_RAM_WINDOW_SEC();
    EPD_SET_RAM_CURSOR_SEC();
    SPI_WRITE_COMMAND_DISP(0xA4); // 10100100
    for (i = 0; i < ALLSCREEN_BYTES; i++) {
        tempOriginal = *(ImageBW + tempLine * SOURCE_BYTES * 2 + tempCol);
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        SPI_WRITE_DATA_DISP(tempOriginal);
    }
}

// Forces waveform transition to clear screen quickly
// Will clear
// Note: Horizontally scans from left to right, top to bottom
void EPD_FAST_INVERT(const unsigned char *data) {
    unsigned int i;
    unsigned char tempOriginal;
    unsigned int tempCol = 0;
    unsigned int tempLine = 0;

    EPD_SET_RAM_WINDOW_PRI();
    EPD_SET_RAM_CURSOR_PRI();

    EPD_BUSY_HOLD();
    SPI_WRITE_COMMAND_DISP(0x24); // Command: Write RAM (Black White) / RAM 0x24 00100100
    for (i = 0; i < ALLSCREEN_BYTES; i++) {
        tempOriginal = *(data + tempLine * SOURCE_BYTES * 2 + tempCol);
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        SPI_WRITE_DATA_DISP(~tempOriginal); // invert the byte to force max contrast waveform
    }
    SPI_WRITE_COMMAND_DISP(0x26); // Command: Write RAM (RED) / RAM 0x26 00100110
    for (i = 0; i < ALLSCREEN_BYTES; i++) {
        SPI_WRITE_DATA_DISP(BLACK); // Write black to red RAM
    }

    // Repeat for secondary SSD1683
    EPD_SET_RAM_WINDOW_SEC();
    EPD_SET_RAM_CURSOR_SEC();

    EPD_BUSY_HOLD();
    
    tempCol -= 1;
    tempLine = 0;
    SPI_WRITE_COMMAND_DISP(0xA4); // 10100100
    for (i = 0; i < ALLSCREEN_BYTES; i++) {
        tempOriginal = *(data + tempLine * SOURCE_BYTES * 2 + tempCol);
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        SPI_WRITE_DATA_DISP(~tempOriginal);
    }
    SPI_WRITE_COMMAND_DISP(0xA6); // Command: 10100110
    for (i - 0; i < ALLSCREEN_BYTES; i++) {
        SPI_WRITE_DATA_DISP(0x00); // 
    }
    EPD_FastUpdate();
}