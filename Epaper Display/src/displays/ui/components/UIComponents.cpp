#include "displays/ui/components/UIComponents.h"
#include "displays/ui/UITheme.h"


lv_obj_t* MFDButton::create(lv_obj_t* parent, const char* text) {
    lv_obj_t* mfdButton = lv_obj_create(parent);

    lv_obj_t* button = lv_button_create(mfdButton);

    lv_obj_t* label = lv_label_create(button);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_label_set_text(label, text);

    return mfdButton;
}


lv_obj_t* MFDButtonMatrix::create(lv_obj_t* parent, const char* const* labelMap, const lv_buttonmatrix_ctrl_t* controlMap) {
    lv_obj_t* mfdButtonMatrix = lv_buttonmatrix_create(parent);
    lv_obj_set_size(mfdButtonMatrix, lv_pct(100), lv_pct(10));

    lv_buttonmatrix_set_map(mfdButtonMatrix, labelMap);
    lv_buttonmatrix_set_ctrl_map(mfdButtonMatrix, controlMap);

    return mfdButtonMatrix;
}