/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-07-29     Arda.Fu      first implementation
 */
#include <rtthread.h>
#include <head.h>


struct rt_thread rt_led_thread;

ALIGN(RT_ALIGN_SIZE)
rt_uint8_t rt_led_thread_stack[512];
static rt_thread_t led_thread = RT_NULL;




static void led_thread_entry(void* parameter)
{
	rt_uint32_t count = 0;
	while (1)
	{
		rt_pin_write(LED_PIN, PIN_HIGH);
    rt_kprintf("led on, count : %d\r\n", count);
    rt_thread_delay(1000);
        
    rt_pin_write(LED_PIN, PIN_LOW);
    rt_kprintf("led off\r\n");
    rt_thread_mdelay(100);
	}
}


int main(void)
{
	led_init();
  /* user app entry */
	
    led_thread = rt_thread_create("led_thread", 
							led_thread_entry, RT_NULL, 
							512, 
							2, 
							20);

	if (led_thread != RT_NULL)
			rt_thread_startup(led_thread);
	else
			rt_kprintf("create led_thread failed!\r\n");
	
    return 0;
}
