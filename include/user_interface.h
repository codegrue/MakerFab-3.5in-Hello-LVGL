#include <LVGL.h>
#include "graphics.h"

static const int bufferSize = SCREEN_WIDTH * SCREEN_HEIGHT / 10;

// Calibration extents
#define TOUCH_X_MIN 52
#define TOUCH_X_MAX 464
#define TOUCH_Y_MIN 25
#define TOUCH_Y_MAX 314

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[bufferSize];
static lv_color_t buf2[bufferSize];

namespace UserInterface
{
    void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
    void setup_lvgl();
    void setup_lovyan_GFX();
    void setup_ui();
    void increment_counter(lv_event_t *event);
}