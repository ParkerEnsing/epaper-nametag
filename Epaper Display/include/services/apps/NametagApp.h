#ifndef NAMETAG_APP
#define NAMETAG_APP

#include "services/ScreenApp.h"

class NametagApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;
    private:
        void createScreen() override;
};

#endif