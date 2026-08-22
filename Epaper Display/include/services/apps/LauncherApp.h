#ifndef LAUNCHER_APP_H
#define LAUNCHER_APP_H


#include "services/ScreenApp.h"
// #include <stddef.h>


class LauncherApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;

        void onInput(AppContext &context, const InputEvent &event) override;
    private:
        void createScreen() override;
        void onScreenEnter(AppContext &context) override;
        void updateListText();

        void moveSelection(uint32_t delta);
        void launchSelected(AppContext &context);

        static constexpr size_t TEXT_BUFFER_SIZE = 512;

        lv_obj_t* _titleLabel = nullptr;
        lv_obj_t* _listLabel = nullptr;
        lv_obj_t* _hintLabel = nullptr;

        size_t _selectedIndex = 0;

        char _textBuffer[TEXT_BUFFER_SIZE] = {};
};


#endif