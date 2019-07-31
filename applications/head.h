#ifndef __HEAD_H
#define __HEAD_H	 

#include <rtthread.h>
#include <rtdevice.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "led.h"

#define myuart1_DEVICE_NAME "uart1"
#define myuart2_DEVICE_NAME "uart2"
#define myuart3_DEVICE_NAME "uart3"
#define myuart4_DEVICE_NAME "uart4"
#define myuart5_DEVICE_NAME "uart5"







typedef rt_int32_t s32;
typedef rt_int16_t s16;
typedef rt_int8_t  s8;

typedef rt_uint32_t  u32;
typedef rt_uint16_t u16;
typedef rt_uint8_t  u8;


#define	DEF_SAVE_DATA_LEN	1024
#define	DEF_SAVE_ADDR_CALI	1024
#define	DEF_SAVE_ADDR_CALI_BAK	2048
#define	DEF_SAVE_ADDR_CFG	0

#define DEF_TCD_DHW_VER     	((float)2.3)
#define DEF_TCD_DSW_MAIN_VER     	((u8)1)
#define DEF_TCD_DSW_SECOND_VER     	((u8)0)
#define DEF_TCD_DSW_EXTEN_VER     	((u8)2)
#define DEF_TCD_AHW_VER     	((float)1.1)

#define	BOARD_SLAVE_ADDR	0x01


/*Modbus Slave �궨��*/
#define S_COIL_START	0U
#define S_COIL_NCOILS						6U

#define S_REG0000_INPUT_START	0U
#define S_REG0000_INPUT_NREGS	128U
#define S_REG1000_INPUT_START	0x1000UL
#define S_REG1000_INPUT_NREGS	128U

#define S_REG0000_HOLDING_START         0U
#define S_REG0000_HOLDING_NREGS         256U
#define S_REG0100_HOLDING_START         0x100U
#define S_REG0100_HOLDING_NREGS         256U
#define S_REG0400_HOLDING_START         0x400U
#define S_REG0400_HOLDING_NREGS         256U
#define S_REG9000_HOLDING_START         0x9000UL
#define S_REG9000_HOLDING_NREGS         128U
#define S_REGE000_HOLDING_START         0xE000UL
#define S_REGE000_HOLDING_NREGS         128U
#define S_REG9000_INPUT_START         	0x9000UL
#define S_REG9000_INPUT_NREGS          	128U


typedef union
{
	float	fData;
	u16	wData[2];
}T_U16ToF32;

typedef	union	
{
	float	fData;
	u8	ucData[4];
}T_U8ToF32;

typedef	union	
{
	u32	dwData;
	u8	ucData[4];
}T_U8ToU32;

typedef	union	
{
	u32	ulData;
	u16	usData[2];
}T_U16ToU32;


typedef struct {
    u16	wSCoilStart;/*�����ȦModbus��ʼ��ַ*/
#if S_COIL_NCOILS%8
	u8	ucSCoilBuf[S_COIL_NCOILS/8+1];
#else
	u8	ucSCoilBuf[S_COIL_NCOILS/8];
#endif	
	u16   wSReg0000InStart;/*����Ĵ���Modbus��ʼ��ַ������ֻ����Ϣ��*/
	u16   awSReg0000InBuf[S_REG0000_INPUT_NREGS];

    u16   wSReg1000InStart;/*����Ĵ���Modbus��ʼ��ַ������ֻ����Ϣ��*/
	u16   awSReg1000InBuf[S_REG0000_INPUT_NREGS];
	
	u16   wSReg0100HoldStart;/*���ּĴ���Modbus��ʼ��ַ������ֻ����Ϣ��*/
	u16   awSReg0100HoldBuf[S_REG0000_HOLDING_NREGS];
	u16   wSReg0000HoldStart;/*���ּĴ���Modbus��ʼ��ַ������У׼�洢��*/
	u16   awSReg0000HoldBuf[S_REG0000_HOLDING_NREGS];
	u16   wSReg0400HoldStart;/*���ּĴ���Modbus��ʼ��ַ������У׼���ݴ洢��*/
	u16   awSReg0400HoldBuf[S_REG0400_HOLDING_NREGS];
	u16   wSReg9000HoldStart;/*���ּĴ���Modbus��ʼ��ַ������������*/
	u16   awSReg9000HoldBuf[S_REG9000_HOLDING_NREGS];

	u16   wSRegE000HoldStart;/*���ּĴ���Modbus��ʼ��ַ����������������*/
	u16   awSRegE000HoldBuf[S_REGE000_HOLDING_NREGS];
    
    u16   wSReg9000InStart;/*���ּĴ���Modbus��ʼ��ַ�����ڻ�����Ϣ��*/
	u16   awSReg9000InBuf[S_REG9000_INPUT_NREGS];	
}T_MODBUS_REGS;

