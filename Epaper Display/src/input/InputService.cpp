#include "input/InputService.h"

#include "DevBoardIO.h"

#include <Arduino.h>
#include <stddef.h>


namespace {
    static constexpr uint8_t BUTTON_UP_PIN = ROT_DOWN;
    static constexpr uint8_t BUTTON_DOWN_PIN = ROT_UP;
    static constexpr uint8_t BUTTON_SELECT_PIN = ROT_PRESS;
    static constexpr uint8_t BUTTON_BACK_PIN = EXIT_BUTTON;
    static constexpr uint8_t BUTTON_LAUNCHER_PIN = MENU_BUTTON;

    static constexpr bool BUTTON_ACTIVE_LOW = true; // Change to false if buttons are active high
    static constexpr bool BUTTON_USE_INTERNAL_PULLUPS = false;

    static constexpr uint32_t DEBOUNCE_MS = 35;

    struct ButtonConfig {
        uint8_t pin;
        InputAction action;
    };

    static constexpr ButtonConfig BUTTONS[] = {
        {BUTTON_UP_PIN, InputAction::Up},
        {BUTTON_DOWN_PIN, InputAction::Down},
        {BUTTON_SELECT_PIN, InputAction::Select},
        {BUTTON_BACK_PIN, InputAction::Back},
        {BUTTON_LAUNCHER_PIN, InputAction::OpenLauncher},
    };

    static constexpr size_t BUTTON_COUNT = sizeof(BUTTONS) / sizeof(BUTTONS[0]);

    struct ButtonState {
        bool rawPressed = false;
        bool stablePressed = false;
        uint32_t lastRawChangeMs = 0;
    };

    ButtonState buttonStates[BUTTON_COUNT];

    /*
        Small ring buffer for input events generated between main-loop drains.

        Current five-button, press-only input can generate at most five button
        events in one update pass, so 8 is sufficient. Increase this when release,
        long-press, encoder, or multiple input sources are added.
    */
    static constexpr size_t EVENT_QUEUE_SIZE = 8;

    InputEvent eventQueue[EVENT_QUEUE_SIZE];
    size_t eventQueueReadIndex = 0;
    size_t eventQueueWriteIndex = 0;
    size_t eventQueueCount = 0;


    bool readRawPressed(uint8_t pin) {
        const int level = digitalRead(pin);

        if (BUTTON_ACTIVE_LOW) {
            return (level == LOW);
        }

        return (level == HIGH);
    }


    bool enqueueEvent(const InputEvent &event) {
        if (eventQueueCount >= EVENT_QUEUE_SIZE) {
            return false;
        }

        eventQueue[eventQueueWriteIndex] = event;
        eventQueueWriteIndex = (eventQueueWriteIndex + 1) % EVENT_QUEUE_SIZE;
        eventQueueCount++;

        return true;
    }


    void enqueueButtonPressed(size_t buttonIndex) {
        InputEvent event;
        event.action = BUTTONS[buttonIndex].action;
        event.value = 1; // currently unused; can later be used for enums/switches
        event.source = static_cast<uint8_t>(buttonIndex);

        enqueueEvent(event);
    }
}


void InputService::begin() {
    for (size_t i = 0; i < BUTTON_COUNT; i++) {
        if (BUTTON_USE_INTERNAL_PULLUPS) {
            pinMode(BUTTONS[i].pin, INPUT_PULLUP);
        }
        else {
            pinMode(BUTTONS[i].pin, INPUT);
        }

        const bool pressed = readRawPressed(BUTTONS[i].pin);
        
        buttonStates[i].rawPressed = pressed;
        buttonStates[i].stablePressed = pressed;
        buttonStates[i].lastRawChangeMs = millis();
    }
}


void InputService::update(uint32_t nowMs) {
    for (size_t i = 0; i < BUTTON_COUNT; i++) {
        const bool rawPressed = readRawPressed(BUTTONS[i].pin);
        
        if (rawPressed != buttonStates[i].rawPressed) {
            buttonStates[i].rawPressed = rawPressed;
            buttonStates[i].lastRawChangeMs = nowMs;
        }
            
        const uint32_t timeSinceRawChange = nowMs - buttonStates[i].lastRawChangeMs;
            
        if (timeSinceRawChange < DEBOUNCE_MS) {
            continue;
        }

        if (buttonStates[i].stablePressed != buttonStates[i].rawPressed) {
            buttonStates[i].stablePressed = buttonStates[i].rawPressed;

            if (buttonStates[i].stablePressed) {
                enqueueButtonPressed(i);
            }
        }
    }
}


bool InputService::read(InputEvent &event) {
    if (eventQueueCount == 0) {
        return false;
    }

    event = eventQueue[eventQueueReadIndex];
    eventQueueReadIndex = (eventQueueReadIndex + 1) % EVENT_QUEUE_SIZE;
    eventQueueCount--;

    return true;
}