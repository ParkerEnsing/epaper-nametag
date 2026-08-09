#include "DevBoardIO.h"
#include "EPaperDisplay.h"
#include <SPI.h>


// https://www.elecrow.com/download/product/DIS08792E/SSD1683_Datasheet.PDF


#define readBusy digitalRead(DISP_SPI_BUSY)
#define setCS() digitalWrite(DISP_SPI_CS, HIGH)
#define clearCS() digitalWrite(DISP_SPI_CS, LOW)
#define setDC() digitalWrite(DISP_SPI_DC, HIGH)
#define clearDC() digitalWrite(DISP_SPI_DC, LOW)
#define setRES() digitalWrite(DISP_SPI_RES, HIGH)
#define clearRES() digitalWrite(DISP_SPI_RES, LOW)
#define setSCK() digitalWrite(DISP_SPI_SCK, HIGH);
#define clearSCK() digitalWrite(DISP_SPI_SCK, LOW);


EPaperDisplay::EPaperDisplay() {}


/*
How to set up the screen:
  1) Make sure the screen is powered:
  2) Initialize the SPI GPIO
  3) Create the image struct with an image buffer
  4) Initialize the image buffer with black or white (optional, I think)
  5) Set the update mode, probably FastMode1
  6) Clear the display (optional but recommended)
  7) Update the display
*/
void EPaperDisplay::begin() {
    // power on
    pinMode(DISP_POWER, OUTPUT);
    digitalWrite(DISP_POWER, HIGH);
    delay(10);

    initializeGPIO();

    clearUpdate();
}


void EPaperDisplay::end() {
    initializeFastMode();
    clear();
    update();
    SPI.end();
    digitalWrite(DISP_POWER, LOW);
}


void EPaperDisplay::initializeGPIO() {
    // configure SPI
    pinMode(DISP_SPI_BUSY, INPUT);
    pinMode(DISP_SPI_COPI, OUTPUT);
    pinMode(DISP_SPI_SCK, OUTPUT);
    pinMode(DISP_SPI_CS, OUTPUT);
    pinMode(DISP_SPI_DC, OUTPUT);
    pinMode(DISP_SPI_RES, OUTPUT);
    SPI.begin(DISP_SPI_SCK, -1, DISP_SPI_COPI, DISP_SPI_CS); // No CIPO connection
}


// Resets hardware
// Note: HW reset is required after entering deep sleep
void EPaperDisplay::hwReset() {
    delay(10);
    clearRES();
    delay(10);
    setRES();
    delay(10);
    _busyHold();
}


/*
The SSD1683 has a max resolution of 400 x 300, but the display is 792 x 272.
Therefore, there are two SSD1683s that each control half of the display.
They each have their own BW and Red RAM, so 4 units of RAM will have to be set individually.
Refer to Table 6-4 for more information on what the following code does.
*/
void EPaperDisplay::clear() {
    uint16_t i, j;
    _setRAMWindowPri(); // Set primary RAM window
    _setRAMCursorPri(); // Set primary RAM cursor
    _writeCommand(0x24); // Command: Write RAM (Black White) / RAM 0x24
    // Iterate over primary RAM bits and set to white
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            _writeData(0xFF); // Sets the byte to white
        }
    }
    _setRAMCursorPri(); // Move cursor back to beginning
    _writeCommand(0x26); // Command: Write RAM (RED) / RAM 0x26
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            _writeData(0x00); // Sets the byte to black
        }
    }
    _setRAMWindowSec(); // Set secondary RAM window
    _setRAMCursorSec(); // Set secondary RAM cursor
    _writeCommand(0xA4); // Command (secondary): Write RAM (Black White) / RAM 0x24
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            _writeData(0xFF); // Sets the byte to white
        }
    }
    _setRAMCursorSec(); // Move cursor back to beginning
    _writeCommand(0xA6); // Command (secondary): Write RAM (RED) / RAM 0x26
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            _writeData(0x00); // Sets the byte to black
        }
    }
}


// Initialize the display by resetting the hardware and then software
void EPaperDisplay::initialize() {
    hwReset();
    _busyHold();
    _writeCommand(0x12); // SW reset
    _busyHold();
}


