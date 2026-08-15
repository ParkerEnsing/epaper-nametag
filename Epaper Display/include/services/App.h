#ifndef APP_H
#define APP_H


#include <Arduino.h>
#include <lvgl.h>

class AppContext;
struct InputEvent;

class App {
    public:
        App(const App &) = delete;
        App &operator = (const App &) = delete;

        virtual ~App() = default;

        virtual const char* id() const = 0;
        virtual const char* name() const = 0;

        virtual void onEnter(AppContext &context) = 0;
        virtual void onExit(AppContext &context) {
            (void)context;
        }
        virtual void onUpdate(AppContext &context, uint32_t nowMs) {
            (void)context;
            (void)nowMs;
        }
        virtual void onInput(AppContext &context, const InputEvent &event) {
            (void)context;
            (void)event;
        }
        virtual lv_obj_t* screen() const = 0;
    protected:
        App() = default;
};


#endif