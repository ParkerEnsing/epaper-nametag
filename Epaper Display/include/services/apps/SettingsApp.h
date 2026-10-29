#ifndef SETTINGS_APP_H
#define SETTINGS_APP_H

#include "services/ScreenApp.h"


class SettingsApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;
    private:
        void createScreen() override;
};

#endif