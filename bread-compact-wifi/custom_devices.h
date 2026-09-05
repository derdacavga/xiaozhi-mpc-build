#pragma once
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <led_strip.h>
#include "config.h"

class DeviceController {
private:
    inline static led_strip_handle_t led_strip_ = nullptr;

public:
    static void init();
    static void setRelay(bool state);
    static void setServoAngle(int angle);
    static void setLampBrightness(int percent);
    static void setRgbColor(int r, int g, int b);
    static void clearRgb();
};

void RegisterCustomMcpTools();