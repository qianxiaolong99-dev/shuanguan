/**************************************************************************
**************************************************************************/
#ifndef _DEFINE_GLOBAL_H_
#define _DEFINE_GLOBAL_H_
#pragma warning disable 752
 
/**************************************************************************
---------------------------------- 头文件 ---------------------------------
**************************************************************************/
#include <cms.h>
#include "Define_Type.h" 
#include "Setup_Global.h"
#include "Setup_IO.h" 

#include "__REL_IhDebug.h"
#include "__CMS_IhPlus.h" 	
#include "Communicate.h"
#include "HMI.h"
  
/**************************************************************************
--------------------------------- 函数声明 --------------------------------
**************************************************************************/ 
unsigned char Memory_Write(unsigned char addr,unsigned char value);
unsigned char Memory_Read(unsigned char addr);
 
void Delay_Xus(unsigned char x);		
void Delay_Xms(unsigned int x);		 
//-------------------------------------------------------------------------
void Buzz_Short_One();
void Buzz_Long_One();
//-------------------------------------------------------------------------
void Check_AD();			 
void Deal_Adc();
void Set_OutPut();	
void Set_PPGWork();  					 						 

/**************************************************************************
--------------------------------- 变量定义 --------------------------------
**************************************************************************/
volatile unsigned char Time_HMI;			//人机界面处理计时 
volatile unsigned char Time_BackPress;		//过压保护处理计时 
volatile unsigned char Status_PPGStop;		//PPG停止状态
/*************************************************************************/
volatile unsigned char AdcCount[3];			//ADC计数
volatile unsigned char AdcData[3];			//ADC数据
volatile unsigned char Delay_Adc;			//ADC延时计时
volatile unsigned char Delay_AdcNoOk;		//ADC未完成延时计时

volatile unsigned int  AD_VRef;				//内部基准电压AD值
volatile unsigned char AD_Vdd;				//VDD-AD值
volatile unsigned char AD_Gnd;	 			//GND-AD值

volatile unsigned char AD_Current_Fst;		//工作电流AD初值
volatile unsigned int  AD_Current;			//工作电流AD值

volatile unsigned int  AD_Volatage;			//市电电压AD值

volatile unsigned char AD_TmprIgbt;			//IGBT温度AD值
volatile unsigned char AD_TmprPan;			//锅底温度AD值
volatile unsigned char AD_TmprTop;			//顶部温度AD值

volatile unsigned char Count_IgbtDown;		//IGBT高温降功率次数
volatile VolElecType Vol;					//电压计算结果

volatile unsigned char Min_CheckError;		//故障检测计分
//-------------------------------------------------------------------------
volatile bit B_PanOver_Dis;					//锅底NTC超温判断禁止
volatile bit B_PanFail_Dis;					//锅底NTC失效判断禁止
volatile bit B_Dly_CheckErr;				//硬件故障检测延时
volatile bit B_IgbtDown;					//IGBT高温降功率 

volatile bits				AdcF1;
#define	Flag1_Adc			AdcF1.all		
#define B_Adc_On			AdcF1.one.b0	//ADC开启
#define B_Adc_Ok			AdcF1.one.b1	//ADC完成
#define B_Adc_Error			AdcF1.one.b2	//ADC错误
#define B_Adc_SetOk			AdcF1.one.b3	//ADC设置完成
 
volatile bits				AdcF2;
#define	Flag2_Adc			AdcF2.all
#define B_GetAD_MaxMin		AdcF2.one.b0	//求AD最大最小值
#define B_GetAD_AllSum		AdcF2.one.b1	//求AD总和值
#define B_CurrAdc_Fst		AdcF2.one.b2	//工作电流AD初值检测完成
#define B_CurrAdc_Once		AdcF2.one.b3	//工作电流ADC完成一次
#define B_VolAdc_Once		AdcF2.one.b4	//市电电压ADC完成一次

volatile bits				AdcCH;
#define	Channel_Adc			AdcCH.all		//AD检测通道
#define B_Channel_Vol		AdcCH.one.b0	//市电电压
#define B_Channel_Curr		AdcCH.one.b1	//工作电流
#define B_Channel_Igbt		AdcCH.one.b2	//IGBT温度
#define B_Channel_Pan		AdcCH.one.b3	//锅底温度
#define B_Channel_Top		AdcCH.one.b4	//顶部温度
#define B_Channel_VRef		AdcCH.one.b5	//内部基准电压	
#define B_Channel_Gnd		AdcCH.one.b6	//GND
#define B_Channel_Vdd		AdcCH.one.b7	//VDD
#define B_Channel_End		B_Channel_Vdd	//【最后一个ADC通道】	
 
volatile bits				ErrorF1;
#define	Flag1_Error			ErrorF1.all		//硬件故障标志1
#define B_EVolLow			ErrorF1.one.b0	//市电电压过低
#define B_EVolHigh			ErrorF1.one.b1	//市电电压过高
#define B_EIgbtOpen			ErrorF1.one.b2	//IGBT-NTC开路
#define B_EIgbtClose		ErrorF1.one.b3	//IGBT-NTC短路
#define B_EIgbtOver			ErrorF1.one.b4	//IGBT-NTC超温
#define B_EPanOpen			ErrorF1.one.b5	//锅底NTC开路
#define B_EPanClose			ErrorF1.one.b6	//锅底NTC短路
#define B_EPanOver			ErrorF1.one.b7	//锅底NTC超温

