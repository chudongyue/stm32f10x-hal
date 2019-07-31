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


/*Modbus Slave 宏定义*/
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
    u16	wSCoilStart;/*输出线圈Modbus起始地址*/
#if S_COIL_NCOILS%8
	u8	ucSCoilBuf[S_COIL_NCOILS/8+1];
#else
	u8	ucSCoilBuf[S_COIL_NCOILS/8];
#endif	
	u16   wSReg0000InStart;/*输入寄存器Modbus起始地址，用于只读信息区*/
	u16   awSReg0000InBuf[S_REG0000_INPUT_NREGS];

    u16   wSReg1000InStart;/*输入寄存器Modbus起始地址，用于只读信息区*/
	u16   awSReg1000InBuf[S_REG0000_INPUT_NREGS];
	
	u16   wSReg0100HoldStart;/*保持寄存器Modbus起始地址，用于只读信息区*/
	u16   awSReg0100HoldBuf[S_REG0000_HOLDING_NREGS];
	u16   wSReg0000HoldStart;/*保持寄存器Modbus起始地址，用于校准存储区*/
	u16   awSReg0000HoldBuf[S_REG0000_HOLDING_NREGS];
	u16   wSReg0400HoldStart;/*保持寄存器Modbus起始地址，用于校准备份存储区*/
	u16   awSReg0400HoldBuf[S_REG0400_HOLDING_NREGS];
	u16   wSReg9000HoldStart;/*保持寄存器Modbus起始地址，用于配置区*/
	u16   awSReg9000HoldBuf[S_REG9000_HOLDING_NREGS];

	u16   wSRegE000HoldStart;/*保持寄存器Modbus起始地址，用于离线升级区*/
	u16   awSRegE000HoldBuf[S_REGE000_HOLDING_NREGS];
    
    u16   wSReg9000InStart;/*保持寄存器Modbus起始地址，用于基础信息区*/
	u16   awSReg9000InBuf[S_REG9000_INPUT_NREGS];	
}T_MODBUS_REGS;

typedef struct {
	T_U16ToF32               tP1Set; /*  第1点浓度                  */
	T_U16ToF32               tP1Sample; /*  第1点转换数据              */
	T_U16ToF32               tP2Set;  /*  第2点浓度                  */
	T_U16ToF32               tP2Sample;/*  第2点转换数据              */
	T_U16ToF32               tP3Set;   /*  第3点浓度                  */
	T_U16ToF32               tP3Sample; /*  第3点转换数据              */
	T_U16ToF32               tP4Set;   /*  第4点浓度                  */
	T_U16ToF32               tP4Sample;/*  第4点转换数据              */
    T_U16ToF32               tP5Set;   /*  第5点浓度                  */
	T_U16ToF32               tP5Sample; /*  第5点转换数据              */
	T_U16ToF32               tP6Set;   /*  第6点浓度                  */
	T_U16ToF32               tP6Sample;/*  第6点转换数据              */
	float               fK1;  /*  比例值                      */
	float               fV1;  /*  偏移值                      */
	float               fK2;  /*  比例值                      */
	float               fV2;  /*  偏移值                      */
	float               fK3;  /*  比例值                      */
	float               fV3;  /*  偏移值                      */
	float               fK4;  /*  比例值                      */
	float               fV4;  /*  偏移值                      */
    float               fK5;  /*  比例值                      */
    float               fV5;  /*  偏移值                      */
    //float               fK6;  /*  比例值                      */
	//float               fV6;  /*  偏移值                      */
	T_U16ToF32 	tOneSet;	
	T_U16ToF32 	tOneSample;

	float               fSetMax;/*  气体浓度的上限              */
	float               fSetMin;/*  气体浓度的上限              */
} T_GAS_CALI_PARA;

typedef struct {
	T_U16ToF32          tSample; /*  采样值              */
	float               tTemper;/*  通道温度             */
	u8                  ucSample_Updated;/*  采样值更新标志 */
    u8                  ucReadStep;
    u8                  ucWaitStepNum;
}T_GAS_MEASURE_PARA;

