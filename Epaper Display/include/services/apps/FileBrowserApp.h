#ifndef FILE_BROWSER_APP
#define FILE_BROWSER_APP

#include "services/ScreenApp.h"

class FileBrowserApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;
    private:
        void createScreen() override;
};

#endif