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

MAIN_STR	g_tMainStr;


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
rt_device_t myuart3;
rt_uint8_t uart3_rangeFlag;//0-SO2Ũ��;1-SO2�¶�;2-COŨ��;3-CO�¶�


struct rt_thread rt_readuart3_thread;
static rt_thread_t readuart3_thread = RT_NULL;



static void led_thread_entry(void* parameter)
{
	rt_uint32_t count = 0;

	//rt_kprintf("\n");
	rt_kprintf("create led_thread OK!\n");

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
		
		count++;
		if(count > 3)
		{
			count = 0;
			PrintUserData(g_tMainStr.ucPrintFlag);
		}
	}
}


rt_err_t uart_input3(rt_device_t dev, rt_size_t size)
{
	/* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&rx_sem_uart3);
	return RT_EOK;
}

void readCouart3(rt_uint8_t range, rt_uint8_t val_temp)
{
	rt_uint8_t str[10];

	if(range)
	{
		rt_pin_write(ChangeTO_SoCo_PIN, PIN_LOW);//SO
	}
	else
	{
		rt_pin_write(ChangeTO_SoCo_PIN, PIN_HIGH);//CO
	}
	
	if(val_temp == 1)//���¶�
	{
		if(range)
		{
			str[0] = 0x1a;
			uart3_rangeFlag = 1;//SO2 �¶�
		}
		else
		{
			str[0] = 0x18;
			uart3_rangeFlag = 3;//CO �¶�
		}
	}
	else if(val_temp == 0)
	{
		str[0] = 0x0f;
		if(range)
		{
			uart3_rangeFlag = 0;//SO2 Ũ��
		}
		else
		{
			uart3_rangeFlag = 2;//CO Ũ��
		}
	}
	str[1] = 0x00;
	str[2] = 0x00;
	str[3] = 0x00;
	str[4] = 0x00;
	str[5] = 0x00;
	str[6] = 0x55;
	str[7] = 0x99;
	str[8] = 0x0d;
	str[9] = 0x0a;
	
	if(val_temp == 2)//�������
	{
		str[1] = 0x08;
		str[2] = 0x00;
		str[3] = 0x01;
		str[4] = 0x00;
		str[5] = 0x00;
		str[6] = 0x7a;
		str[7] = 0x99;
		str[8] = 0x0d;
		str[9] = 0x0a;
	}
		
	rt_device_write(myuart3, 0, &str, 10);
}


static void readuart3_thread_entry(void* parameter)
{
	//rt_kprintf("\n");
	rt_kprintf("create readuart3_thread OK!\n");

	while (1)
	{ 
		readCouart3(0,0);//COŨ��
		rt_thread_delay(500);		
		readCouart3(1,0);//SOŨ��
		rt_thread_delay(500);
		readCouart3(0,1);//CO�¶�
		rt_thread_delay(500);
		readCouart3(1,1);//SO�¶�
		rt_thread_delay(500);
	}
}


static void sensor_thread_entry(void* parameter)
{
	rt_uint8_t str[64] = {0};
	char ch;
	rt_uint8_t i;
	float val;
	rt_err_t openflag;


	rt_kprintf("\n");
	rt_kprintf("create sensor_thread OK!\n");
	myuart3 = rt_device_find(myuart3_DEVICE_NAME);
	if(!myuart3)
	{
		rt_kprintf("find %s failed!\n", myuart3_DEVICE_NAME);
	}
	rt_kprintf("find %s OK!\n", myuart3_DEVICE_NAME);

	/*�����豸�򿪷�ʽ���жϽ���*/
	openflag = rt_device_open(myuart3, RT_DEVICE_FLAG_INT_RX);
	if(RT_EOK != openflag)
	{
		rt_kprintf("Open %s failed!\n", myuart3_DEVICE_NAME);
	}
	//rt_kprintf("\n");
	rt_kprintf("%s is opened!\n", myuart3_DEVICE_NAME);


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
		if(ch != 0x0a)
		{
			str[i++] = ch;
			if(i >= 32)i = 0;
		}
		else
		{
			str[i++] = ch;
			if(i >= 20)i = 2;
			if(i < 2)i = 2;
			if((str[i-2] == 0x0d)&&(str[i-1] == 0x0a))
			{
				str[i++] = '\0';
				val = atof((const char*)str);
				//val *= 1000;						//Ŀǰrt��֧�ָ�����
				if(0 == uart3_rangeFlag)
				{
					g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_SO2].tSample.fData = val;
					//rt_kprintf("The SO2 val*1000 is %d \n", (rt_int16_t)val);	
				}
				else if(1 == uart3_rangeFlag)
				{
					g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_SO2].tTemper = val;
					//rt_kprintf("The SO2 temp*1000 is %d \n", (rt_int16_t)val);
				}
				else if(2 == uart3_rangeFlag)
				{
					g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_CO].tSample.fData = val;
					//rt_kprintf("The CO val*1000 is %d \n", (rt_int16_t)val);
				}
				else if(3 == uart3_rangeFlag)
				{
					g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_CO].tTemper = val;
					//rt_kprintf("The CO temp*1000 is %d \n", (rt_int16_t)val);
				}
			}
			i = 0;
		}
		
		/*���Ͳ����ַ���*/
		//rt_device_write(myuart3, 0, str, 24);
		
		//rt_thread_delay(10); 
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
			rt_kprintf("create led_thread failed!\n");

	sensor_thread = rt_thread_create("sensor_thread", 
							sensor_thread_entry, RT_NULL, 
							768, 
							4, 
							20);

	if (sensor_thread != RT_NULL)
			rt_thread_startup(sensor_thread);
	else
			rt_kprintf("create sensor_thread failed!\n");

    readuart3_thread = rt_thread_create("readuart3_thread", 
							readuart3_thread_entry, RT_NULL, 
							256, 
							6, 
							20);

	if (readuart3_thread != RT_NULL)
			rt_thread_startup(readuart3_thread);
	else
			rt_kprintf("create readuart3_thread failed!\n");

	
    return 0;
}



