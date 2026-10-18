#include "services/apps/DisplayTestApp.h"


namespace {
    // for private local functions


    lv_obj_t* createGrid(lv_obj_t* parent, uint16_t width, uint16_t height, uint16_t rows, uint16_t columns) {
        lv_obj_t* chart = lv_chart_create(parent);
    
        lv_obj_set_size(chart, width, height);
        lv_obj_center(chart);

        lv_obj_remove_flag(chart, LV_OBJ_FLAG_SCROLLABLE); // make the chart unscrollable
        
        lv_obj_set_style_bg_color(chart, lv_color_white(), 0);
        lv_obj_set_style_bg_opa(chart, LV_OPA_COVER, 0);

        lv_obj_set_style_border_width(chart, 1, LV_PART_MAIN);
        lv_obj_set_style_border_color(chart, lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_border_opa(chart, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_border_side(chart, LV_BORDER_SIDE_FULL, LV_PART_MAIN);

        lv_obj_set_style_pad_all(chart, 0, LV_PART_MAIN);
        lv_obj_set_style_radius(chart, 0, LV_PART_MAIN);

        lv_obj_set_style_line_color(chart, lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_line_width(chart, 1, LV_PART_MAIN);
        lv_obj_set_style_line_opa(chart, LV_OPA_COVER, LV_PART_MAIN);

        lv_chart_set_div_line_count(chart, rows + 1, columns + 1);

        return chart;
    }
}


const char* DisplayTestApp::id() const {
    return "display_test";
}


const char* DisplayTestApp::name() const {
    return "EPD Test";
}


void DisplayTestApp::createScreen() {
    _screen = lv_obj_create(nullptr);

    uint16_t grid_width = 792;
    uint16_t grid_height = 272;
    uint16_t grid_rows = 8;
    uint16_t grid_cols = grid_width * grid_rows / grid_height;
    lv_obj_t* _grid = createGrid(_screen, grid_width, grid_height, grid_rows, grid_cols);
}