#define	GAS_TYPE_SO2    0U//SO2
#define	GAS_TYPE_CO     1U//CO
#define	GAS_TYPE_TD     2U//湿度        // 0   1    2   3      4      5       6  
#define	GAS_TYPE_P20    3U//密度        //SO2  CO  Td  P20  P(测量)  P(回充)  Pt100(暂时不用)
#define	GAS_TYPE_P      4U//测量模组压力
#define	GAS_TYPE_PUMP   5U//回充压力
#define	TYPE_PT100      6U//PT100                                      
#define	TYPE_FLOW       7U//流量
typedef	struct	
{
    T_GAS_CALI_PARA     tGasCaliParm[7];//二次校准相关   0   1   2   3       4      5        6              7
                                                    //SO2  CO  Td  P20  P(测量)  P(回充)  Pt100(暂时不用)  流量(暂时不用)
    T_GAS_MEASURE_PARA  tGasMeasureParm[7];       /*原始测量量相关*/
    T_U16ToF32          fTemperatureData;/*环境温度*/

    /*压力测量相关 begin*/
    T_U16ToF32  tP_Val;/*压力测量值*/
    T_U16ToF32  tP20_Val;/*密度测量值*/
    T_U16ToF32  tSensorP_Val;/*传感器模组处压力测量值*/
    T_U16ToF32  tPumpP_Val;/*回充气泵压力测量值*/
    T_U16ToF32  tp_Pt100;//Pt100温度值
    /*压力测量相关 end*/

    /*分解物测量相关*/
    T_U16ToF32  tSo2_CaliVal;//SO2校准后值
    T_U16ToF32  tCo_CaliVal;//CO校准后值
    u8          ucSo2_SetZeroFlag;//SO2发送清零命令
    u8          ucCo_SetZeroFlag;//CO发送清零命令

    /*质量流量计控制*/
    u16	        tFlow_SetVal;/*设定流量*/
    T_U16ToF32  tFlow_Val;/*反馈流量*/
    T_U16ToF32  tFlow_AD;/*流量计反馈流量采样值*/
	
	
    /*湿度测量相关*/
    //T_U16ToF32  TdData;//湿度变送器出来的露点值  
    T_U16ToF32  TdDataCali;//二次校准后的露点值 
    //T_U16ToF32  Td;	//本地环压下的露点  对变送器输出露点进行过压力补偿
    //T_U16ToF32  Td0;	//标况下的露点
    //T_U16ToF32  Tdp;	//在压下的td
    T_U16ToF32  TdPPM;	//TD PPM
    T_U16ToF32  TdPPMS;	//SF6湿度换算到20度
    T_U16ToF32  TdRH;	//相对湿度
    //T_U16ToF32  Td_Temp;	//TD测量时 环境温度
    //int         Td_Press_Sample;	//湿度补偿用压力采样
    //float       Td_Press_Value;	//湿度补偿用压力计算值
    /*湿度测量相关*/

    //u8	ucTCD_CaliData_Updated;	//数据更新标志位
    u8	ucO2_CaliData_Updated;	//数据更新标志位
    //u8	ucTd_Sample_Updated;	//更新换算后的露点标志位
    //u8	ucTd143_Sample_Updated;	//变送器输出露点值更新标志位
    u16  ucTdReadCont;
    u8  uc_CaliData_Updated;//校准数据更新标志
    u8  uc_SysData_Upfated;//系统数据有更新
    u8  uc_PumpTop_Flag;//气泵正转限位标志
    u8  uc_PumpBottom_Flag;//气泵反转限位标志
    u8  uc_PumpPHigh_Flag;//气泵压力过高标志

	
}T_MEASURE_STR;//测量信息数据集合

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
}T_DEV_VERSION;//版本号

/* 电磁阀控制相关*/
typedef struct
{
    u8  MAG_Cur_State;	//电磁阀当前状态
    u8  MAG_State_Updated;	//电磁阀更新标志
    u8  MAG_Times;	//电磁阀动作延时时间
		
}DEV_MAGNETIC;


/* 气泵控制相关*/
typedef struct
{
    u8	PUMP_Cur_State;	//气泵前状态
    u8	PUMP_State_Updated;	//气泵更新标志
    u8	PUMP_Times;	//气泵动作延时时间
    u16 PUMP_Speed_Val;
		
}DEV_PUMP;


//数据主结构体
//测量信息数据集合
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
	
    T_MEASURE_STR   tMeasureStr;/*测量数据集合*/
    T_MODBUS_REGS   tModbusRegs;/*Modbus数据集合*/
    T_DEV_VERSION   tDevVer;/*版本数据集合*/
    DEV_MAGNETIC    Mag[18];/*电磁阀1参数集合*/
    DEV_PUMP        PUMP[2];//气泵控制参数集合
    u16             PumpStartV;//小气泵默认启动电压

    //T_IAP_DEV       *ptIAP_Dev;	/*升级参数集合*/
}MAIN_STR;

extern MAIN_STR     g_tMainStr;



extern void PrintUserData(int open);

		 				    
#endif

