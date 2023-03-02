/*

This sample demonstrates how to run LVGL on the
Makerfabs-ESP32-S3-Parallel-TFT-with-Touch-3.5inch
development board. It includes a label, a counter, and
a button to increment the value.

Board:
https://www.makerfabs.com/esp32-s3-parallel-tft-with-touch-ili9488.html

Wiki:
https://wiki.makerfabs.com/ESP32_S3_Parallel_3.5_TFT_with_Touch.html

None of the included samples did a good job at packaging a runnable
demo, so I created this one in PlatformIO (VSCode) to provide
an out of the box runnable demo. Hope it is useful.

Notes:
- It uses LovyanGFX to buffer the drawing and dump to the screen
- The code for the FT6236 touch chip is in the "Libraries" folder
- lv_conf.h only has two changes: enabled LV_TICK_CUSTOM and activated LV_FONT_MONTSERRAT_24
- it uses a custom "esp32s3dev" in the "Boards" folder because this doesn't exist in the PlatformIO library
- graphics.h encapselates the boilerplate code for LovyanGFX
- user_interface.h encapselates the boilerplate code for LVGL
- main.cpp is clean with just the normal setup() and loop() functions, and a button handler
*/

#include <Arduino.h>
#include <LVGL.h>

#include "user_interface.h"



void setup(void)
{
  Serial.begin(9800);

  UserInterface::setup_lovyan_GFX();
  UserInterface::setup_lvgl();

  UserInterface::setup_ui();
}

void loop()
{
  lv_timer_handler();
  delay(5);
}
