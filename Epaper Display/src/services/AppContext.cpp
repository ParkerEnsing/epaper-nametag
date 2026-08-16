#include "services/AppContext.h"


void AppContext::clear() {
    clearCommitRequest();
    clearCommand();
}


void AppContext::requestCommit() {
    _commitRequested = true;
}


bool AppContext::isCommitRequested() const {
    return _commitRequested;
}


void AppContext::clearCommitRequest() {
    _commitRequested = false;
}


void AppContext::requestLaunchApp(const char* appID) {
    setCommand(AppCommandType::LaunchApp, appID);
}


void AppContext::requestGoHome() {
    setCommand(AppCommandType::GoHome);
}


void AppContext::requestOpenLauncher() {
    setCommand(AppCommandType::OpenLauncher);
}


void AppContext::requestStartSlideshow() {
    setCommand(AppCommandType::StartSlideshow);
}


void AppContext::requestStopSlideshow() {
    setCommand(AppCommandType::StopSlideshow);
}


bool AppContext::hasCommand() const {
    return _command.type != AppCommandType::None;
}


AppCommand AppContext::command() const {
    return _command;
}


void AppContext::clearCommand() {
    _command.type = AppCommandType::None;
    _command.appId = nullptr;
}


void AppContext::setCommand(AppCommandType type, const char* appId) {
    _command.type = type;
    _command.appId = appId;
}