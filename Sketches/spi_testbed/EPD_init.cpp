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

// Initialize the display in fast mode 1
void EPD_FastMode1Init(void) {
    EPD_Init();

    DISP_SPI_WRITE_COMMAND(0x18); // Command: Temperature Sensor Control
    DISP_SPI_WRITE_DATA(0x80); // Select built-in temperature sensor

    DISP_SPI_WRITE_COMMAND(0x22); // Command: Display Update Control 2
    DISP_SPI_WRITE_DATA(0xB1); // operating sequence parameter (should probably be B9)
    DISP_SPI_WRITE_COMMAND(0x20); // Comand: Master Activation
    EPD_BUSY_HOLD();

    DISP_SPI_WRITE_COMMAND(0x1A); // Command: Temperature Sensor Control (Write to temp register)
    DISP_SPI_WRITE_DATA(0x64); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x00); // Don't know what this does

    DISP_SPI_WRITE_COMMAND(0x22); // Command: Display Update Control 2
    DISP_SPI_WRITE_DATA(0x91); // operating sequence parameter (should probably be 99)
    DISP_SPI_WRITE_COMMAND(0x20); // Comand: Master Activation
    EPD_BUSY_HOLD();

    DISP_SPI_WRITE_COMMAND(0x3C); // Command: Border Waveform Control
    DISP_SPI_WRITE_DATA(0x3); // Don't know what this does
    EPD_BUSY_HOLD();
}

//
void EPD_SetRAMMP(void) {
    DISP_SPI_WRITE_COMMAND(0x11); // Command: Data Entry Mode Setting
    DISP_SPI_WRITE_DATA(0x05); // Don't know what this does
    DISP_SPI_WRITE_COMMAND(0x44); // Command: Set RAM X Address Start/End Position
    DISP_SPI_WRITE_DATA(0x00); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x31); // Don't know what this does
    DISP_SPI_WRITE_COMMAND(0x45); // Command: Set RAM Y Address Start/End Position
    DISP_SPI_WRITE_DATA(0x0F); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x01); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x00); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x00); // Don't know what this does
}

//
void EPD_SetRAMMA(void) {
    DISP_SPI_WRITE_COMMAND(0x4E); // Command: Set RAM X Address Counter
    DISP_SPI_WRITE_DATA(0x00); // Don't know what this does
    DISP_SPI_WRITE_COMMAND(0x4F); // Command: Set RAM Y Address Counter
    DISP_SPI_WRITE_DATA(0x0F); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x01); // Don't know what this does
}

//
void EPD_SetRAMSP(void) {
    DISP_SPI_WRITE_COMMAND(0x91); // Command: Unknown
    DISP_SPI_WRITE_DATA(0x04); // Don't know what this does
    DISP_SPI_WRITE_COMMAND(0xC4); // Command: Unknown
    DISP_SPI_WRITE_DATA(0x31); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x00); // Don't know what this does
    DISP_SPI_WRITE_COMMAND(0xC5); // Command: Unknown
    DISP_SPI_WRITE_DATA(0x0F); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x01); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x00); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x00); // Don't know what this does
}

//
void EPD_SetRAMSA(void) {
    DISP_SPI_WRITE_COMMAND(0xCE); // Command: Unknown
    DISP_SPI_WRITE_DATA(0x31); // Don't know what this does
    DISP_SPI_WRITE_COMMAND(0xCF); // Command: Unknown
    DISP_SPI_WRITE_DATA(0x0F); // Don't know what this does
    DISP_SPI_WRITE_DATA(0x01); // Don't know what this does
}

//
void EPD_Clear_R26A6H(void) {
    uint16_t i, j;
    EPD_SetRAMMA();
    DISP_SPI_WRITE_COMMAND(0x26); // Command: 
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            DISP_SPI_WRITE_DATA(0xFF); //
        }
    }
    EPD_SetRAMSA();
    DISP_SPI_WRITE_COMMAND(0xA6); // Command: 
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            DISP_SPI_WRITE_DATA(0xFF); // 
        }
    }
}

// 
void EPD_Display_Clear(void) {
    uint16_t i, j;
    EPD_SetRAMMP(); //
    EPD_SetRAMMA(); //
    DISP_SPI_WRITE_COMMAND(0x24); // Command: 
    for (i = 0; i < GATE_BITS, i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            DISP_SPI_WRITE_DATA(0xFF); // 
        }
    }
    EPD_SetRAMMA(); //
    DISP_SPI_WRITE_COMMAND(0x26); // Command: 
    for (i = 0; i < GATE_BITS, i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            DISP_SPI_WRITE_DATA(0x00); //
        }
    }
    EPD_SetRAMSP(); //
    EPD_SetRAMSA(); //
    DISP_SPI_WRITE_COMMAND(0xA4); // Command: 
    for (i = 0; i < GATE_BITS, i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            DISP_SPI_WRITE_DATA(0xFF); // 
        }
    }
    EPD_SetRAMSA(); //
    DISP_SPI_WRITE_COMMAND(0xA6); // Command: 
    for (i = 0; i < GATE_BITS, i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            DISP_SPI_WRITE_DATA(0x00); // 
        }
    }
}

