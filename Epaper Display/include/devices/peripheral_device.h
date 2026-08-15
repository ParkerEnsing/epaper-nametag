#ifndef PERIPHERAL_DEVICE_H
#define PERIPHERAL_DEVICE_h

#include <lvgl.h>

class PeripheralDevice {
    public:
        virtual void begin() const = 0;
        virtual void end() const = 0;
        virtual void stateCallBack(lv_indev_t* indev, lv_indev_data_t* data) const = 0;
        virtual ~PeripheralDevice() = default;
    private:
};

#endif