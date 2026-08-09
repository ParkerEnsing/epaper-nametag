#ifndef UI_THEME_H
#define UI_THEME_H

#include <lvgl.h>

namespace UITheme
{
    void init();

    lv_style_t* screen();
    lv_style_t* titleLabel();
    lv_style_t* bodyLabel();
    lv_style_t* panel();
    lv_style_t* button();
    lv_style_t* buttonFocused();
}

#endif