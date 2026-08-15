#ifndef APP_H
#define APP_H


#include <Arduino.h>
#include <lvgl.h>


struct InputEvent;

class App {
    public:
        virtual ~App() = default;
        virtual const char* id() const = 0;
        virtual const char* name() const = 0;
        virtual void onEnter() = 0;
        virtual void onExit() = 0;
        virtual void onUpdate(uint32_t nowMs) = 0;
        virtual void onInput(const InputEvent &event) = 0;
        virtual lv_obj_t* root() = 0;
    private:
};


#endif