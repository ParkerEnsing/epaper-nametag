#include <Arduino.h>

// #include "assets/test_images/pic_home.h"

#include "DevBoardIO.h"
#include "displays/drivers/EPaperDisplay.h"
// #include <lvgl.h>
#include "displays/ui/UIRuntime.h"

#include "input/InputService.h"
#include "input/InputEvent.h"

#include "services/SystemController.h"


EPaperDisplay EPD;


static bool inputEventFromSerialChar(char c, InputEvent &event) {
    switch (c) {
        case 'l':
        case 'L':
            event.action = InputAction::OpenLauncher;
            return true;
        case 'u':
        case 'U':
            event.action = InputAction::Up;
            return true;
        case 'd':
        case 'D':
            event.action = InputAction::Down;
            return true;
        case 'e':
        case 'E':
            event.action = InputAction::Select;
            return true;
        case 'b':
        case 'B':
            event.action = InputAction::Back;
            return true;
        case 'h':
        case 'H':
            event.action = InputAction::Home;
            return true;
        default:
            return false;
    }
}


static void processSerialDebugInput() {
    while (Serial.available() > 0) {
        const char c = static_cast<char>(Serial.read());

        if (c == '\n' || c == '\r') {
            continue;
        }

        InputEvent event;

        if (inputEventFromSerialChar(c, event)) {
            SystemController::handleInput(event);
        }
        else {
            Serial.print("Unknown input: ");
            Serial.println(c);
        }
    }
}


void setup() {
    Serial.begin(115200);

    EPD.begin();
    UIRuntime::begin(&EPD);
    InputService::begin();
    SystemController::begin();

    if (SystemController::shouldCommit()) {
        UIRuntime::commit();
        SystemController::clearCommitRequest();
    }

    Serial.println("Setup complete.");
}


void loop() {
    const uint32_t nowMs = millis();
    UIRuntime::service();

    InputService::update(nowMs);
    InputEvent event;

    while (InputService::read(event)) {
        SystemController::handleInput(event);
    }

    //processSerialDebugInput();

    SystemController::update(nowMs);

    if (SystemController::shouldCommit()) {
        UIRuntime::commit();
        SystemController::clearCommitRequest();
    }
    
    delay(5);
}