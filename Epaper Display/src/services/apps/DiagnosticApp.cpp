#include "services/apps/DiagnosticApp.h"
#include "services/AppContext.h"
#include "displays/ui/UITheme.h"


const char* DiagnosticApp::id() const {
    return "diagnostic";
}


const char* DiagnosticApp::name() const {
    return "Diagnostic";
}


void DiagnosticApp::createScreen() {
    _screen = lv_obj_create(nullptr);
    lv_obj_add_style(_screen, UITheme::screen(), LV_PART_MAIN);

    _titleLabel = lv_label_create(_screen);
    lv_label_set_text(_titleLabel, name());
    lv_obj_add_style(_titleLabel, UITheme::titleLabel(), LV_PART_MAIN);
    lv_obj_align(_titleLabel, LV_ALIGN_TOP_MID, 0, 8);

    _bodyLabel = lv_label_create(_screen);
    lv_obj_add_style(_bodyLabel, UITheme::bodyLabel(), LV_PART_MAIN);
    lv_obj_set_width(_bodyLabel, 760);
    lv_label_set_long_mode(_bodyLabel, LV_LABEL_LONG_WRAP);
    lv_obj_align(_bodyLabel, LV_ALIGN_TOP_LEFT, 16, 48);
}


void DiagnosticApp::onScreenEnter(AppContext &context) {
    (void)context;
    updateText();
}


void DiagnosticApp::updateText() {
    snprintf(
        _textBuffer,
        sizeof(_textBuffer),
        "LVGL: %d.%d.%d\n"
        "PSRAM: %s\n"
        "PSRAM size: %lu bytes\n"
        "Free heap: %lu bytes\n"
        "Free PSRAM: %lu bytes\n"
        "Flash size: %lu bytes\n"
        "CPU frequency: %lu MHz\n"
        "SDK: %s",
        lv_version_major(),
        lv_version_minor(),
        lv_version_patch(),
        psramFound() ? "Okay" : "FAIL",
        static_cast<unsigned long>(ESP.getPsramSize()),
        static_cast<unsigned long>(ESP.getFreeHeap()),
        static_cast<unsigned long>(ESP.getFreePsram()),
        static_cast<unsigned long>(ESP.getFlashChipSize()),
        static_cast<unsigned long>(ESP.getCpuFreqMHz()),
        ESP.getSdkVersion()
    );

    lv_label_set_text(_bodyLabel, _textBuffer);
}