// Initialize the display in fast mode 1
void EPaperDisplay::initializeFastMode() {
    initialize();
    delay(10);

    _writeCommand(0x18); // Command: Temperature Sensor Control
    _writeData(0x80); // Select built-in temperature sensor

    _writeCommand(0x22); // Command: Display Update Control 2
    _writeData(0xB1); // operating sequence parameter
    _writeCommand(0x20); // Comand: Master Activation
    _busyHold();

    _writeCommand(0x1A); // Command: Temperature Sensor Control (Write to temp register)
    _writeData(0x64); // Seems to set internal temp sensor max range to 100C, per Table 6-8
    _writeData(0x00); // Seems to set internal temp sensor min range to 0C, per Table 6-8

    _writeCommand(0x22); // Command: Display Update Control 2
    _writeData(0x91); // operating sequence parameter
    _writeCommand(0x20); // Comand: Master Activation
    _busyHold();

    _writeCommand(0x3C); // Command: Border Waveform Control
    _writeData(0x3); // GS Transition, VSS for VBD, select LUT3
    _busyHold();
}


// Write data from an image to display pixels
void EPaperDisplay::display(const uint8_t* imageAddress) {
    uint32_t i;
    uint8_t tempOriginal;
    uint32_t tempCol = 0;
    uint32_t tempLine = 0;
    _setRAMWindowPri();
    _setRAMCursorPri();
    _writeCommand(0x24); // Command: Write RAM (Black White) / RAM 0x24 00100100
    // Scan across image and write values to RAM
    for (i = 0; i < TOTAL_SCREEN_BYTES; i++) {
        tempOriginal = *(imageAddress + tempLine * SOURCE_BYTES * 2 + tempCol); // Retrieve image pixel address
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        _writeData(tempOriginal); // Write image pixel address to display pixel
    }
    // Repeat for secondary SSD1683
    _setRAMWindowSec();
    _setRAMCursorSec();
    _writeCommand(0xA4); // 10100100
    for (i = 0; i < TOTAL_SCREEN_BYTES; i++) {
        tempOriginal = *(imageAddress + tempLine * SOURCE_BYTES * 2 + tempCol);
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        _writeData(tempOriginal);
    }
}


// Updates the display with full operating sequence
void EPaperDisplay::update() {
    _writeCommand(0x22); // Command: Display Update Control 2
    _writeData(0xF7); // operating sequence parameter
    _writeCommand(0x20); // Command: Master Activation
    _busyHold();
}


// Updates the display with partial operating sequence
void EPaperDisplay::partialUpdate() {
    _writeCommand(0x22); // Command: Display Update Control 2
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
    _writeData(0xDC); // operating sequence parameter
    _writeCommand(0x20); // Command: Master Activation
    _busyHold();
}


// Update with faster operating sequence but no temp data or LUT
// Note: works in quick brush mode
void EPaperDisplay::fastUpdate() {
    _writeCommand(0x22); // Command: Display Update Control 2
    _writeData(0xC7); // operating sequence parameter
    _writeCommand(0x20); // Command: Master Activation
    _busyHold();
}


void EPaperDisplay::clearUpdate() {
    initializeFastMode();
    clear();
    update();
}


void EPaperDisplay::render(const uint8_t* imageAddress) {
    initializeFastMode();
    display(imageAddress);
    update();
}


void EPaperDisplay::renderAndSleep(const uint8_t* imageAddress) {
    render(imageAddress);
    deepSleep();
}


void EPaperDisplay::fastRender(const uint8_t* imageAddress) {
    initializeFastMode();
    display(imageAddress);
    fastUpdate();
}


void EPaperDisplay::fastRenderAndSleep(const uint8_t* imageAddress) {
    fastRender(imageAddress);
    deepSleep();
}


void EPaperDisplay::partialRender(const uint8_t* imageAddress) {
    initializeFastMode();
    display(imageAddress);
    partialUpdate();
}


void EPaperDisplay::partialRenderAndSleep(const uint8_t* imageAddress) {
    partialRender(imageAddress);
    deepSleep();
}


// Forces waveform transition to clear screen quickly
// Note: Horizontally scans from left to right, top to bottom
void EPaperDisplay::fastInvert(const unsigned char* data) {
    unsigned int i;
    unsigned char tempOriginal;
    unsigned int tempCol = 0;
    unsigned int tempLine = 0;
    _setRAMWindowPri();
    _setRAMCursorPri();
    _busyHold();
    _writeCommand(0x24); // Command: Write RAM (Black White) / RAM 0x24 00100100
    for (i = 0; i < TOTAL_SCREEN_BYTES; i++) {
        tempOriginal = *(data + tempLine * SOURCE_BYTES * 2 + tempCol);
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        _writeData(~tempOriginal); // invert the byte to force max contrast waveform
    }
    _writeCommand(0x26); // Command: Write RAM (RED) / RAM 0x26 00100110
    for (i = 0; i < TOTAL_SCREEN_BYTES; i++) {
        _writeData(0x00); // Write black to red RAM
    }

    // Repeat for secondary SSD1683
    _setRAMWindowSec();
    _setRAMCursorSec();

    _busyHold();
    
    tempCol -= 1;
    tempLine = 0;
    _writeCommand(0xA4); // 10100100
    for (i = 0; i < TOTAL_SCREEN_BYTES; i++) {
        tempOriginal = *(data + tempLine * SOURCE_BYTES * 2 + tempCol);
        tempLine++;
        if (tempLine >= GATE_BITS) {
            tempCol++;
            tempLine = 0;
        }
        _writeData(~tempOriginal);
    }
    _writeCommand(0xA6); // Command: 10100110
    for (i - 0; i < TOTAL_SCREEN_BYTES; i++) {
        _writeData(0x00); // 
    }
    fastUpdate();
}


