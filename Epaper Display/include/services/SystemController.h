#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H


#include <stdint.h>


class App;
struct InputEvent;

enum class SystemMode {
    BootSequence,
    Home,
    AppRunning,
    Launcher,
    Slideshow,
    Settings,
    Error,
};


namespace SystemController {
    void begin();

    void update(uint32_t nowMs);
    void handleInput(const InputEvent &event);

    bool launchApp(const char* appId);
    bool goHome();

    void openLauncher();
    void startSlideshow();
    void stopSlideshow();

    bool setHomeAppId(const char* appId);
    const char* homeAppId();

    bool shouldCommit();
    void clearCommitRequest();

    SystemMode mode();
    App* activeApp();
}


#endif