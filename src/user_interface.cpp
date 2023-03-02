#include "user_interface.h"

static int counter = 0;
lv_obj_t *counterLabel;
lv_obj_t *led1;

void Screen_Clicked(lv_event_t *e)
{
    lv_point_t p;

    lv_indev_t *indev = lv_indev_get_act();
    lv_indev_type_t indev_type = lv_indev_get_type(indev);
    lv_indev_get_point(indev, &p);

    lv_obj_align(led1, LV_ALIGN_TOP_LEFT, p.x, p.y);
    lv_led_on(led1);
}

void UserInterface::setup_ui()
{
    auto screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x006666), LV_PART_MAIN);
    lv_obj_add_event_cb(screen, Screen_Clicked, LV_EVENT_CLICKED, NULL);

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
    lv_obj_add_flag(widgetPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t *titleLabel = lv_label_create(widgetPanel);
    lv_label_set_text(titleLabel, "Hello MakerFab LVGL");
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_flag(titleLabel, LV_OBJ_FLAG_EVENT_BUBBLE);

    counterLabel = lv_label_create(widgetPanel);
    lv_label_set_text(counterLabel, String(counter).c_str());
    lv_obj_set_style_text_font(counterLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(counterLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_flag(counterLabel, LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t *button = lv_btn_create(widgetPanel);
    lv_obj_add_event_cb(button, increment_counter, LV_EVENT_CLICKED, NULL);
    lv_obj_t *buttonLabel = lv_label_create(button);
    lv_label_set_text(buttonLabel, "Increment");
    lv_obj_set_style_text_font(buttonLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(button, LV_OBJ_FLAG_EVENT_BUBBLE);

    led1 = lv_led_create(screen);
    lv_obj_align(led1, LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_clear_flag(led1, LV_OBJ_FLAG_CLICKABLE);
    lv_led_set_color(led1, lv_palette_main(LV_PALETTE_RED));
    lv_led_off(led1);
}

void UserInterface::increment_counter(lv_event_t *event)
{
    counter++;
    lv_label_set_text(counterLabel, String(counter).c_str());
}