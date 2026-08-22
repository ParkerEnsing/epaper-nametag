#include "services/ScreenApp.h"
#include "services/AppContext.h"


lv_obj_t* ScreenApp::screen() const {
    return _screen;
}


void ScreenApp::onEnter(AppContext &context) {
    if(_screen == nullptr) {
        createScreen();
    }

    onScreenEnter(context);
    context.requestCommit();
}