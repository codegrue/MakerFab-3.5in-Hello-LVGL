#include "user_interface.h"

LGFX lcd;

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
    int touchPosition[2] = {0, 0};
    int touched = ft6236_pos(touchPosition);

    // uint16_t touchX, touchY;
    // bool touched = lcd.getTouch(&touchX, &touchY);

    if (touched == 1)
    {
        int touch_x = touchPosition[1];
        int touch_y = SCREEN_HEIGHT - touchPosition[0];

        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touch_x;
        data->point.y = touch_y;

        // Print coordinates, for debugging
        // Serial.print("(");
        // Serial.print(touch_x);
        // Serial.print(" | ");
        // Serial.print(touch_y);
        // Serial.println(")");
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

void UserInterface::setup_lovyan_GFX()
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

void UserInterface::setup_graphics()
{
    Serial.println("Setting up LovyanGFX");

    // Set up LCD interface
    pinMode(LCD_CS, OUTPUT);
    pinMode(LCD_BLK, OUTPUT);

    digitalWrite(LCD_CS, LOW);
    digitalWrite(LCD_BLK, HIGH);

    lcd.init();
    lcd.setRotation(1); // Rotate to landscape

    // Set up touch I2C initerface
    Wire.begin(I2C_SDA, I2C_SCL);
    byte error, address;
    Wire.beginTransmission(TOUCH_I2C_ADD);
    error = Wire.endTransmission();

    if (error == 0)
    {
        Serial.print("I2C device found at address 0x");
        Serial.print(TOUCH_I2C_ADD, HEX);
        Serial.println("  !");
    }
    else
    {
        Serial.print("Unknown error at address 0x");
        Serial.println(TOUCH_I2C_ADD, HEX);
    }

    Serial.println("Completed setting up LovyanGFX");
}