void PrintUserData(int open)
{
    s16 temps;
	u16 tempu;
	float tempf;

    if (open)
    {
    	g_tMainStr.ucPrintCount ++;
        rt_kprintf("********************************************************************************\n");
		rt_kprintf("��ӡ(���5S): %d S\n", g_tMainStr.ucPrintCount*5);
		rt_kprintf("********************************************************************************\n");
		temps = (s16)g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_SO2].tSample.fData;
		tempf = g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_SO2].tSample.fData - temps;
		tempu = (u16)(tempf * 1000);
		rt_kprintf("--SO2Ũ��: %d.%d ppm\n", temps,tempu);

		temps = (s16)g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_SO2].tTemper;
		tempf = g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_SO2].tTemper - temps;
		tempu = (u16)(tempf * 1000);
		rt_kprintf("--SO2�¶�: %d.%d ��\n", temps,tempu);

		temps = (s16)g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_CO].tSample.fData;
		tempf = g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_CO].tSample.fData - temps;
		tempu = (u16)(tempf * 1000);
		rt_kprintf("---COŨ��: %d.%d ppm\n", temps,tempu);

		temps = (s16)g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_CO].tTemper;
		tempf = g_tMainStr.tMeasureStr.tGasMeasureParm[GAS_TYPE_CO].tTemper - temps;
		tempu = (u16)(tempf * 1000);
		rt_kprintf("---CO�¶�: %d.%d ��\n", temps,tempu);
		rt_kprintf("********************************************************************************\n");
    }
}



void print_rt_data(int argc, char ** argv)//����ʵʱ�����������
{
    if (argc < 2)
    {
        rt_kprintf("Please input'cmd <o:open|c:close>'\n");
        return;
    }

    if (!rt_strcmp(argv[1], "o")||!rt_strcmp(argv[1], "O"))//����o�������������
    {
        g_tMainStr.ucPrintFlag = 1;
    }
    else if (!rt_strcmp(argv[1], "c")||!rt_strcmp(argv[1], "C"))//����c�ر������������
    {
        g_tMainStr.ucPrintFlag = 0;
    }
    else
    {
        rt_kprintf("Please input'cmd <o:open|c:close>'\n");
    }	
}
MSH_CMD_EXPORT(print_rt_data, Printout Real Time Data );


void reboot(void)//ϵͳ��λ
{
   rt_kprintf("Sys will be reboot after 3S  !\n"); 
   rt_thread_delay(1000);
   rt_kprintf("Sys will be reboot after 2S  !\n"); 
   rt_thread_delay(1000);
   rt_kprintf("Sys will be reboot after 1S  !\n"); 
   rt_thread_delay(1000);
   rt_kprintf(" Sys  will  be  reboot   now !\n"); 
   rt_thread_delay(1000);

   rt_hw_cpu_reset();
}
MSH_CMD_EXPORT(reboot, Reboot sys !); 






