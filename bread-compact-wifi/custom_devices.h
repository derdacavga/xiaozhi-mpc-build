#pragma once
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <led_strip.h>
#include "config.h"

class DeviceController {
private:
    inline static led_strip_handle_t led_strip_ = nullptr;
    inline static int current_lamp_brightness_ = 0;
    
    inline static uint8_t current_r_ = 255;
    inline static uint8_t current_g_ = 255;
    inline static uint8_t current_b_ = 255;
    inline static int current_rgb_brightness_ = 100;

    static void updateRgbStrip();

public:
    static void init();
    static void setRelay(bool state);
    static void setServoAngle(int angle);
    static void setLampBrightness(int percent);
    static void setRgbColor(int r, int g, int b);
    static void setRgbBrightness(int percent);
    static void clearRgb();
};

void RegisterCustomMcpTools();
