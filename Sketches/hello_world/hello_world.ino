#include <Arduino.h>
#include "EPD.h"

/*
Pinout:
SCK   -> 12 | O
MOSI  -> 11 | O
RES   -> 47 | O
DC    -> 46 | O
CS    -> 45 | O
BUSY  -> 48 | I
*/

#define SCREEN_PWR 7

uint8_t ImageBW[27200];

void setup() {
  // Set screen power pin and turn on screen
  pinMode(SCREEN_PWR, OUTPUT);
  digitalWrite(SCREEN_PWR, HIGH);

  EPD_GPIOInit(); // Initializes SPI pins. See spi.cpp
  Paint_NewImage(ImageBW, EPD_W, EPD_H, Rotation, WHITE); // Create an image buffer with device size and white background color
  Paint_Clear(WHITE); // Clear buffer and fill screen with white
  
  clear_all();

  // EPD_GPIOInit();
  // EPD_FastMode1Init();
  // EPD_DrawCircle(EPD_W, EPD_H, 100, BLACK, 0);
  // delay(5000);

  // clear_all();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void clear_all() {
  EPD_FastMode1Init();
  EPD_Display_Clear();
  EPD_Update();
}