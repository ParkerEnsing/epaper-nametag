#include "services/apps/SplashApp.h"


namespace {
    // for private local functions
}


const char* SplashApp::id() const {
    return "splash";
}


const char* SplashApp::name() const {
    return "Splash Screen";
}


void SplashApp::createScreen() {
    _screen = lv_obj_create(nullptr);

    lv_obj_t* _title = lv_label_create(_screen);
    lv_label_set_text(_title, "Splash Screen");
    lv_obj_center(_title);
}