#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include <stdint.h>

enum class AppCommandType {
    None,
    LaunchApp,
    GoHome,
    OpenLauncher,
    StartSlideshow,
    StopSlideshow,
};

struct AppCommand {
    AppCommandType type = AppCommandType::None;
    const char* appID = nullptr;
};

class AppContext {
    public:
        void clear();

        void requestCommit();
        bool isCommitRequested() const;
        void clearCommitRequest();

        void requestLaunchApp(const char* appID);
        void requestGoHome();
        void requestOpenLauncher();
        void requestStartSlideshow();
        void requestStopSlideshow();

        bool hasCommand() const;
        AppCommand command() const;
        void clearCommand();
    private:
        void setCommand(AppCommandType type, const char* appId = nullptr);

        bool _commitRequested = false;
        AppCommand _command;
};

#endif
