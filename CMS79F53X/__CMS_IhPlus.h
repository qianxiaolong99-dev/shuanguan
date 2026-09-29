/**************************************************************************
**************************************************************************/
#ifndef	_CMS_IHPLUS_H_
#define	_CMS_IHPLUS_H_
#pragma warning disable 752,759

/**************************************************************************
---------------------------------- 头文件 ---------------------------------
**************************************************************************/
#include "Define_Global.h"
 
/**************************************************************************
--------------------------------- 函数声明 --------------------------------
**************************************************************************/ 
extern void __CMS_Plus_Interrupt(); 
extern void __CMS_Plus_PPG();
 
/**************************************************************************
------------------------------- 连续低功率 --------------------------------
**************************************************************************/
//参数名称：PPG导通延时值（连续低功率）
//取值范围：0~15		
//对应关系：0=不延时，1=4~5*125ns，2=8~9*125ns，……15=64~65*125ns 	
#define 	C_PPGDly_LxLow			0		 
//-------------------------------------------------------------------------
//参数名称：连续低功率开启功率下限阀值（实际：800W）
//参数说明：>=此阀值，才开启连续低功率功能
#define  	C_MinPower_LxLow 		800/C_Mult_Power	
/*************************************************************************/
const unsigned char	ValueTime_PPGDly_LxLow = C_PPGDly_LxLow;
 
const unsigned char ValueMin_Power_LxLow = C_MinPower_LxLow;
/*************************************************************************/
extern volatile unsigned char 	Status_LxLow;	//连续低功率状态
//-------------------------------------------------------------------------	
extern volatile unsigned char 	Num_LxLowOn;	//连续低功率开个数
extern volatile unsigned char	Num_LxLowOff;	//连续低功率关个数
 
/**************************************************************************
--------------------------------- PPG抖频 ---------------------------------
**************************************************************************/
//计算公式：电流斜率 = 256 * 256 * 实际电流 / 检测的电流AD
//参数说明：电流斜率越大，功率越小

//参数名称：铁锅电流斜率（PPG抖频） 
#define  	C_SlopeIron_Curr_PPGDP	12000//3960
//参数名称：钢锅电流斜率（PPG抖频）
#define  	C_SlopeSteel_Curr_PPGDP	12000//3880	
//-------------------------------------------------------------------------
//参数名称：PPG抖频开启PPGTMR下限阀值（实际：8us） 
//参数说明：>=此阀值，才开启PPG抖频功能
#define  	C_MinPPGTMR_PPGDP 		32*8	

//参数名称：PPG抖频开启功率下限阀值（实际：1690W）
//参数说明：>=此阀值，才开启PPG抖频功能
#define  	C_MinPower_PPGDP 		1690/C_Mult_Power
//-------------------------------------------------------------------------
//参数名称：PPG抖频控制值
//参数说明：与市电频率、中断时间（ValueTime_Int）有关联
#define  	C_Data1_PPGDP 			55 //抖频前补偿PPG值
#define  	C_Data2_PPGDP			3 //PPG递减步进 
#define  	C_Data3_PPGDP			2 //PPG递加步进
#define  	C_Time1_PPGDP			56 //抖频开始等待时间
#define  	C_Time2_PPGDP			36  //PPG递减总时间
#define  	C_Time3_PPGDP			54 //PPG递加总时间
#define  	C_Time4_PPGDP			0   //PPG变化暂停时间
/*************************************************************************/
const unsigned int	ValueSlopeIron_Curr_PPGDP = C_SlopeIron_Curr_PPGDP; 
const unsigned int  ValueSlopeSteel_Curr_PPGDP = C_SlopeSteel_Curr_PPGDP;

const unsigned int  ValueMin_PPGTMR_PPGDP = C_MinPPGTMR_PPGDP;
const unsigned char ValueMin_Power_PPGDP = C_MinPower_PPGDP;

const unsigned char ValueData1_PPGDP = C_Data1_PPGDP;
const unsigned char ValueData2_PPGDP = C_Data2_PPGDP;
const unsigned char ValueData3_PPGDP = C_Data3_PPGDP;
const unsigned char ValueTime1_PPGDP = C_Time1_PPGDP;
const unsigned char ValueTime2_PPGDP = C_Time2_PPGDP; 
const unsigned char ValueTime3_PPGDP = C_Time3_PPGDP;
const unsigned char ValueTime4_PPGDP = C_Time4_PPGDP;  
/*************************************************************************/
extern volatile unsigned char Status_PPGDP;		//PPG抖频状态
//-------------------------------------------------------------------------	
extern volatile bit B_PPGDP_Can;				//PPG抖频强制打开 
extern volatile bit B_PPGDP_Dis;				//PPG抖频禁止 
 
