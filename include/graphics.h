#include <Arduino.h>
#include <LovyanGFX.hpp>

#define LCD_CS 37
#define LCD_BLK 45

#define TOUCH_I2C_ADD 0x38
#define I2C_SCL 39
#define I2C_SDA 38

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ILI9488 _panel_instance;
    lgfx::Bus_Parallel16 _bus_instance; // 8-bit parallel bus instance (ESP32 only)
    lgfx::Touch_FT5x06 touch_instance;

public:
    LGFX(void);
};
