#ifndef DIAGNOSTIC_APP_H
#define DIAGNOSTIC_APP_H


#include "services/ScreenApp.h"


class DiagnosticApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;
    private:
        void createScreen();
        void onScreenEnter(AppContext &context) override;
        void updateText();

        lv_obj_t* _titleLabel = nullptr;
        lv_obj_t* _bodyLabel = nullptr;

        char _textBuffer[512] = {};
};


#endif