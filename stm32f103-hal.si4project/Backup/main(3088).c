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

/*LED ����*/
struct rt_thread rt_led_thread;
ALIGN(RT_ALIGN_SIZE)
//rt_uint8_t rt_led_thread_stack[512];
static rt_thread_t led_thread = RT_NULL;

/*SensorProcess*/
struct rt_thread rt_sensor_thread;
//rt_uint8_t rt_sensor_thread_stack[512];
static rt_thread_t sensor_thread = RT_NULL;
static struct rt_semaphore rx_sem_uart3;




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
	rt_kprintf("rx date ok!\n");
	/* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&rx_sem_uart3);
	return RT_EOK;
}


static void sensor_thread_entry(void* parameter)
{
	rt_device_t myuart3;
	char str[] = "this is my uart3 test \n";
	char ch;

	myuart3 = rt_device_find(myuart3_DEVICE_NAME);
	if(!myuart3)
	{
		rt_kprintf("find %s failed!\n", myuart3_DEVICE_NAME);
	}
	rt_kprintf("find %s OK!\n", myuart3_DEVICE_NAME);
	
	/*�����豸�򿪷�ʽ���жϽ���*/
	rt_device_open(myuart3, RT_DEVICE_FLAG_INT_RX);
	
	/*����һ���ź���*/
	rt_sem_init(&rx_sem_uart3, "rx_sem_uart3", 0, RT_IPC_FLAG_FIFO);					

	/* ���ջص����� */
	rt_device_set_rx_indicate(myuart3, uart_input3);

	while (1)
	{  

		/* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(myuart3, -1, &ch, 1) != 1)
        {
            /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
            rt_sem_take(&rx_sem_uart3, RT_WAITING_FOREVER);
        }
        /* ��ȡ��������ͨ��������� */

		str++ = ch;
		if(ch == '\r')
		{
				/*���Ͳ����ַ���*/
			rt_device_write(myuart3, 0, str, 24);
		}
		
		/*���Ͳ����ַ���*/
		//rt_device_write(myuart3, 0, str, 24);
		
		rt_thread_delay(1000); 
	}
}



int main(void)
{
	led_init();
  /* user app entry */
	
    led_thread = rt_thread_create("led_thread", 
							led_thread_entry, RT_NULL, 
							512, 
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