typedef struct {
	T_U16ToF32               tP1Set; /*  ��1��Ũ��                  */
	T_U16ToF32               tP1Sample; /*  ��1��ת������              */
	T_U16ToF32               tP2Set;  /*  ��2��Ũ��                  */
	T_U16ToF32               tP2Sample;/*  ��2��ת������              */
	T_U16ToF32               tP3Set;   /*  ��3��Ũ��                  */
	T_U16ToF32               tP3Sample; /*  ��3��ת������              */
	T_U16ToF32               tP4Set;   /*  ��4��Ũ��                  */
	T_U16ToF32               tP4Sample;/*  ��4��ת������              */
    T_U16ToF32               tP5Set;   /*  ��5��Ũ��                  */
	T_U16ToF32               tP5Sample; /*  ��5��ת������              */
	T_U16ToF32               tP6Set;   /*  ��6��Ũ��                  */
	T_U16ToF32               tP6Sample;/*  ��6��ת������              */
	float               fK1;  /*  ����ֵ                      */
	float               fV1;  /*  ƫ��ֵ                      */
	float               fK2;  /*  ����ֵ                      */
	float               fV2;  /*  ƫ��ֵ                      */
	float               fK3;  /*  ����ֵ                      */
	float               fV3;  /*  ƫ��ֵ                      */
	float               fK4;  /*  ����ֵ                      */
	float               fV4;  /*  ƫ��ֵ                      */
    float               fK5;  /*  ����ֵ                      */
    float               fV5;  /*  ƫ��ֵ                      */
    //float               fK6;  /*  ����ֵ                      */
	//float               fV6;  /*  ƫ��ֵ                      */
	T_U16ToF32 	tOneSet;	
	T_U16ToF32 	tOneSample;

	float               fSetMax;/*  ����Ũ�ȵ�����              */
	float               fSetMin;/*  ����Ũ�ȵ�����              */
} T_GAS_CALI_PARA;

typedef struct {
	T_U16ToF32          tSample; /*  ����ֵ              */
	float               tTemper;/*  ͨ���¶�             */
	u8                  ucSample_Updated;/*  ����ֵ���±�־ */
    u8                  ucReadStep;
    u8                  ucWaitStepNum;
}T_GAS_MEASURE_PARA;

