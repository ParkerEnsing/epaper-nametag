#include "services/apps/FileBrowserApp.h"


namespace {
    // for local private functions
}


const char* FileBrowserApp::id() const {
    return "file_browser";
}


const char* FileBrowserApp::name() const {
    return "File Browser";
}


void FileBrowserApp::createScreen() {
    _screen = lv_obj_create(nullptr);
}