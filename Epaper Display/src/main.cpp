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
  
  EPD.render(gImage_home);
  Serial.println("Displayed image...");
  delay(5000);

  Serial.println("Starting white render...");
  EPD.render(canvasBuffer);
  Serial.println("Rendered white to display...");
  delay(5000);

  canvas.setBackgroundColor(0x00);
  Serial.println("Changed background color to black...");
  canvas.erase();
  Serial.println("Erased buffer...");
  Serial.println("Starting black render...");
  EPD.render(canvasBuffer);
  Serial.println("Rendered black to display...");
  delay(5000);

  // canvas.end();
  // Serial.println("Ended canvas...");
  // EPD.end();
  // Serial.println("Ended display...");
}


void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < sizeof(canvasBuffer); i++) {
    if (i % 2 == 0) {
      canvasBuffer[i] = 0xFF;
    } else {
      canvasBuffer[i] = 0x00;
    }
  }
  // EPD.initializeFastMode();
  // EPD.display(canvasBuffer);
  // EPD.partialUpdate();
  EPD.render(canvasBuffer);
  delay(1000);

  EPD.initializeFastMode();
  EPD.clear();
  EPD.fastUpdate();

  bool isBlack = true;
  for (int i = 0; i < sizeof(canvasBuffer); i++) {
    uint8_t byteColor = 0x00;
    if (!isBlack) {
      byteColor = 0xFF;
    }
    canvasBuffer[i] = byteColor;
    if (i % 272 * 8 == 0) {
      isBlack = !isBlack;
    }
  }
  EPD.display(canvasBuffer);
  EPD.update();
  delay(1000);
}