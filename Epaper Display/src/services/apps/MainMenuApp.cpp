#include "services/apps/MainMenuApp.h"
#include "displays/ui/components/UIComponents.h"
#include "assets/test_images/tracks.h"


namespace {
    // for local private functions
}


const char* MainMenuApp::id() const {
    return "main_menu";
}


const char* MainMenuApp::name() const {
    return "Main Menu";
}


void MainMenuApp::onInput(AppContext &context, const InputEvent &event) {

}


void MainMenuApp::createScreen() {
    _screen = lv_obj_create(nullptr);
    //lv_obj_t* tabView = lv_tabview_create(_screen);
    //lv_obj_set_size(tabView, 756, 72);
    //lv_tabview_set_tab_bar_position(tabView, LV_DIR_TOP);

    // Image display example
    lv_obj_t* image = lv_image_create(_screen);
    lv_image_set_src(image, &tracks_image);
    //lv_obj_center(image);

    // _appTitle = lv_label_create(_screen);
    // lv_label_set_text(_appTitle, "Main Menu");
    // lv_obj_set_align(_appTitle, LV_ALIGN_TOP_LEFT);
    // lv_obj_set_size(_appTitle, lv_pct(100), lv_pct(10));

    // lv_obj_t* flexContainer = lv_obj_create(_screen);
    // lv_obj_set_align(flexContainer, LV_ALIGN_CENTER);
    // lv_obj_set_flex_flow(flexContainer, LV_FLEX_FLOW_ROW_WRAP);
    // lv_obj_set_size(flexContainer, lv_pct(100), lv_pct(80));

    // lv_obj_t* canvas = lv_canvas_create(flexContainer);

    // _sectionHeading = lv_label_create(flexContainer);
    // lv_label_set_text(_sectionHeading, "System Info");
    // lv_obj_set_align(_sectionHeading, LV_ALIGN_TOP_MID);

    // static const char* labelMap[] = {
    //     "APP",
    //     "TOOL",
    //     "OPTN",
    //     "BLNK",
    //     "SYS",
    //     "EXIT",
    //     NULL,
    // };
    // static const lv_buttonmatrix_ctrl_t controlMap[] = {
    //     LV_BUTTONMATRIX_CTRL_NONE,
    //     LV_BUTTONMATRIX_CTRL_NONE,
    //     LV_BUTTONMATRIX_CTRL_NONE,
    //     LV_BUTTONMATRIX_CTRL_HIDDEN,
    //     LV_BUTTONMATRIX_CTRL_NONE,
    //     LV_BUTTONMATRIX_CTRL_NONE,
    // };
    // lv_obj_t* buttonMatrix = MFDButtonMatrix::create(_screen, labelMap, controlMap);
    // lv_obj_set_align(buttonMatrix, LV_ALIGN_BOTTOM_MID);
}


void MainMenuApp::onScreenEnter(AppContext &context) {

}