#ifndef SETTINGS_APP
#define SETTINGS_APP

#include "services/ScreenApp.h"


class SettingsApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;
    private:
        void createScreen() override;
};

#endif