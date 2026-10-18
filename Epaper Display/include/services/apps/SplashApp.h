#ifndef SPLASH_APP
#define SPLASH_APP

#include "services/ScreenApp.h"


class SplashApp: public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;
    private:
        void createScreen() override;
};


#endif