volatile bits				ErrorF2;
#define	Flag2_Error			ErrorF2.all		//硬件故障标志2
#define B_EPanFail			ErrorF2.one.b0	//锅底NTC失效
#define B_ETopOpen			ErrorF2.one.b1	//顶部NTC开路
#define B_ETopClose			ErrorF2.one.b2	//顶部NTC短路
#define B_ETopFail			ErrorF2.one.b3	//顶部NTC失效
#define B_Comm_Error		ErrorF2.one.b7	//通信故障
/*************************************************************************/
volatile unsigned int PwmPeriod_Fan;		//风扇PWM周期
volatile unsigned int PwmDuty_Fan;			//风扇PWM占空比	

volatile unsigned int PwmPeriod_Buzz;		//蜂鸣器PWM周期	
volatile unsigned int PwmDuty_Buzz;			//蜂鸣器PWM占空比

volatile unsigned char TimeBuzz;			//蜂鸣器鸣叫时间  
//-------------------------------------------------------------------------
volatile bits				OutPutF;
#define Flag_OutPut			OutPutF.all		//输出控制标志
#define B_Fan_En			OutPutF.one.b0	//风扇使能
#define B_Buzz_En			OutPutF.one.b1	//蜂鸣器使能
#define B_Buzz_Out			OutPutF.one.b2	//蜂鸣器输出
#define B_HeatStop_CheckPan	OutPutF.one.b4	//不加热但检锅				
#define B_HeatStop_NoCheck	OutPutF.one.b5	//不加热不检锅
/*************************************************************************/
volatile VolElecType Elec;					//电量计算结果

volatile unsigned int  Count_IgbtProtect;	//IGBT硬件保护计数

volatile unsigned int  SlopeCurr;			//电流斜率
volatile unsigned char SlopeCurr_Adjust;	//电流斜率调节值 

volatile unsigned int  CurrentReal;			//实际工作电流	
volatile unsigned int  PowerReal;			//实际输出功率 
volatile unsigned int  PowerSet;			//目标输出功率 
//-------------------------------------------------------------------------
volatile bit B_PPGTMR_Adjust;				//PPGTMR调整
volatile bit B_Adjust_SlopeCurr;			//电流斜率调节 
 
volatile bits				PPGF;
#define Flag_PPG			PPGF.all		//PPG控制标志
#define B_PPG_En			PPGF.one.b0		//PPG使能
#define B_PPGStop_CheckPan	PPGF.one.b1		//PPG暂停但检锅				
#define B_PPGStop_NoCheck	PPGF.one.b2		//PPG暂停不检锅

/**************************************************************************
--------------------------------- 配置缓存 --------------------------------
**************************************************************************/
volatile unsigned int  Buf_PPGMin;
volatile unsigned int  Buf_PPGMax; 
volatile unsigned int  Buf_PPGMax_Steel;
volatile unsigned int  Buf_PPGMax_Iron; 

volatile unsigned int  Buf_Power_CurrerConst;
volatile unsigned int  Buf_PowerLose_VHigh;
volatile unsigned int  Buf_PowerLose_VLow;

volatile unsigned char Buf_Vol_LowErr_In;
volatile unsigned char Buf_Vol_LowErr_Out;	
volatile unsigned char Buf_Vol_HighErr_In;
volatile unsigned char Buf_Vol_HighErr_Out;

//volatile unsigned char Buf_AD_Pan_FanOn; 
//volatile unsigned char Buf_AD_Pan_FanOff;
volatile unsigned char Buf_AD_Pan_OverIn;
volatile unsigned char Buf_AD_Pan_OverOut;

//volatile unsigned char Buf_AD_Igbt_FanOn;
//volatile unsigned char Buf_AD_Igbt_FanOff;
volatile unsigned char Buf_AD_Igbt_OverIn; 
volatile unsigned char Buf_AD_Igbt_OverOut;  
volatile unsigned char Buf_AD_Igbt_HighIn;
volatile unsigned char Buf_AD_Igbt_HighOut; 

/**************************************************************************
--------------------------- 特殊功能寄存器自定义 --------------------------
**************************************************************************/
volatile 	unsigned char 	INTCON2		@	0x08;

volatile 	unsigned char 	PIR1		@	0x0C;
volatile  	bit      		ADIF       	@ 	((unsigned)&PIR1*8)+6;
volatile 	bit 	 		RCIF	 	@ 	((unsigned)&PIR1*8)+5;
volatile 	bit 	 		TXIF	 	@ 	((unsigned)&PIR1*8)+4;
volatile  	bit      		TMR2IF     	@ 	((unsigned)&PIR1*8)+1;
volatile  	bit      		TMR1IF     	@ 	((unsigned)&PIR1*8)+0; 

volatile 	unsigned char 	PIR2		@	0x0D; 
volatile  	bit      		PPGWDTIF  	@ 	((unsigned)&PIR2*8)+1;
  
volatile 	unsigned char 	PIE1		@	0x13;
volatile  	bit      		TMR2IE     	@ 	((unsigned)&PIE1*8)+1;
 
volatile 	unsigned char 	ADCON0		@	0x9E;
volatile  	bit      		GODONE     	@ 	((unsigned)&ADCON0*8)+1;
volatile  	bit      		ADON       	@ 	((unsigned)&ADCON0*8)+0;

volatile 	unsigned char 	CM1CON1		@	0x117; 
volatile 	bit 	 		CM1COF	 	@ 	((unsigned)&CM1CON1*8)+3; 

volatile 	unsigned char 	PPGTMRL		@	0x190;
volatile 	unsigned char 	PPGTMRH		@	0x191; 

/**************************************************************************
**************************************************************************/
#endif

