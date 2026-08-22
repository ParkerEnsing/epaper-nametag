#ifndef MAIN_MENU_APP_H
#define MAIN_MENU_APP_H


#include "services/ScreenApp.h"


class MainMenuApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;

        void onInput(AppContext &context, const InputEvent &event) override;
    private:
        void createScreen() override;
        void onScreenEnter(AppContext &context) override;
};


#endif