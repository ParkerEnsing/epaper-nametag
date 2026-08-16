#include "services/apps/UptimeApp.h"
#include "services/AppContext.h"
#include "displays/ui/UITheme.h"


const char* UptimeApp::id() const {
    return "uptime";
}


const char* UptimeApp::name() const {
    return "Uptime";
}



void UptimeApp::onEnter(AppContext &context) {
    if (_screen == nullptr) {
        createScreen();
    }

    _lastUpdateMs = millis();
    updateText(_lastUpdateMs);

    context.requestCommit();
}


void UptimeApp::onUpdate(AppContext &context, uint32_t nowMs) {
    if (nowMs - _lastUpdateMs < UPDATE_INTERVAL_MS) {
        return;
    }

    _lastUpdateMs = nowMs;
    _updateCount++;

    updateText(nowMs);

    context.requestCommit();
}


lv_obj_t* UptimeApp::screen() const {
    return _screen;
}


void UptimeApp::createScreen() {
    _screen = lv_obj_create(nullptr);
    lv_obj_add_style(_screen, UITheme::screen(), LV_PART_MAIN);

    _titleLabel = lv_label_create(_screen);
    lv_label_set_text(_titleLabel, name());
    lv_obj_add_style(_titleLabel, UITheme::titleLabel(), LV_PART_MAIN);
    lv_obj_align(_titleLabel, LV_ALIGN_TOP_MID, 0, 8);

    _uptimeLabel = lv_label_create(_screen);
    lv_obj_add_style(_titleLabel, UITheme::bodyLabel(), LV_PART_MAIN);
    lv_obj_align(_titleLabel, LV_ALIGN_CENTER, 0, -8);

    _hintLabel = lv_label_create(_screen);
    lv_label_set_text(_hintLabel, "Updates every 5 seconds.");
    lv_obj_add_style(_hintLabel, UITheme::bodyLabel(), LV_PART_MAIN);
    lv_obj_align(_hintLabel, LV_ALIGN_BOTTOM_MID, 0, -12);
}


void UptimeApp::updateText(uint32_t nowMs) {
    const uint32_t totalSeconds = nowMs / 1000;
    const uint32_t minutes = totalSeconds / 60;
    const uint32_t seconds = totalSeconds % 60;

    snprintf(
        _textBuffer,
        sizeof(_textBuffer),
        "Uptime: %lu:%02lu\nUpdates: %lu",
        static_cast<unsigned long>(minutes),
        static_cast<unsigned long>(seconds),
        static_cast<unsigned long>(_updateCount)
    );

    lv_label_set_text(_uptimeLabel, _textBuffer);
}