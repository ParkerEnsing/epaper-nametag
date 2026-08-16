#ifndef DIAGNOSTIC_APP_H
#define DIAGNOSTIC_APP_H


#include "services/App.h"


class DiagnosticApp : public App {
    public:
        const char* id() const override;
        const char* name() const override;

        void onEnter(AppContext &context) override;

        lv_obj_t* screen() const override;
    private:
        void createScreen();
        void updateText();

        lv_obj_t* _screen = nullptr;
        lv_obj_t* _titleLabel = nullptr;
        lv_obj_t* _bodyLabel = nullptr;

        char _textBuffer[512] = {};
};


#endif