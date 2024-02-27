/**
 * @file app_led.h
 * @author h
 * @brief
 * @version 0.1
 * @date 2023-12-28
 *
 * @copyright Copyright (c) 2023
 *
 */
 
#ifndef __APP_LED_H__
#define __APP_LED_H__

#include "bsp_led.h"

typedef struct {
    uint16_t flash_light_time;
    uint16_t recorde_flash_light_time;
    uint16_t flash_dark_time;
    uint16_t recorde_flash_dark_time;
    uint16_t flash_times;
    uint16_t recorde_flash_times;
    uint16_t dark_time;
    uint16_t recorde_dark_time;
}led_param, *led_param_t;

typedef enum {
    LED_ALWAY_ON = 0,
    LED_ALWAY_OFF,
    LED_CYCLE_FLASH,
    LED_FLICKER,
}led_state;

typedef struct {
	int gd;
    led_state led_sta;
    led_param led_time;
}dev_led;

void app_led_init(void);
void set_led_always_on(uint32_t led_index);
void set_led_always_off(uint32_t led_index);
void set_led_cycle_flash(uint32_t led_index, uint32_t flash_light_time, uint32_t flash_dark_time);
void set_led_flicker(uint32_t led_index, uint32_t flash_times, uint32_t flash_light_time, uint32_t flash_dark_time, uint32_t dark_time);
void app_led_thread(void);

#endif /* __APP_LED_H__ */


