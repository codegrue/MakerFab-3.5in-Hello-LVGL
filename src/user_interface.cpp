#include "user_interface.h"

LGFX lcd;

static int counter = 0;
lv_obj_t *counterLabel;
lv_obj_t *led1;

/*** Display callback to flush the buffer to screen ***/
void UserInterface::display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    lcd.startWrite();
    lcd.setAddrWindow(area->x1, area->y1, w, h);
    lcd.writePixels((uint16_t *)&color_p->full, w * h, true);
    lcd.endWrite();

    lv_disp_flush_ready(disp);
}

/*** Touchpad callback to read the touchpad ***/
void UserInterface::touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    lgfx::touch_point_t tp;
    int touched = lcd.getTouch(&tp);

    if (touched == 1)
    {
        data->state = LV_INDEV_STATE_PR;

        int touch_x = tp.x;
        int touch_y = tp.y;

        // calibrate for touch extents
        int calibrated_x = 1.0 * (touch_x - TOUCH_X_MIN) / (TOUCH_X_MAX - TOUCH_X_MIN) * SCREEN_WIDTH;
        calibrated_x = max(0, calibrated_x);
        calibrated_x = min(SCREEN_WIDTH, calibrated_x);
        int calibrated_y = 1.0 * (touch_y - TOUCH_Y_MIN) / (TOUCH_Y_MAX - TOUCH_Y_MIN) * SCREEN_HEIGHT;
        calibrated_y = max(0, calibrated_y);
        calibrated_y = min(SCREEN_HEIGHT, calibrated_y);

        /*Set the coordinates*/
        data->point.x = calibrated_x;
        data->point.y = calibrated_y;

        lv_obj_align(led1, LV_ALIGN_TOP_LEFT, calibrated_x, calibrated_y);
        lv_led_on(led1);

        // Print coordinates, for debugging
        // Serial.println("True  = (" + String(touch_x) + " | " + String(touch_y) + ")");
        // Serial.println("Calib = [" + String(calibrated_x) + " | " + String(calibrated_y) + "]");
        // Serial.println("-----");
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

void UserInterface::setup_lvgl()
{
    String LVGL_Version = "Setting up LVGL, ";
    LVGL_Version += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.println(LVGL_Version);

    lv_init();

    // set up the display buffer
    lv_disp_draw_buf_init(&draw_buf, buf, buf2, bufferSize);

    // Set up LVGL screen draw handler
    static lv_disp_drv_t disp_drv;                    /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                      /*Basic initialization*/
    disp_drv.flush_cb = UserInterface::display_flush; /*Set your driver function*/
    disp_drv.draw_buf = &draw_buf;                    /*Assign the buffer to the display*/
    disp_drv.hor_res = SCREEN_WIDTH;                  /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = SCREEN_HEIGHT;                 /*Set the vertical resolution of the display*/
    lv_disp_drv_register(&disp_drv);                  /*Finally register the driver*/

    // Set up LVGL screen touch handler
    static lv_indev_drv_t indev_drv;                  /*Descriptor of a input device driver*/
    lv_indev_drv_init(&indev_drv);                    /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;           /*Touch pad is a pointer-like device*/
    indev_drv.read_cb = UserInterface::touchpad_read; /*Set your driver function*/
    lv_indev_drv_register(&indev_drv);                /*Finally register the driver*/

    Serial.println("Completed setting up LVGL");
}

void UserInterface::setup_lovyan_GFX()
{
    Serial.println("Setting up LovyanGFX");

    // Set up LCD interface
    pinMode(LCD_CS, OUTPUT);
    pinMode(LCD_BLK, OUTPUT);

    digitalWrite(LCD_CS, LOW);
    digitalWrite(LCD_BLK, HIGH);

    lcd.init();
    lcd.setRotation(1); // Rotate to landscape

    Serial.println("Completed setting up LovyanGFX");
}

void UserInterface::setup_ui()
{

    auto screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x006666), LV_PART_MAIN);

    auto touchPanel = lv_obj_create(screen);
    lv_obj_set_width(touchPanel, 480);
    lv_obj_set_height(touchPanel, 320);
    lv_obj_set_style_bg_opa(touchPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(touchPanel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_pad_left(touchPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(touchPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(touchPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(touchPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    led1 = lv_led_create(touchPanel);
    lv_obj_align(led1, LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_clear_flag(led1, LV_OBJ_FLAG_CLICKABLE);
    lv_led_set_color(led1, lv_palette_main(LV_PALETTE_RED));
    lv_led_off(led1);

    auto widgetPanel = lv_obj_create(screen);
    lv_obj_set_width(widgetPanel, 480);
    lv_obj_set_height(widgetPanel, 320);
    lv_obj_set_style_bg_opa(widgetPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(widgetPanel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(widgetPanel, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_BETWEEN);
    lv_obj_set_style_pad_left(widgetPanel, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(widgetPanel, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(widgetPanel, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(widgetPanel, 30, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *titleLabel = lv_label_create(widgetPanel);
    lv_label_set_text(titleLabel, "Hello MakerFab LVGL");
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    counterLabel = lv_label_create(widgetPanel);
    lv_label_set_text(counterLabel, String(counter).c_str());
    lv_obj_set_style_text_font(counterLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(counterLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    lv_obj_t *button = lv_btn_create(widgetPanel);
    lv_obj_add_event_cb(button, increment_counter, LV_EVENT_CLICKED, NULL);
    lv_obj_t *buttonLabel = lv_label_create(button);
    lv_label_set_text(buttonLabel, "Increment");
    lv_obj_set_style_text_font(buttonLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void UserInterface::increment_counter(lv_event_t *event)
{
    counter++;
    lv_label_set_text(counterLabel, String(counter).c_str());
}