//
void EPD_Display(const uint8_t *ImageBW) {
    uint32_t i;
    uint8_t tempOriginal;
    uint32_t tempCol = 0;
    uint32_t tempLine = 0;
    EDP_SetRAMMP(); // 
    EPD_SetRAMMA(); // 
    DISP_SPI_WRITE_COMMAND(0x24); // Command: 
    for (i = 0; i < ALLSCREEN_BYTES; i++) {
        tempOriginal = *(ImageBW + tempLine * SOURCE_BYTES * 2 + tempCol);
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        DISP_SPI_WRITE_DATA(tempOriginal); // 
    }
    EDP_SetRAMSP(); // 
    EPD_SetRAMSA(); // 
    DISP_SPI_WRITE_COMMAND(0xA4); // Command: 
    for (i = 0; i < ALLSCREEN_BYTES; i++) {
        tempOriginal = *(ImageBW + tempLine * SOURCE_BYTES * 2 + tempCol);
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        DISP_SPI_WRITE_DATA(tempOriginal); //
    }
}

// 
// Note: Horizontally scans from left to right, top to bottom
void EPD_WhiteScreen_ALL_Fast(const unsigned char *data) {
    unsigned int i;
    unsigned char tempOriginal;
    unsigned int tempCol = 0;
    unsigned int tempLine = 0;

    DISP_SPI_WRITE_COMMAND(0x11); // Command: 
    DISP_SPI_WRITE_DATA(0x05); // 

    DISP_SPI_WRITE_COMMAND(0x44); // Command: 
    DISP_SPI_WRITE_DATA(0x00); // 
    DISP_SPI_WRITE_DATA(0x31); // 

    DISP_SPI_WRITE_COMMAND(0x45); // Command: 
    DISP_SPI_WRITE_DATA(0x0F); // 
    DISP_SPI_WRITE_DATA(0x01); // 
    DISP_SPI_WRITE_DATA(0x00); // 
    DISP_SPI_WRITE_DATA(0x00); // 

    DISP_SPI_WRITE_COMMAND(0x4E); // Command: 
    DISP_SPI_WRITE_DATA(0x00); // 
    DISP_SPI_WRITE_COMMAND(0x4F); // Command: 
    DISP_SPI_WRITE_DATA(0x0F); // 
    DISP_SPI_WRITE_DATA(0x01); // 

    EPD_BUSY_HOLD();
    DISP_SPI_WRITE_COMMAND(0x24); // Command: 
    for (i = 0; i < ALLSCREEN_BYTES; i++) {
        tempOriginal = *(data + tempLine * SOURCE_BYTES * 2 + tempCol);
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        DISP_SPI_WRITE_DATA(~tempOriginal); // 
    }
    DISP_SPI_WRITE_COMMAND(0x26); // Command: 
    for (i - 0; i < ALLSCREEN_BYTES; i++) {
        DISP_SPI_WRITE_DATA(0x00); // 
    }

    DISP_SPI_WRITE_COMMAND(0x91); // Command: 
    DISP_SPI_WRITE_DATA(0x04); // 

    DISP_SPI_WRITE_COMMAND(0xC4); // Command: 
    DISP_SPI_WRITE_DATA(0x31); // 
    DISP_SPI_WRITE_DATA(0x00); // 

    DISP_SPI_WRITE_COMMAND(0xC5); // Command: 
    DISP_SPI_WRITE_DATA(0x0F); // 
    DISP_SPI_WRITE_DATA(0x01); // 
    DISP_SPI_WRITE_DATA(0x00); // 
    DISP_SPI_WRITE_DATA(0x00); // 

    DISP_SPI_WRITE_COMMAND(0xCE); // Command: 
    DISP_SPI_WRITE_DATA(0x31); // 
    DISP_SPI_WRITE_COMMAND(0xCF); // Command: 
    DISP_SPI_WRITE_DATA(0x0F); // 
    DISP_SPI_WRITE_DATA(0x01); // 

    EPD_BUSY_HOLD();
    
    tempCol -= 1;
    tempLine = 0;
    DISP_SPI_WRITE_COMMAND(0xA4); // Command: 
    for (i = 0; i < ALLSCREEN_BYTES; i++) {
        tempOriginal = *(data + tempLine * SOURCE_BYTES * 2 + tempCol);
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        DISP_SPI_WRITE_DATA(~tempOriginal); // 
    }
    DISP_SPI_WRITE_COMMAND(0xA6); // Command: 
    for (i - 0; i < ALLSCREEN_BYTES; i++) {
        DISP_SPI_WRITE_DATA(0x00); // 
    }
    EPD_FastUpdate();
}