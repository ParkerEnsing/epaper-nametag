#include "displays/ui/screens/NametagScreen.h"
#include "displays/ui/UITheme.h"


lv_obj_t* NametagScreen::create() {
    lv_obj_t* screen = lv_obj_create(nullptr);
    lv_obj_add_style(screen, UITheme::screen(), LV_PART_MAIN);

    lv_obj_t* flexContainer = lv_obj_create(screen);
    lv_obj_center(flexContainer);
    lv_obj_set_flex_flow(flexContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(flexContainer, 792, 272);
    lv_obj_set_flex_align(flexContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* title = lv_label_create(flexContainer);
    lv_label_set_text(title, "Parker Ensing");
    lv_obj_add_style(title, UITheme::titleLabel(), LV_PART_MAIN);
    //lv_obj_align(title, LV_ALIGN_LEFT_MID, 0, 8);

    lv_obj_t* label = lv_label_create(flexContainer);
    lv_label_set_text(label, "Lead Spacecraft Systems Engineer");
    lv_obj_add_style(label, UITheme::bodyLabel(), LV_PART_MAIN);
    //lv_obj_center(label);

    return screen;
}