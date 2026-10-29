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


void FileBrowserApp::onInput(AppContext &context, const InputEvent &event) {}


void FileBrowserApp::createScreen() {
    _screen = lv_obj_create(nullptr);
}


void FileBrowserApp::onScreenEnter(AppContext &context) {}


bool FileBrowserApp::loadPage(const char* path, size_t offset) {}


bool FileBrowserApp::changeDirectory(const char* path) {}


bool FileBrowserApp::moveSelection(int32_t direction) {}


bool FileBrowserApp::openSelection() {}


bool FileBrowserApp::goToParent() {}


void FileBrowserApp::renderView() {}