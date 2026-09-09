#include "custom_devices.h"
#include "mcp_server.h"
#include <esp_log.h>

#define TAG "CustomDevices"

void DeviceController::init() {
    gpio_config_t relay_conf = {
        .pin_bit_mask = (1ULL << RELAY_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&relay_conf);
    gpio_set_level(RELAY_GPIO, 0);

    ledc_timer_config_t servo_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_14_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&servo_timer);

    ledc_channel_config_t servo_ch = {
        .gpio_num = SERVO_PWM_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&servo_ch);

    ledc_timer_config_t lamp_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_1,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&lamp_timer);

    ledc_channel_config_t lamp_ch = {
        .gpio_num = LAMP_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&lamp_ch);

    led_strip_config_t strip_config = {
        .strip_gpio_num = WS2812_DATA_GPIO,
        .max_leds = WS2812_LED_NUM,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .flags = { .invert_out = false }
    };
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,
        .mem_block_symbols = 64,
        .flags = { .with_dma = false }
    };
    esp_err_t err = led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip_);
    if (err == ESP_OK) {
        led_strip_clear(led_strip_);
    } else {
        ESP_LOGE(TAG, "Failed to initialize WS2812B RMT driver");
    }
}

void DeviceController::setRelay(bool state) {
    gpio_set_level(RELAY_GPIO, state ? 1 : 0);
}

void DeviceController::setServoAngle(int angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    uint32_t duty = 410 + (angle * (2048 - 410) / 180);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void DeviceController::setLampBrightness(int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    uint32_t duty = (percent * 255) / 100;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}

void DeviceController::clearRgb() {
    if (!led_strip_) return;
    led_strip_clear(led_strip_);
}

void DeviceController::updateRgbStrip() {
    if (!led_strip_) return;
    
    uint8_t r_scaled = (current_r_ * current_rgb_brightness_) / 100;
    uint8_t g_scaled = (current_g_ * current_rgb_brightness_) / 100;
    uint8_t b_scaled = (current_b_ * current_rgb_brightness_) / 100;

    for (int i = 0; i < WS2812_LED_NUM; i++) {
        led_strip_set_pixel(led_strip_, i, r_scaled, g_scaled, b_scaled);
    }
    led_strip_refresh(led_strip_);
}

void DeviceController::setRgbColor(int r, int g, int b) {
    current_r_ = (uint8_t)r;
    current_g_ = (uint8_t)g;
    current_b_ = (uint8_t)b;
    updateRgbStrip();
}

void DeviceController::setRgbBrightness(int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    current_rgb_brightness_ = percent;
    updateRgbStrip();
}

void RegisterCustomMcpTools() {
    DeviceController::init();

    auto& mcp_server = McpServer::GetInstance();

    mcp_server.AddTool("self.relay.turn_on", "Turn the power relay ON", PropertyList(), 
        [](const PropertyList&) -> ReturnValue {
            DeviceController::setRelay(true);
            return true;
        });

    mcp_server.AddTool("self.relay.turn_off", "Turn the power relay OFF", PropertyList(), 
        [](const PropertyList&) -> ReturnValue {
            DeviceController::setRelay(false);
            return true;
        });

    mcp_server.AddTool("self.servo.set_angle", "Rotate the servo motor to a specific angle (0-180)", 
        PropertyList({ Property("angle", kPropertyTypeInteger, 0, 180) }), 
        [](const PropertyList& properties) -> ReturnValue {
            int angle = properties["angle"].value<int>();
            DeviceController::setServoAngle(angle);
            return true;
        });

    mcp_server.AddTool("self.lamp.turn_on", "Turn on the lamp at full brightness", PropertyList(),
        [](const PropertyList&) -> ReturnValue {
            DeviceController::setLampBrightness(100);
            return true;
        });

    mcp_server.AddTool("self.lamp.turn_off", "Turn off the lamp", PropertyList(),
        [](const PropertyList&) -> ReturnValue {
            DeviceController::setLampBrightness(0);
            return true;
        });

    mcp_server.AddTool("self.lamp.set_brightness", "Set lamp brightness percentage (0 to 100)", 
        PropertyList({ Property("brightness", kPropertyTypeInteger, 0, 100) }), 
        [](const PropertyList& properties) -> ReturnValue {
            int b = properties["brightness"].value<int>();
            DeviceController::setLampBrightness(b);
            return true;
        });

    mcp_server.AddTool("self.rgb.set_color", "Set RGB LED color using 0-255 red, green, blue values", 
        PropertyList({
            Property("r", kPropertyTypeInteger, 0, 255),
            Property("g", kPropertyTypeInteger, 0, 255),
            Property("b", kPropertyTypeInteger, 0, 255)
        }), 
        [](const PropertyList& properties) -> ReturnValue {
            int r = properties["r"].value<int>();
            int g = properties["g"].value<int>();
            int b = properties["b"].value<int>();
            DeviceController::setRgbColor(r, g, b);
            return true;
        });

    mcp_server.AddTool("self.rgb.turn_off", "Turn off the RGB LED", PropertyList(),
        [](const PropertyList&) -> ReturnValue {
            DeviceController::clearRgb();
            return true;
        });

    mcp_server.AddTool("self.rgb.set_brightness", "Set RGB LED brightness percentage (0 to 100)", 
        PropertyList({ Property("brightness", kPropertyTypeInteger, 0, 100) }), 
        [](const PropertyList& properties) -> ReturnValue {
            int b = properties["brightness"].value<int>();
            DeviceController::setRgbBrightness(b);
            return true;
        });
}
