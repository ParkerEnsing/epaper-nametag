#include "services/apps/NametagApp.h"

#include "displays/ui/UITheme.h"

namespace {
    // for local private functions
}


const char* NametagApp::id() const {
    return "nametag";
}


const char* NametagApp::name() const {
    return "Nametag";
}


void NametagApp::createScreen() {
    _screen = lv_obj_create(nullptr);

    lv_obj_t* flexContainer = lv_obj_create(_screen);
    lv_obj_center(flexContainer);
    lv_obj_set_flex_flow(flexContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(flexContainer, lv_pct(100), lv_pct(100));

    lv_obj_t* _fullNameLabel = lv_label_create(flexContainer);
    lv_label_set_text(_fullNameLabel, "Parker Ensing");
    lv_obj_add_style(_fullNameLabel, UITheme::titleLabel(), LV_PART_MAIN);
    
    lv_obj_t* _primaryDescriptionLabel = lv_label_create(flexContainer);
    lv_label_set_text(_primaryDescriptionLabel, "Lead Spacecraft Systems Engineer");
    lv_obj_add_style(_primaryDescriptionLabel, UITheme::bodyLabel(), LV_PART_MAIN);

    lv_obj_t* _secondaryDescriptionLabel = lv_label_create(flexContainer);
    lv_label_set_text(_secondaryDescriptionLabel, "Engineering, SEIT");
    lv_obj_add_style(_secondaryDescriptionLabel, UITheme::bodyLabel(), LV_PART_MAIN);
}