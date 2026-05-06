#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "EPD.h"

File myFile;
// uint8_t ImageBW[27200];

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect
  }

  Serial.println("Initializing SD card...");
  Serial.println(SPI.begin(SD_SPI_SCK, SD_SPI_CIPO, SD_SPI_COPI, SD_SPI_CS));

  if (!SD.begin(SD_SPI_CS)) {
    Serial.println("Initialization failed!");
    while (1);
  }
  Serial.println("Initialization Done.");

  myFile = SD.open("test.txt", FILE_WRITE);

  if (myFile) {
    Serial.print("Writing to test.text...");
    myFile.println("testing 1 2 3 4");
    myFile.close();
    Serial.println("Writing done.");
  }
  else {
    Serial.println("Error opening test.txt!");
  }

  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  }
  else {
    Serial.println("Error opening test.txt!");
  }



  // Turn on display
  // pinMode(DISP_POWER, OUTPUT);
  // digitalWrite(DISP_POWER, HIGH);

  // EPD_GPIOInit();
  // Paint_NewImage(ImageBW, DISPLAY_WIDTH, DISPLAY_HEIGHT, ROTATION, WHITE);
  // Paint_Clear(WHITE);

  // clear_all();

  // EPD_FastMode1Init();

  // // Draw content here
  // EPD_Display(ImageBW);
  // EPD_FastUpdate();
  // EPD_DeepSleep();
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