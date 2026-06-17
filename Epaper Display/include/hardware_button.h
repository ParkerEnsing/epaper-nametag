#ifndef HARDWARE_BUTTON_H
#define HARDWARE_BUTTON_H

#include "peripheral_device.h"

class HardwareButton : public PeripheralDevice {
    public:
        HardwareButton();
        void begin() const override;
        void end() const override;
        void stateCallBack(lv_indev_t* indev, lv_indev_data_t* data) const override;
    private:
};

#endif