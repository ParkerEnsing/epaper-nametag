#include <Arduino.h>
#include "EPD.h"

uint8_t ImageBW[27200];

void setup() {
  // Turn on display
  pinMode(DISP_POWER, OUTPUT);
  digitalWrite(DISP_POWER, HIGH);

  EPD_GPIOInit();
  Paint_NewImage(ImageBW, DISPLAY_WIDTH, DISPLAY_HEIGHT, ROTATION, WHITE);
  Paint_Clear(WHITE);

  clear_all();

  EPD_FastMode1Init();

  // Draw content here
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(5000);

  clear_all();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void clear_all() {
  EPD_FastMode1Init();
  EPD_Display_Clear();
  EPD_Update();
}