// Place the display in a low power mode
void EPaperDisplay::deepSleep() {
    _writeCommand(0x10); // Command: Deep Sleep Mode
    _writeData(0x01); // enter deep sleep mode 1
    delay(5);
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


// Wait until display is done refreshing
void EPaperDisplay::_busyHold() {
    while (1) {
        if (readBusy == LOW) {
            break;
        }
    }
}


// Sets Primary Chip RAM X/Y Window Ranges (i.e. sets RAM addressing to match display resolution)
void EPaperDisplay::_setRAMWindowPri() {
    _writeCommand(0x11); // Command: Data Entry Mode Setting 00010001
    _writeData(0x05); // Y decrement, X increment. Address counter is updated in Y direction 00000101
    _writeCommand(0x44); // Command: Set RAM X Address Start/End Position 01000100
    // 0x00 -> 0x31 = 0 -> 49 = 50 bytes = 400 bits = 400 pixels
    _writeData(0x00); // Sets starting RAM X Range Address
    _writeData(0x31); // Sets ending RAM X Range Address
    _writeCommand(0x45); // Command: Set RAM Y Address Start/End Position
    // Sets vertical pixel range to 272
    _writeData(0x0F); // Sets A[7:0] bits for RAM Y Range Start Address
    _writeData(0x01); // Sets A[8] bit for RAM Y Range Start Address
    _writeData(0x00); // Sets B[7:0] bits for RAM Y Range End Address
    _writeData(0x00); // Sets B[8] bit for RAM Y Range End Address
}


// Sets Secondary Chip RAM X/Y Window Ranges (i.e. sets RAM addressing to match display resolution)
// Note: code for secondary chip uses the same commands/data, but addresses have A[7] = 1 (likely for chip selection)
void EPaperDisplay::_setRAMWindowSec() {
    _writeCommand(0x91); // 10010001
    _writeData(0x04); // 00000100
    _writeCommand(0xC4); // 11000100
    _writeData(0x31); // 00110001
    _writeData(0x00); // 00000000
    _writeCommand(0xC5); // 11000101
    _writeData(0x0F); // 00001111
    _writeData(0x01); // 00000001
    _writeData(0x00); // 00000000
    _writeData(0x00); // 00000000
}


// Sets Primary Chip address for RAM Window Counter (i.e Tells the "cursor" where to be in the RAM window - same as picking a pixel)
void EPaperDisplay::_setRAMCursorPri() {
    _writeCommand(0x4E); // Command: Set RAM X Address Counter
    _writeData(0x00); // Sets initial RAM x address counter value
    _writeCommand(0x4F); // Command: Set RAM Y Address Counter
    _writeData(0x0F); // Sets A[7:0] bits for RAM y address counter value
    _writeData(0x01); // Sets A[8] bit for RAM y address counter value
}


// Sets Secondary Chip address for RAM Window Counter (i.e Tells the "cursor" where to be in the RAM window - same as picking a pixel)
// Note: code for secondary chip uses the same commands/data, but addresses have A[7] = 1 (likely for chip selection)
void EPaperDisplay::_setRAMCursorSec() {
    _writeCommand(0xCE); // 11001110
    _writeData(0x31); // 11000001
    _writeCommand(0xCF); // 11001111
    _writeData(0x0F); // 00001111
    _writeData(0x01); // 00000001
}


// Fills Red RAM on primary and secondary chips with white
void EPaperDisplay::_fillRedRAM() {
    uint16_t i, j;
    _setRAMCursorPri();
    _writeCommand(0x26); // Command: Write RAM (RED) / RAM 0x26 00100110
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            _writeData(0xFF); // fills with white
        }
    }
    _setRAMCursorSec();
    _writeCommand(0xA6); // 10100110
    for (i = 0; i < GATE_BITS; i++) {
        for (j = 0; j < SOURCE_BYTES; j++) {
            _writeData(0xFF); // fills with white
        }
    }
}

