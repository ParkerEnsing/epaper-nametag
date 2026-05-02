#include <Arduino.h>
#include "EPD.h"
#include "pic_home.h"

/*
How to set up the screen:
  1) Make sure the screen is powered:
  2) Initialize the SPI GPIO
  3) Create the image struct with an image buffer
  4) Initialize the image buffer with black or white (optional, I think)
  5) Set the update mode, probably FastMode1
  6) Clear the display (optional but recommended)
  7) Update the display

How to display:
  1) EPD_FastMode1Init();
  2) Draw content to buffer
  3) EPD_Display(buffer);
  4) EPD_FastUpdate();
  5) EPD_DeepSleep(); (optional; used to save power)
*/

#define SCREEN_PWR 7
#define MAX_CHAR_NUM_PER_LINE 66
#define CYCLE_PERIOD 3000 // milliseconds

uint8_t ImageBW[27200];
char *text = "Hello world";

void setup() {
  pinMode(SCREEN_PWR, OUTPUT);
  digitalWrite(SCREEN_PWR, HIGH);

  INITIALIZE_DISP_SPI_GPIO();
  Paint_NewImage(ImageBW, DISPLAY_WIDTH, DISPLAY_HEIGHT, ROTATION, WHITE);
  Paint_Clear(WHITE);
  
  clear_all();

  EPD_FastMode1Init();
  // Display default image
  EPD_ShowPicture(0, 0, 792, 272, gImage_global_1, WHITE);
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(CYCLE_PERIOD);
  clear_all();

  // Display UI example image
  EPD_ShowPicture(0, 0, 792, 272, gImage_home, WHITE);
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(CYCLE_PERIOD);
  clear_all();

  // Display inverted image
  EPD_ShowPicture(0, 0, 792, 272, gImage_global_1, BLACK);
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(CYCLE_PERIOD);
  clear_all();

  // Display text
  Paint_Clear(WHITE);
  displayLongText(0, 0, text, 24, BLACK);
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(CYCLE_PERIOD);
  clear_all();

  // Draw a line
  Paint_Clear(WHITE);
  EPD_DrawLine(0, 0, 792, 272, BLACK);
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(CYCLE_PERIOD);
  clear_all();

  // Draw a hollow rectangle
  Paint_Clear(WHITE);
  EPD_DrawRectangle(20, 20, 772, 252, BLACK, 0);
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(CYCLE_PERIOD);
  clear_all();

  // Draw a filled rectangle
  Paint_Clear(WHITE);
  EPD_DrawRectangle(20, 20, 772, 252, BLACK, 1);
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(CYCLE_PERIOD);
  clear_all();

  // Draw a hollow circle
  Paint_Clear(WHITE);
  EPD_DrawCircle(792 / 2, 272 / 2, 100, BLACK, 0);
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(CYCLE_PERIOD);
  clear_all();

  // Draw a hollow circle
  Paint_Clear(WHITE);
  EPD_DrawCircle(792 / 2, 272 / 2, 100, BLACK, 1);
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(CYCLE_PERIOD);
  clear_all();

  // Draw an image from primatives
  Paint_Clear(BLACK);
  EPD_DrawCircle(792 / 2, 272 / 2, 120, WHITE, 1);
  EPD_DrawLine(792 / 2 - 115, 272 / 2, 792 / 2 + 115, 272 / 2, BLACK);
  EPD_DrawLine(792 / 2, 272 / 2 - 115, 792 / 2, 272 / 2 + 115, BLACK);
  EPD_DrawCircle(792 / 2, 272 / 2, 10, WHITE, 1);
  EPD_DrawLine(792 / 2 - 5, 272 / 2, 792 / 2 + 5, 272 / 2, BLACK);
  EPD_DrawLine(792 / 2, 272 / 2 - 5, 792 / 2, 272 / 2 + 5, BLACK);
  EPD_Display(ImageBW);
  EPD_FastUpdate();
  EPD_DeepSleep();
  delay(CYCLE_PERIOD);
  clear_all();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void displayLongText(int x, int y, const char* content, int fontSize, int color) {
  int length = strlen(content);
  int i = 0;
  char line[MAX_CHAR_NUM_PER_LINE + 1]; // + 1 places the string terminator '\0'

  while (i < length) {
    int lineLength = 0;
    memset(line, 0, sizeof(line)); // clear line buffer

    // Fill the line until it reaches the screen width or the end of the string
    while (lineLength < MAX_CHAR_NUM_PER_LINE && i < length) {
      line[lineLength++] = content[i++];
    }
    EPD_ShowString(x, y, line, fontSize, color);
    y += fontSize;

    if (y >= 272) {
      break; // break if there are too many characters to display.
    }
  }
}

void clear_all() {
  EPD_FastMode1Init();
  EPD_Display_Clear();
  // EPD_FastUpdate();
  EPD_Update();
}