#include "ui/screens/ProductSplashScreen.h"
#include "ui/UITheme.h"


lv_obj_t* ProductSplashScreen::create() {
    lv_obj_t* screen = lv_obj_create(nullptr);
    lv_obj_add_style(screen, UITheme::screen(), LV_PART_MAIN);

    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "Product Splash Screen");
    lv_obj_add_style(title, UITheme::titleLabel(), LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 8);

    lv_obj_t* label = lv_label_create(screen);
    lv_label_set_text(label, "[Logo, name, maybe version]");
    lv_obj_add_style(label, UITheme::bodyLabel(), LV_PART_MAIN);
    lv_obj_center(label);

    return screen;
}