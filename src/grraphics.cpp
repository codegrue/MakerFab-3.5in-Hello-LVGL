
#include "graphics.h"

LGFX::LGFX(void)
{
  // Configure bus control settings.
  auto bus = _bus_instance.config(); // Get a structure for bus settings.

  // 16 bit settings
  bus.port = 0;              // Select the I2S port to use (0 or 1) (use ESP32's I2S LCD mode)
  bus.freq_write = 20000000; // Transmit clock (up to 20MHz, rounded to the value obtained by dividing 80MHz by an integer)
  bus.pin_wr = 35;           // Pin number connecting WR
  bus.pin_rd = 48;           // Pin number connecting RD
  bus.pin_rs = 36;           // Pin number connecting RS(D/C)

  bus.pin_d0 = 47;
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

  // Set the display panel control.
  auto panel = _panel_instance.config(); // Gets the structure for display panel settings.

  panel.pin_cs = -1;   // CS to be pulled low
  panel.pin_rst = -1;  // RST is connected to the development board RST
  panel.pin_busy = -1; // Pin number to which BUSY is connected (-1 = disable)

  panel.memory_width = SCREEN_HEIGHT; // Maximum width supported by the driver IC
  panel.memory_height = SCREEN_WIDTH; // Maximum height supported by the driver IC
  panel.panel_width = SCREEN_HEIGHT;  // actual displayable width
  panel.panel_height = SCREEN_WIDTH;  // actual visible height
  panel.offset_x = 0;                 // Panel offset amount in X direction
  panel.offset_y = 0;                 // Panel offset amount in Y direction
  panel.offset_rotation = 0;          // Rotation direction value offset 0~7 (4~7 is upside down)
  panel.dummy_read_pixel = 8;         // Number of bits for dummy read before pixel readout
  panel.dummy_read_bits = 1;          // Number of bits for dummy read before non-pixel data read
  panel.readable = true;              // Set to true if data can be read
  panel.invert = false;               // Set to true if the light/darkness of the panel is reversed
  panel.rgb_order = false;            // Set to true if the panel's red and blue are swapped
  panel.dlen_16bit = true;            // Set to true for panels that send data length in 16-bit units
  panel.bus_shared = true;            // If the bus is shared with the SD card, set to true (bus control with drawJpgFile etc.)

  auto cfg = touch_instance.config();
  cfg.i2c_port = 1;
  cfg.i2c_addr = TOUCH_I2C_ADD;
  cfg.pin_sda = I2C_SDA;
  cfg.pin_scl = I2C_SCL;
  cfg.x_min = 0;
  cfg.x_max = SCREEN_HEIGHT;
  cfg.y_min = 0;
  cfg.y_max = SCREEN_WIDTH;
  touch_instance.config(cfg);
  _panel_instance.touch(&touch_instance);

  _panel_instance.config(panel);
  setPanel(&_panel_instance); // Sets the panel to use.
}