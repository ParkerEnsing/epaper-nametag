#ifndef LAUNCHER_APP_H
#define LAUNCHER_APP_H


#include "services/App.h"
// #include <stddef.h>


class LauncherApp : public App {
    public:
        const char* id() const override;
        const char* name() const override;

        void onEnter(AppContext &context) override;

        lv_obj_t* screen() const override; 
    private:
        void createScreen();
        void updateListText();

        static constexpr size_t TEXT_BUFFER_SIZE = 512;

        lv_obj_t* _screen = nullptr;
        lv_obj_t* _titleLabel = nullptr;
        lv_obj_t* _listLabel = nullptr;
        lv_obj_t* _hintLabel = nullptr;

        size_t _selectedIndex = 0;

        char _textBuffer[TEXT_BUFFER_SIZE] = {};
};


#endif