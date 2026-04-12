#ifndef _EPD_INIT_H_
#define _EPD_INIT_H_

#include "spi.h"

#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 272

#define WHITE 0xFF
#define BLACK 0x00

//#define ALLSCREEN_GRAGHBYTES 27200/2 // = SOURCE_BYTES * GATE_BITS

#define SOURCE_BYTES 400/8
#define GATE_BITS 272
#define ALLSCREEN_BYTES SOURCE_BYTES * GATE_BITS

void EPD_BUSY_HOLD(void);
void EPD_HW_RESET(void);
void EPD_Update(void);
void EPD_PartialUpdate(void);
void EPD_FastUpdate(void);
void EPD_DeepSleep(void);
void EPD_Init(void);
void EPD_FastMode1Init(void);
void EPD_SetRAMMP(void);
void EPD_SetRAMMA(void);
void EPD_SetRAMSP(void);
void EPD_SetRAMSA(void);
void EPD_Clear_R26A6H(void);
void EPD_Display_Clear(void);
void EPD_Display(const uint8_t *ImageBW);
void EPD_WhiteScreen_ALL_Fast(const unsigned char *data);

#endif