#ifndef DISPLAY_TEST_APP
#define DISPLAY_TEST_APP

#include "services/ScreenApp.h"


class DisplayTestApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;
    private:
        void createScreen() override;
};


#endif