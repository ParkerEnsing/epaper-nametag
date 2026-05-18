#include <Arduino.h>
#include "pic_home.h"
#include "EPaperDisplay.h"
#include "chalkboard.h"


uint8_t canvasBuffer[27200];


void setup() {
  Serial.begin(115200);
  EPaperDisplay EPD;
  EPD.begin();
  Serial.println("EPD initialized...");


  Serial.println("Starting test pattern...");
  for (int i = 0; i < sizeof(canvasBuffer); i++) {
    if (i % 2 == 0) {
      canvasBuffer[i] = 0xFF;
    } else {
      canvasBuffer[i] = 0x00;
    }
  }
  // EPD.hwReset();
  EPD.initializeFastMode();
  Serial.println("Reset complete...");
  EPD.display(canvasBuffer);
  Serial.println("Display complete...");
  EPD.update();
  Serial.println("Updates complete...");
  Serial.println("Starting 2 second delay...");
  delay(2000);
  Serial.println("Rendered test pattern...");

  EPD.initializeFastMode();
  EPD.clear();
  EPD.update();

  // Canvas canvas(canvasBuffer, 792, 272, Canvas::landscape, 0xFF);
  // canvas.begin();
  
  // EPD.render(gImage_home);
  // Serial.println("Displayed image...");
  // delay(5000);

  // Serial.println("Starting white render...");
  // EPD.render(canvasBuffer);
  // Serial.println("Rendered white to display...");
  // delay(5000);

  // canvas.setBackgroundColor(0x00);
  // Serial.println("Changed background color to black...");
  // canvas.erase();
  // Serial.println("Erased buffer...");
  // Serial.println("Starting black render...");
  // EPD.render(canvasBuffer);
  // Serial.println("Rendered black to display...");
  // delay(5000);

  // canvas.end();
  // Serial.println("Ended canvas...");
  EPD.end();
  Serial.println("Ended display...");
}


void loop() {
  // put your main code here, to run repeatedly:

}