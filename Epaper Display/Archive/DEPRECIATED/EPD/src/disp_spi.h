#ifndef _DISP_SPI_H_
#define _DISP_SPI_H_

#include <Arduino.h>
#include "io.h"

#define clearDispCOPI() digitalWrite(DISP_SPI_COPI, LOW)
#define setDispCOPI() digitalWrite(DISP_SPI_COPI, HIGH)

#define clearDispSCK() digitalWrite(DISP_SPI_SCK, LOW)
#define setDispSCK() digitalWrite(DISP_SPI_SCK, HIGH)

#define clearDispCS() digitalWrite(DISP_SPI_CS, LOW)
#define setDispCS() digitalWrite(DISP_SPI_CS, HIGH)

#define clearDispDC() digitalWrite(DISP_SPI_DC, LOW)
#define setDispDC() digitalWrite(DISP_SPI_DC, HIGH)

#define clearDispRES() digitalWrite(DISP_SPI_RES, LOW)
#define setDispRES() digitalWrite(DISP_SPI_RES, HIGH)

#define readDispBusy digitalRead(DISP_SPI_BUSY)

void INITIALIZE_DISP_SPI_GPIO(void);
void SPI_WRITE_BYTE_DISP(uint8_t data);
void SPI_WRITE_COMMAND_DISP(uint8_t registerCommand);
void SPI_WRITE_DATA_DISP(uint8_t data);

#endif