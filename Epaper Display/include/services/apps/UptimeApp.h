#ifndef UPTIME_APP_H
#define UPTIME_APP_H


#include "services/ScreenApp.h"


class UptimeApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;

        void onUpdate(AppContext &context, uint32_t nowMs) override;
    private:
        void createScreen() override;
        void onScreenEnter(AppContext &context) override;
        void updateText(uint32_t nowMs);

        static constexpr uint32_t UPDATE_INTERVAL_MS = 5000;

        lv_obj_t* _titleLabel = nullptr;
        lv_obj_t* _uptimeLabel = nullptr;
        lv_obj_t* _hintLabel = nullptr;

        uint32_t _lastUpdateMs = 0;
        uint32_t _updateCount = 0;

        char _textBuffer[128] = {};
};

#endif