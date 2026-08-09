#include "ui/UITheme.h"


namespace {
    lv_style_t styleScreen;
    lv_style_t styleTitleLabel;
    lv_style_t styleBodyLabel;
    lv_style_t stylePanel;
    lv_style_t styleButton;
    lv_style_t styleButtonFocused;

    bool initialized = false;
}


void UITheme::init() {
    if(initialized) {
        return;
    }

    initialized = true;

    // Screen style
    lv_style_init(&styleScreen);
    lv_style_set_bg_color(&styleScreen, lv_color_white());
    lv_style_set_bg_opa(&styleScreen, LV_OPA_COVER);
    lv_style_set_text_color(&styleScreen, lv_color_black());
    lv_style_set_pad_all(&styleScreen, 0);
    lv_style_set_border_width(&styleScreen, 0);
    lv_style_set_radius(&styleScreen, 0);

    // Title label style
    lv_style_init(&styleTitleLabel);
    lv_style_set_text_color(&styleTitleLabel, lv_color_black());
    lv_style_set_text_font(&styleTitleLabel, LV_FONT_DEFAULT);
    lv_style_set_pad_all(&styleTitleLabel, 0);

    // Body label style
    lv_style_init(&styleBodyLabel);
    lv_style_set_text_color(&styleBodyLabel, lv_color_black());
    lv_style_set_text_font(&styleBodyLabel, LV_FONT_DEFAULT);
    lv_style_set_pad_all(&styleBodyLabel, 0);

    // Panel/card style
    lv_style_init(&stylePanel);
    lv_style_set_bg_color(&stylePanel, lv_color_white());
    lv_style_set_bg_opa(&stylePanel, LV_OPA_COVER);
    lv_style_set_border_color(&stylePanel, lv_color_black());
    lv_style_set_border_width(&stylePanel, 1);
    lv_style_set_radius(&stylePanel, 0);
    lv_style_set_pad_all(&stylePanel, 6);

    // Button style
    lv_style_init(&styleButton);
    lv_style_set_bg_color(&styleButton, lv_color_white());
    lv_style_set_bg_opa(&styleButton, LV_OPA_COVER);
    lv_style_set_border_color(&styleButton, lv_color_black());
    lv_style_set_border_width(&styleButton, 1);
    lv_style_set_radius(&styleButton, 0);
    lv_style_set_pad_top(&styleButton, 4);
    lv_style_set_pad_bottom(&styleButton, 4);
    lv_style_set_pad_left(&styleButton, 8);
    lv_style_set_pad_right(&styleButton, 8);
    lv_style_set_text_color(&styleButton, lv_color_black());

    // Focused/selected button style
    lv_style_init(&styleButtonFocused);
    lv_style_set_bg_color(&styleButtonFocused, lv_color_black());
    lv_style_set_bg_opa(&styleButtonFocused, LV_OPA_COVER);
    lv_style_set_border_color(&styleButtonFocused, lv_color_black());
    lv_style_set_border_width(&styleButtonFocused, 1);
    lv_style_set_radius(&styleButtonFocused, 0);
    lv_style_set_text_color(&styleButtonFocused, lv_color_white());
}


lv_style_t* UITheme::screen() {
    return &styleScreen;
}


lv_style_t* UITheme::titleLabel() {
    return &styleTitleLabel;
}


lv_style_t* UITheme::bodyLabel() {
    return &styleBodyLabel;
}


lv_style_t* UITheme::panel() {
    return &stylePanel;
}


lv_style_t* UITheme::button() {
    return &styleButton;
}


lv_style_t* UITheme::buttonFocused() {
    return &styleButtonFocused;
}