#define	GAS_TYPE_SO2    0U//SO2
#define	GAS_TYPE_CO     1U//CO
#define	GAS_TYPE_TD     2U//ʪ��        // 0   1    2   3      4      5       6  
#define	GAS_TYPE_P20    3U//�ܶ�        //SO2  CO  Td  P20  P(����)  P(�س�)  Pt100(��ʱ����)
#define	GAS_TYPE_P      4U//����ģ��ѹ��
#define	GAS_TYPE_PUMP   5U//�س�ѹ��
#define	TYPE_PT100      6U//PT100                                      
#define	TYPE_FLOW       7U//����
typedef	struct	
{
    T_GAS_CALI_PARA     tGasCaliParm[7];//����У׼���   0   1   2   3       4      5        6              7
                                                    //SO2  CO  Td  P20  P(����)  P(�س�)  Pt100(��ʱ����)  ����(��ʱ����)
    T_GAS_MEASURE_PARA  tGasMeasureParm[7];       /*ԭʼ���������*/
    T_U16ToF32          fTemperatureData;/*�����¶�*/

    /*ѹ��������� begin*/
    T_U16ToF32  tP_Val;/*ѹ������ֵ*/
    T_U16ToF32  tP20_Val;/*�ܶȲ���ֵ*/
    T_U16ToF32  tSensorP_Val;/*������ģ�鴦ѹ������ֵ*/
    T_U16ToF32  tPumpP_Val;/*�س�����ѹ������ֵ*/
    T_U16ToF32  tp_Pt100;//Pt100�¶�ֵ
    /*ѹ��������� end*/

    /*�ֽ���������*/
    T_U16ToF32  tSo2_CaliVal;//SO2У׼��ֵ
    T_U16ToF32  tCo_CaliVal;//COУ׼��ֵ
    u8          ucSo2_SetZeroFlag;//SO2������������
    u8          ucCo_SetZeroFlag;//CO������������

    /*���������ƿ���*/
    u16	        tFlow_SetVal;/*�趨����*/
    T_U16ToF32  tFlow_Val;/*��������*/
    T_U16ToF32  tFlow_AD;/*�����Ʒ�����������ֵ*/
	
	
    /*ʪ�Ȳ������*/
    //T_U16ToF32  TdData;//ʪ�ȱ�����������¶��ֵ  
    T_U16ToF32  TdDataCali;//����У׼���¶��ֵ 
    //T_U16ToF32  Td;	//���ػ�ѹ�µ�¶��  �Ա��������¶����й�ѹ������
    //T_U16ToF32  Td0;	//����µ�¶��
    //T_U16ToF32  Tdp;	//��ѹ�µ�td
    T_U16ToF32  TdPPM;	//TD PPM
    T_U16ToF32  TdPPMS;	//SF6ʪ�Ȼ��㵽20��
    T_U16ToF32  TdRH;	//���ʪ��
    //T_U16ToF32  Td_Temp;	//TD����ʱ �����¶�
    //int         Td_Press_Sample;	//ʪ�Ȳ�����ѹ������
    //float       Td_Press_Value;	//ʪ�Ȳ�����ѹ������ֵ
    /*ʪ�Ȳ������*/

    //u8	ucTCD_CaliData_Updated;	//���ݸ��±�־λ
    u8	ucO2_CaliData_Updated;	//���ݸ��±�־λ
    //u8	ucTd_Sample_Updated;	//���»�����¶���־λ
    //u8	ucTd143_Sample_Updated;	//���������¶��ֵ���±�־λ
    u16  ucTdReadCont;
    u8  uc_CaliData_Updated;//У׼���ݸ��±�־
    u8  uc_SysData_Upfated;//ϵͳ�����и���
    u8  uc_PumpTop_Flag;//������ת��λ��־
    u8  uc_PumpBottom_Flag;//���÷�ת��λ��־
    u8  uc_PumpPHigh_Flag;//����ѹ�����߱�־

	
}T_MEASURE_STR;//������Ϣ���ݼ���

typedef struct
{
    u16     ausID_STM32[4];
    u16     usSoftVer;

    float   fTCD_DHardVer;
    u32     ulTCD_DSoftVer;
    float   fTCD_AHardVer;
    u32     ulTCD_ASoftVer;

    float   fO2_DHardVer;
    u32     ulO2_DSoftVer;
    float   fO2_AHardVer;

    float   fTd_DHardVer;
    u32     ulTd_DSoftVer;		
}T_DEV_VERSION;//�汾��

/* ��ŷ��������*/
typedef struct
{
    u8  MAG_Cur_State;	//��ŷ���ǰ״̬
    u8  MAG_State_Updated;	//��ŷ����±�־
    u8  MAG_Times;	//��ŷ�������ʱʱ��
		
}DEV_MAGNETIC;


/* ���ÿ������*/
typedef struct
{
    u8	PUMP_Cur_State;	//����ǰ״̬
    u8	PUMP_State_Updated;	//���ø��±�־
    u8	PUMP_Times;	//���ö�����ʱʱ��
    u16 PUMP_Speed_Val;
		
}DEV_PUMP;


//�������ṹ��
//������Ϣ���ݼ���
typedef struct
    {
    u8  sysledstatus;
    u16 wTransmitterType;
    u8  ucSlaveAddr;
    u16 usSlaveCommBps;
    u8  ucErrFlag;
    u16 usCmdBackup;
    u16 usCmdRecovery;
    u8  ucGasType;
    u8  ucPrintFlag;
	u32 ucPrintCount;
	
    T_MEASURE_STR   tMeasureStr;/*�������ݼ���*/
    T_MODBUS_REGS   tModbusRegs;/*Modbus���ݼ���*/
    T_DEV_VERSION   tDevVer;/*�汾���ݼ���*/
    DEV_MAGNETIC    Mag[18];/*��ŷ�1��������*/
    DEV_PUMP        PUMP[2];//���ÿ��Ʋ�������
    u16             PumpStartV;//С����Ĭ��������ѹ

    //T_IAP_DEV       *ptIAP_Dev;	/*������������*/
}MAIN_STR;

extern MAIN_STR     g_tMainStr;



extern void PrintUserData(int open);

		 				    
#endif

