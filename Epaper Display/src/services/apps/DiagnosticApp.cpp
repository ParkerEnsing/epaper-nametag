#include "services/apps/DiagnosticApp.h"
#include "services/AppContext.h"
#include "displays/ui/UITheme.h"


namespace {
    // for private local functions

}


const char* DiagnosticApp::id() const {
    return "diagnostic";
}


const char* DiagnosticApp::name() const {
    return "Diagnostic";
}


void DiagnosticApp::createScreen() {
    _screen = lv_obj_create(nullptr);
    lv_obj_add_style(_screen, UITheme::screen(), LV_PART_MAIN);

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
    uint32_t freePsram = ESP.getFreePsram();
    uint32_t totalPsram = ESP.getPsramSize();
    uint32_t freeHeap = ESP.getFreeHeap();
    uint32_t totalHeap = ESP.getHeapSize();

    snprintf(
        _textBuffer,
        sizeof(_textBuffer),
        "MCU: %s, Rev %d\n"
        "Cores: %d\n"
        "CPU frequency: %lu MHz\n"
        "SDK Ver.: %s\n"
        "PSRAM: %lu KiB / %lu KiB (%4.1f %% free)\n"
        "Heap: %lu KiB / %lu KiB (%4.1f %% free)\n"
        "Flash size: %lu KiB\n",
        ESP.getChipModel(),
        ESP.getChipRevision(),
        ESP.getChipCores(),
        ESP.getCpuFreqMHz(),
        ESP.getSdkVersion(),
        freePsram / 1024,
        totalPsram / 1024,
        static_cast<float>(freePsram) / static_cast<float>(totalPsram) * 100.0,
        freeHeap / 1024,
        totalHeap / 1024,
        static_cast<float>(freeHeap) / static_cast<float>(totalHeap) * 100.0,
        ESP.getFlashChipSize() / 1024
    );

    lv_label_set_text(_bodyLabel, _textBuffer);
}