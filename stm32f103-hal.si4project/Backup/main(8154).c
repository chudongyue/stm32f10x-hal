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

/*LED 任务*/
struct rt_thread rt_led_thread;
ALIGN(RT_ALIGN_SIZE)
//rt_uint8_t rt_led_thread_stack[512];
static rt_thread_t led_thread = RT_NULL;

/*SensorProcess*/
struct rt_thread rt_sensor_thread;
//rt_uint8_t rt_sensor_thread_stack[512];
static rt_thread_t sensor_thread = RT_NULL;
static struct rt_semaphore rx_sem_uart3;
rt_device_t myuart3;




static void led_thread_entry(void* parameter)
{
	//rt_uint32_t count = 0;
	while (1)
	{       
    	rt_pin_write(LED_PIN, PIN_LOW);  
    	rt_thread_delay(40);
		rt_pin_write(LED_PIN, PIN_HIGH);
    	rt_thread_delay(200); 
		rt_pin_write(LED_PIN, PIN_LOW); 
    	rt_thread_delay(60);
		rt_pin_write(LED_PIN, PIN_HIGH);
		rt_thread_delay(1000); 
	}
}


rt_err_t uart_input3(rt_device_t dev, rt_size_t size)
{
	/* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem_uart3);
	return RT_EOK;
}

void readuart3()
{
	char str[8] = " ";

	str[0] = 0x0f;
	str[1] = 0x00;
	str[6] = 0x55;
	str[7] = 0x99;
	rt_device_write(myuart3, 0, str, 8);
}
MSH_CMD_EXPORT(readuart3, read Decomsensor);


static void sensor_thread_entry(void* parameter)
{
	//char str[] = "this is my uart3 test \n";
	char str[128] = " ";
	char ch;
	rt_uint8_t i;

	myuart3 = rt_device_find(myuart3_DEVICE_NAME);
	if(!myuart3)
	{
		rt_kprintf("find %s failed!\n", myuart3_DEVICE_NAME);
	}
	rt_kprintf("find %s OK!\n", myuart3_DEVICE_NAME);

	/*串口设备打开方式：中断接收*/
	rt_device_open(myuart3, RT_DEVICE_FLAG_INT_RX);

	/*创建一个信号量*/
	rt_sem_init(&rx_sem_uart3, "rx_sem_uart3", 0, RT_IPC_FLAG_FIFO);					

	/* 接收回调函数 */
	rt_device_set_rx_indicate(myuart3, uart_input3);
#if 0
	//rt_pin_write(ChangeTO_SoCo_PIN, PIN_LOW);

	myuart3 = rt_device_find(myuart3_DEVICE_NAME);
	if(!myuart3)
	{
		rt_kprintf("find %s failed!\n", myuart3_DEVICE_NAME);
	}
	rt_kprintf("find %s OK!\n", myuart3_DEVICE_NAME);
	
	/*串口设备打开方式：中断接收*/
	rt_device_open(myuart3, RT_DEVICE_FLAG_INT_RX);
	
	/*创建一个信号量*/
	rt_sem_init(&rx_sem_uart3, "rx_sem_uart3", 0, RT_IPC_FLAG_FIFO);					

	/* 接收回调函数 */
	rt_device_set_rx_indicate(myuart3, uart_input3);
	str[0] = 0x0f;
	str[1] = 0x00;
	str[6] = 0x55;
	str[7] = 0x99;
	rt_device_write(myuart3, 0, str, 8);
	#endif

	while (1)
	{  

		/* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(myuart3, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx_sem_uart3, RT_WAITING_FOREVER);
        }
        /* 读取到的数据通过串口输出 */

		str[i++] = ch;
		if(ch == 0x0a)
		{
			//rt_pin_write(ChangeTO_SoCo_PIN, PIN_HIGH);
			//rt_device_write(myuart3, 0, str, i);
			rt_kprintf("The date is %s !\n", &str);
			i = 0;
			//rt_pin_write(ChangeTO_SoCo_PIN, PIN_LOW);
			
		}
		
		/*发送测试字符串*/
		//rt_device_write(myuart3, 0, str, 24);
		
		rt_thread_delay(10); 
	}
}



int main(void)
{
	led_init();
  /* user app entry */
	
    led_thread = rt_thread_create("led_thread", 
							led_thread_entry, RT_NULL, 
							256, 
							20, 
							20);

	if (led_thread != RT_NULL)
			rt_thread_startup(led_thread);
	else
			rt_kprintf("create led_thread failed!\r\n");

	sensor_thread = rt_thread_create("sensor_thread", 
							sensor_thread_entry, RT_NULL, 
							512, 
							2, 
							20);

	if (sensor_thread != RT_NULL)
			rt_thread_startup(sensor_thread);
	else
			rt_kprintf("create sensor_thread failed!\r\n");
	
    return 0;
}


