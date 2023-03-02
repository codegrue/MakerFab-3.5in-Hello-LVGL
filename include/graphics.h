#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <LVGL.h>

// LCD panel pins
#define LCD_CS 37
#define LCD_BLK 45

// Touch panel pins
#define TOUCH_I2C_ADD 0x38
#define I2C_SCL 39
#define I2C_SDA 38

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

// Calibration extents
#define TOUCH_X_MIN 52
#define TOUCH_X_MAX 464
#define TOUCH_Y_MIN 25
#define TOUCH_Y_MAX 314

static const int bufferSize = SCREEN_WIDTH * SCREEN_HEIGHT / 10;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[bufferSize];
static lv_color_t buf2[bufferSize];

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ILI9488 _panel_instance;
    lgfx::Bus_Parallel16 _bus_instance; // 8-bit parallel bus instance (ESP32 only)
    lgfx::Touch_FT5x06 touch_instance;

public:
    LGFX(void);
};

namespace Graphics
{
    void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
    void setup_lvgl();
    void setup_lovyan_GFX();
}