#ifndef FILE_BROWSER_APP_H
#define FILE_BROWSER_APP_H

#include "services/ScreenApp.h"

class FileBrowserApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;
    private:
        void createScreen() override;
};

#endif