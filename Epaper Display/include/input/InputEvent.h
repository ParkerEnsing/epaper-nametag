#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H


#include <stdint.h>


enum class InputAction {
    None,

    Up,
    Down,
    Left,
    Right,

    Select,
    Back,

    Home,
    OpenLauncher,

    StartSlideshow,
    StopSlideshow,
};


struct InputEvent {
    InputAction action = InputAction::None;

    /*
        Optional value for future use.

        Examples:
            encoder delta: +1 or -1
            slider value
            repeated button count
    */
    int32_t value = 0;

    /*
        Optional source ID for future hardware mapping.

        Examples:
            button 0
            button 1
            encoder 0
    */
   uint8_t source = 0;
};


#endif