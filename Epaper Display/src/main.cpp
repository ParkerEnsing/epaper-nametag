#include <Arduino.h>
#include "pic_home.h"
#include "EPaperDisplay.h"
#include "chalkboard.h"


uint8_t canvasBuffer[27200];
EPaperDisplay EPD;

void setup() {
  Serial.begin(115200);
  EPD.begin();

  Canvas canvas(canvasBuffer, 792, 272, Canvas::landscape, 0xFF);
  canvas.begin();

  // do stuff  

  canvas.end();
  EPD.end();
}


void loop() {
  
}