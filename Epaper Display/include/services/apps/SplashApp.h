#ifndef SPLASH_APP_H
#define SPLASH_APP_H

#include "services/ScreenApp.h"


class SplashApp: public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;
    private:
        void createScreen() override;
};


#endif