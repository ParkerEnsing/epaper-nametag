#include <Arduino.h>

#include "DevBoardIO.h"
#include "displays/drivers/EPaperDisplay.h"
#include <lvgl.h>
#include "displays/ui/UIRuntime.h"

#include "input/InputService.h"
#include "input/InputEvent.h"

#include "config/ConfigManager.h"
#include "services/SystemController.h"
#include "services/StorageService.h"


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


void testStorage() {
    StorageService::DirectoryEntry entries[16];

    size_t count = 0;
    bool truncated = false;

    if (!StorageService::listDirectory("/", entries, 16, count, truncated)) {
        Serial.println("Failed to read SD root directory.");
        return;
    }

    Serial.println("SD root directory:");

    for (size_t i = 0; i < count; i++) {
        const auto &entry = entries[i];
        Serial.printf("%s %s %llu bytes\n", entry.isDirectory ? "[DIR]" : "[FILE]", entry.name, static_cast<unsigned long long>(entry.sizeBytes));
    }

    if (truncated) {
        Serial.println("Additional entries not displayed.");
    }
}


void setup() {
    Serial.begin(115200);

    EPD.begin(); // Initializes eInk hardware
    UIRuntime::begin(&EPD); // Hands eInk control over to the render pipeline

    InputService::begin(); // Sets hardware input pins and initializes input states

    StorageService::begin(); // Initializes TF reader
    testStorage();

    ConfigManager::begin(); // Initializes the system's data model
    SystemController::begin(); // Spins up app system and starts the boot sequence

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