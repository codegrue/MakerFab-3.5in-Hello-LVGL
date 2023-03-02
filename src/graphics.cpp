
#include "graphics.h"

LGFX::LGFX(void)
{
  // Configure bus control settings.
  auto bus = _bus_instance.config(); // Get a structure for bus settings.
  bus.port = 0;                      // Select the I2S port to use (0 or 1) (use ESP32's I2S LCD mode)
  bus.freq_write = 20000000;         // Transmit clock (up to 20MHz, rounded to the value obtained by dividing 80MHz by an integer)
  bus.pin_wr = 35;                   // Pin number connecting WR
  bus.pin_rd = 48;                   // Pin number connecting RD
  bus.pin_rs = 36;                   // Pin number connecting RS(D/C)
  bus.pin_d0 = 47;                   // 16 bit settings
  bus.pin_d1 = 21;
  bus.pin_d2 = 14;
  bus.pin_d3 = 13;
  bus.pin_d4 = 12;
  bus.pin_d5 = 11;
  bus.pin_d6 = 10;
  bus.pin_d7 = 9;
  bus.pin_d8 = 3;
  bus.pin_d9 = 8;
  bus.pin_d10 = 16;
  bus.pin_d11 = 15;
  bus.pin_d12 = 7;
  bus.pin_d13 = 6;
  bus.pin_d14 = 5;
  bus.pin_d15 = 4;
  _bus_instance.config(bus);              // Applies the set value to the bus.
  _panel_instance.setBus(&_bus_instance); // Set the bus on the panel.

  // Configure the display panel control.
  auto panel = _panel_instance.config(); // Gets the structure for display panel settings.
  panel.pin_cs = -1;                     // CS to be pulled low
  panel.pin_rst = -1;                    // RST is connected to the development board RST
  panel.pin_busy = -1;                   // Pin number to which BUSY is connected (-1 = disable)
  panel.memory_width = SCREEN_HEIGHT;    // Maximum width supported by the driver IC
  panel.memory_height = SCREEN_WIDTH;    // Maximum height supported by the driver IC
  panel.panel_width = SCREEN_HEIGHT;     // actual displayable width
  panel.panel_height = SCREEN_WIDTH;     // actual visible height
  panel.offset_x = 0;                    // Panel offset amount in X direction
  panel.offset_y = 0;                    // Panel offset amount in Y direction
  panel.offset_rotation = 0;             // Rotation direction value offset 0~7 (4~7 is upside down)
  panel.dummy_read_pixel = 8;            // Number of bits for dummy read before pixel readout
  panel.dummy_read_bits = 1;             // Number of bits for dummy read before non-pixel data read
  panel.readable = true;                 // Set to true if data can be read
  panel.invert = false;                  // Set to true if the light/darkness of the panel is reversed
  panel.rgb_order = false;               // Set to true if the panel's red and blue are swapped
  panel.dlen_16bit = true;               // Set to true for panels that send data length in 16-bit units
  panel.bus_shared = true;               // If the bus is shared with the SD card, set to true (bus control with drawJpgFile etc.)

  // configure touch panel
  auto touchCfg = touch_instance.config();
  touchCfg.i2c_port = 1;
  touchCfg.i2c_addr = TOUCH_I2C_ADD;
  touchCfg.pin_sda = I2C_SDA;
  touchCfg.pin_scl = I2C_SCL;
  touchCfg.x_min = 0;
  touchCfg.x_max = SCREEN_HEIGHT;
  touchCfg.y_min = 0;
  touchCfg.y_max = SCREEN_WIDTH;
  touch_instance.config(touchCfg);
  _panel_instance.touch(&touch_instance);

  _panel_instance.config(panel);
  setPanel(&_panel_instance); // Sets the panel to use.
}

LGFX lcd;

/*** Display callback to flush the buffer to screen ***/
void Graphics::display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
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
void Graphics::touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
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

void Graphics::setup_lovyan_GFX()
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

void Graphics::setup_lvgl()
{
  String LVGL_Version = "Setting up LVGL, ";
  LVGL_Version += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Version);

  lv_init();

  // set up the display buffer
  lv_disp_draw_buf_init(&draw_buf, buf, buf2, bufferSize);

  // Set up LVGL screen draw handler
  static lv_disp_drv_t disp_drv;               /*Descriptor of a display driver*/
  lv_disp_drv_init(&disp_drv);                 /*Basic initialization*/
  disp_drv.flush_cb = Graphics::display_flush; /*Set your driver function*/
  disp_drv.draw_buf = &draw_buf;               /*Assign the buffer to the display*/
  disp_drv.hor_res = SCREEN_WIDTH;             /*Set the horizontal resolution of the display*/
  disp_drv.ver_res = SCREEN_HEIGHT;            /*Set the vertical resolution of the display*/
  lv_disp_drv_register(&disp_drv);             /*Finally register the driver*/

  // Set up LVGL screen touch handler
  static lv_indev_drv_t indev_drv;             /*Descriptor of a input device driver*/
  lv_indev_drv_init(&indev_drv);               /*Basic initialization*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;      /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = Graphics::touchpad_read; /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);           /*Finally register the driver*/

  Serial.println("Completed setting up LVGL");
}
