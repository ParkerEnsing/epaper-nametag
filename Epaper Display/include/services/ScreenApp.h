#ifndef SCREEN_APP_H
#define SCREEN_APP_H


#include "services/App.h"


class ScreenApp : public App {
    public:
        void onEnter(AppContext &context) override;
        lv_obj_t* screen() const override;
    protected:
        virtual void createScreen() = 0;
        virtual void onScreenEnter(AppContext &context) {
            (void)context;
        }

        lv_obj_t* _screen = nullptr;
};


#endif