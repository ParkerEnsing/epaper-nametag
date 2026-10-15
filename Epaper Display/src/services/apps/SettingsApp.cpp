#include "services/apps/SettingsApp.h"


const char* SettingsApp::id() const {
    return "settings";
}


const char* SettingsApp::name() const {
    return "Settings";
}


void SettingsApp::createScreen() {
    _screen = lv_obj_create(nullptr);
}