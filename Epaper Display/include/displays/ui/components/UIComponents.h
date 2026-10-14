#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H


#include <lvgl.h>


namespace MFDButton {
    lv_obj_t* create(lv_obj_t* parent, const char* text);
}


namespace MFDButtonMatrix {
    lv_obj_t* create(lv_obj_t* parent, const char* const* labelMap, const lv_buttonmatrix_ctrl_t* controlMap);
}


#endif