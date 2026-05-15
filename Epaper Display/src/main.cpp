#include <Arduino.h>
#include "EPD.h"
#include "pic_home.h"
#include "EPaperDisplay.h"


#define SCREEN_PWR 7
#define MAX_CHAR_NUM_PER_LINE 66
#define CYCLE_PERIOD 3000 // milliseconds

uint8_t ImageBW[27200];


void setup() {
  EPaperDisplay EPD;
  EPD.begin();

  Paint_NewImage(ImageBW, DISPLAY_WIDTH, DISPLAY_HEIGHT, ROTATION, WHITE);
  Paint_Clear(WHITE);
  EPD.render(ImageBW, false, true);
  
  EPD_ShowPicture(0, 0, 792, 272, gImage_global_1, WHITE);
  EPD.render(ImageBW, true, true);
  delay(CYCLE_PERIOD);

  // Display UI example image
  EPD_ShowPicture(0, 0, 792, 272, gImage_home, WHITE);
  EPD.render(ImageBW, true, true);
  delay(CYCLE_PERIOD);

  // Display inverted image
  EPD_ShowPicture(0, 0, 792, 272, gImage_global_1, BLACK);
  EPD.render(ImageBW, true, true);
  delay(CYCLE_PERIOD);

  // Draw a line
  Paint_Clear(WHITE);
  EPD_DrawLine(0, 0, 792, 272, BLACK);
  EPD.render(ImageBW, true, true);
  delay(CYCLE_PERIOD);

  // Draw a hollow rectangle
  Paint_Clear(WHITE);
  EPD_DrawRectangle(20, 20, 772, 252, BLACK, 0);
  EPD.render(ImageBW, true, true);
  delay(CYCLE_PERIOD);

  // Draw a filled rectangle
  Paint_Clear(WHITE);
  EPD_DrawRectangle(20, 20, 772, 252, BLACK, 1);
  EPD.render(ImageBW, true, true);
  delay(CYCLE_PERIOD);

  // Draw a hollow circle
  Paint_Clear(WHITE);
  EPD_DrawCircle(792 / 2, 272 / 2, 100, BLACK, 0);
  EPD.render(ImageBW, true, true);
  delay(CYCLE_PERIOD);

  // Draw a hollow circle
  Paint_Clear(WHITE);
  EPD_DrawCircle(792 / 2, 272 / 2, 100, BLACK, 1);
  EPD.render(ImageBW, true, true);
  delay(CYCLE_PERIOD);

  // Draw an image from primatives
  Paint_Clear(BLACK);
  EPD_DrawCircle(792 / 2, 272 / 2, 120, WHITE, 1);
  EPD_DrawLine(792 / 2 - 115, 272 / 2, 792 / 2 + 115, 272 / 2, BLACK);
  EPD_DrawLine(792 / 2, 272 / 2 - 115, 792 / 2, 272 / 2 + 115, BLACK);
  EPD_DrawCircle(792 / 2, 272 / 2, 10, WHITE, 1);
  EPD_DrawLine(792 / 2 - 5, 272 / 2, 792 / 2 + 5, 272 / 2, BLACK);
  EPD_DrawLine(792 / 2, 272 / 2 - 5, 792 / 2, 272 / 2 + 5, BLACK);
  EPD.render(ImageBW, true, true);
  delay(CYCLE_PERIOD);
}


void loop() {
  // put your main code here, to run repeatedly:

}