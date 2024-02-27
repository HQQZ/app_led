/**
 * @file app_led.c
 * @author h
 * @brief
 * @version 0.1
 * @date 2023-12-28
 *
 * @copyright Copyright (c) 2023
 *
 */
 
#include "app_led.h"
#include <string.h>


#define APP_LED_CYCLE   5 /* cycle ms */
#define MAX_LED_SUM     12


static dev_led led_buff[MAX_LED_SUM];


/**
 * @brief init app led
 *
 */
void app_led_init(void)
{
	set_all_led_off();
    memset(led_buff, 0, sizeof(led_buff));
}

void set_led_always_on(uint32_t led_index)
{
	led_buff[led_index].gd = 0;
	led_buff[led_index].led_sta = LED_ALWAY_ON;
	led_buff[led_index].gd = -1;
}


void set_led_always_off(uint32_t led_index)
{
	led_buff[led_index].gd = 0;
	led_buff[led_index].led_sta = LED_ALWAY_OFF;
	led_buff[led_index].gd = -1;
}

void set_led_cycle_flash(uint32_t led_index, uint32_t flash_light_time, uint32_t flash_dark_time)
{
	led_buff[led_index].gd = 0;
	memset((char *)&led_buff[led_index], 0, sizeof(dev_led));
	led_buff[led_index].led_sta = LED_CYCLE_FLASH;
	led_buff[led_index].led_time.flash_light_time = flash_light_time / APP_LED_CYCLE;
	led_buff[led_index].led_time.flash_dark_time = flash_dark_time / APP_LED_CYCLE;
	led_buff[led_index].gd = -1;
}

void set_led_flicker(uint32_t led_index, uint32_t flash_times, uint32_t flash_light_time, uint32_t flash_dark_time, uint32_t dark_time)
{
	led_buff[led_index].gd = 0;
	memset((char *)&led_buff[led_index], 0, sizeof(dev_led));
	led_buff[led_index].led_sta = LED_FLICKER;
	led_buff[led_index].led_time.dark_time = dark_time / APP_LED_CYCLE;
	led_buff[led_index].led_time.flash_times = flash_times;
	led_buff[led_index].led_time.flash_light_time = flash_light_time / APP_LED_CYCLE;
	led_buff[led_index].led_time.flash_dark_time = flash_dark_time / APP_LED_CYCLE;
	led_buff[led_index].gd = -1;
}


/**
 * @brief   app led thred(周期性调用)
 *
 */
void app_led_thread(void)
{
	for(int i = 0; i < MAX_LED_SUM; i++) {
		if(led_buff[i].gd == -1) {
			switch(led_buff[i].led_sta) {
			case LED_ALWAY_ON:
				set_led_sta(i, 1);
				break;

			case LED_ALWAY_OFF:
				set_led_sta(i, 0);
				break;

			case LED_CYCLE_FLASH:
				if(led_buff[i].led_time.recorde_flash_light_time++ < led_buff[i].led_time.flash_light_time)
				{
					set_led_sta(i, 1);
					led_buff[i].led_time.recorde_flash_dark_time = 0;
				}
				else
				{
					if(led_buff[i].led_time.recorde_flash_dark_time++ < led_buff[i].led_time.flash_dark_time)
					{
						set_led_sta(i, 0);
					}
					else
					{
						led_buff[i].led_time.recorde_flash_light_time = 0;
					}
				}
				break;

			case LED_FLICKER:
				if(led_buff[i].led_time.recorde_flash_times <= led_buff[i].led_time.flash_times)
				{
					if(led_buff[i].led_time.recorde_flash_light_time++ < led_buff[i].led_time.flash_light_time)
					{
						set_led_sta(i, 1);
						led_buff[i].led_time.recorde_flash_dark_time = 0;
					}
					else
					{
						if(led_buff[i].led_time.recorde_flash_dark_time++ < led_buff[i].led_time.flash_dark_time - 1)
						{
							set_led_sta(i, 0);
						}
						else
						{
							led_buff[i].led_time.recorde_flash_light_time = 0;
							led_buff[i].led_time.recorde_flash_times++;
						}
					}
				}
				else
				{
					if(led_buff[i].led_time.recorde_dark_time++ < led_buff[i].led_time.dark_time - 1)
					{
						set_led_sta(i, 0);
					}
					else
					{
						led_buff[i].led_time.recorde_dark_time = 0;
						led_buff[i].led_time.recorde_flash_times = 0;
					}
				}
				break;
			}
		}
	}
	led_output();
}