/**************************************************************************
--------------------------------- 其他部分 --------------------------------
**************************************************************************/ 		
const unsigned char ValuePPGSub_BackPress = 0;
//-------------------------------------------------------------------------			
const unsigned char	ValueTime_Int = C_TimeUS_Int;	 
const unsigned char	ValueTime_Main = C_TimeMS_Main;	
//-------------------------------------------------------------------------  
const unsigned int 	ValueSlopeIron_Curr = C_SlopeIron_Curr; 
const unsigned int 	ValueSlopeSteel_Curr = C_SlopeSteel_Curr; 
//------------------------------------------------------------------------- 
const unsigned char ValueTime_SteelIron = C_Vaule_SteelIron;
//------------------------------------------------------------------------- 
const unsigned char ValueVol_CoilVol = C_Vol_CoilVol;	
	 
const unsigned char ValueVol_ZeroTurn = C_Vol_ZeroTurn;	 
const unsigned char ValueVol_ZeroTurn_VH = C_Vol_ZeroTurn_VH;	 
const unsigned char ValueVol_ZeroTurn_VL = C_Vol_ZeroTurn_VL;	 
/*************************************************************************/
volatile unsigned char ValueOPA_Set = 0B00000000;	

#ifdef _TEST_PGAOUT_
volatile unsigned char ValuePGA_Set = 0B10000010 | ((C_Times_PGA & 0x03) << 5);	
#else
volatile unsigned char ValuePGA_Set = 0B10000000 | ((C_Times_PGA & 0x03) << 5);	
#endif	

#ifdef _TEST_CMPOUT_
volatile unsigned char ValueCM_Test = 0x08 | (C_CMOUTEN_Num & 0x07);
#else
volatile unsigned char ValueCM_Test = 0;
#endif
//-------------------------------------------------------------------------	
volatile unsigned int  ValuePPG_Single = C_PPGSingle; 	 
volatile unsigned char ValueCnt_HaveNoPan = C_Vaule_HaveNoPan; 
//-------------------------------------------------------------------------	
volatile unsigned char ValueTime_PPGDly = C_PPGDly;  
//-------------------------------------------------------------------------	
volatile unsigned char ValueVol_VolSurge = C_Vol_VolSurge;	
volatile unsigned char ValueVol_CurrSurge = C_Vol_CurrSurge;
volatile unsigned char ValueVol_OnStep = C_Vol_OnStep;	

volatile unsigned char ValueVol_BackPress1 = C_Vol_BackPress1;
volatile unsigned char ValuePPG_BackPress1 = C_PPG_BackPress1;
volatile unsigned char ValueVol_BackPress2 = C_Vol_BackPress2; 
/*************************************************************************/
volatile unsigned int  Count_BackPress;			//1级过压保护计数
volatile unsigned int  BakCount_BackPress;		//1级过压保护计数备份

volatile unsigned int  Delay_CheckPan;			//检锅延时时间
volatile unsigned int  Time_Syn;				//同步脉宽时间 
volatile unsigned char Count_Syn;				//同步次数 
 
volatile unsigned int  SlopeCurr_Set;			//电流斜率设置值

volatile unsigned char PowerWork;				//工作功率值
volatile unsigned int  Buf_PPGTMR;				//PPG导通缓存值
volatile unsigned int  Value_PPGTMR;			//PPG导通临时值
//-------------------------------------------------------------------------
volatile bit B_Zero_No;							//无过零
volatile bit B_Zero_Have;						//有过零
volatile bit B_Zero_Lose;						//过零丢失
volatile bit B_Zero_60Hz;						//过零频率=60Hz

volatile bit B_Zero_Time;						//过零翻转（时间基准）
volatile bit B_Zero_Adc; 						//过零翻转（ADC）

volatile bit B_Vol_Low;							//低压区间
volatile bit B_Vol_Mid;							//中压区间
volatile bit B_Vol_High;						//高压区间

volatile bit B_Adc_Auto;						//ADC自动开始
volatile bit B_Adc_Curr;						//检测工作电流AD
volatile bit B_Adc_Vol;							//检测市电电压AD

volatile bit B_Pan_Check;						//检锅
volatile bit B_Pan_Single;						//单次检锅脉冲输出
volatile bit B_Pan_LoseDly;						//丢锅检测延时
volatile bit B_Pan_Error;						//检锅故障（未接线盘）
volatile bit B_Pan_No;							//无锅
volatile bit B_Pan_Have;						//有锅
volatile bit B_Pan_Iron;						//铁锅 
volatile bit B_Pan_Steel;						//钢锅  

volatile bit B_PPGTMR_ReadLock;					//读PPGTMR锁保护 
volatile bit B_PPGTMR_WriteLock;				//写PPGTMR锁保护 

volatile bit B_Protect_BackPress;				//过压保护
volatile bit B_Protect_Stop;					//硬件保护停止
volatile bit B_ProtectCM_Restart;				//保护比较器重启

volatile bit B_PPG_On;							//PPG开启 
volatile bit B_PPG_Out;							//PPG输出 
volatile bit B_Heat_En;							//加热使能

/**************************************************************************
**************************************************************************/
#endif

