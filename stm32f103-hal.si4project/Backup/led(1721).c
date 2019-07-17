#include <rtthread.h>
#include <rtdevice.h>
#include "led.h"

int led_init(void)
{
    //rt_uint8_t count;

    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);  
    
    /*for(count = 0 ; count < 10 ;count++)
    {       
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_kprintf("led on, count : %d\r\n", count);
        rt_thread_mdelay(500);
        
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_kprintf("led off\r\n");
        rt_thread_mdelay(500);
    }*/
    return 0;
}
//MSH_CMD_EXPORT(led, Heart led sample);






