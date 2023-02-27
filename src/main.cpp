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

int counter = 0;
lv_obj_t *counterLabel;

void increment_counter(lv_event_t *event)
{
  counter++;
  lv_label_set_text(counterLabel, String(counter).c_str());
}

void setup(void)
{
  Serial.begin(9800);

  UserInterface::setup_lovyan_GFX();
  UserInterface::setup_lvgl();

  auto screen = lv_scr_act();
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x008888), LV_PART_MAIN);
  lv_obj_set_style_pad_left(screen, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(screen, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(screen, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(screen, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_BETWEEN);

  lv_obj_t *titleLabel = lv_label_create(lv_scr_act());
  lv_label_set_text(titleLabel, "Hello MakerFab LVGL");
  lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

  counterLabel = lv_label_create(lv_scr_act());
  lv_label_set_text(counterLabel, String(counter).c_str());
  lv_obj_set_style_text_font(counterLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(counterLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

  lv_obj_t *button = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(button, increment_counter, LV_EVENT_CLICKED, NULL);
  lv_obj_t *buttonLabel = lv_label_create(button);
  lv_label_set_text(buttonLabel, "Increment");
  lv_obj_set_style_text_font(buttonLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void loop()
{
  lv_timer_handler();
  delay(5);
}
