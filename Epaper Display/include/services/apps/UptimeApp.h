#ifndef UPTIME_APP_H
#define UPTIME_APP_H


#include "services/App.h"


class UptimeApp : public App {
    public:
        const char* id() const override;
        const char* name() const override;

        void onEnter(AppContext &context) override;
        void onUpdate(AppContext &context, uint32_t nowMs) override;

        lv_obj_t* screen() const override;
    private:
        void createScreen();
        void updateText(uint32_t nowMs);

        static constexpr uint32_t UPDATE_INTERVAL_MS = 5000;

        lv_obj_t* _screen = nullptr;
        lv_obj_t* _titleLabel = nullptr;
        lv_obj_t* _uptimeLabel = nullptr;
        lv_obj_t* _hintLabel = nullptr;

        uint32_t _lastUpdateMs = 0;
        uint32_t _updateCount = 0;

        char _textBuffer[128] = {};
};

#endif