/**************************************************************************
**************************************************************************/
#include "Define_Global.h"
 
/**************************************************************************
* 函数名称：Init_UserRam
* 函数功能：初始化用户寄存器
* 入口参数：无
* 出口参数：无 
* 备    注：初始化调用
**************************************************************************/
void Init_UserRam()
{
	#if _MODE_COMM_NO_
	B_Comm_No = 1;		//无通信模式	 
	#elif _MODE_COMM_HAVE_
	B_Comm_Have = 1;	//有通信模式
	#endif		

	Buzz_Long_One();	//蜂鸣器长鸣1声	
	B_Adc_Curr = 1;		//检测工作电流AD			
	//---------------------------------------------------------------------		
	//配置缓存初始化：
	Buf_PPGMin = C_PPGMin;
	Buf_PPGMax = C_PPGMax; 
	Buf_PPGMax_Steel = C_PPGMax_Steel;
	Buf_PPGMax_Iron = C_PPGMax_Iron; 

	Buf_Power_CurrerConst = (unsigned int)(C_Vol_Low * C_Currer_Const);
	Buf_PowerLose_VHigh = C_PowerLose_VHigh;
	Buf_PowerLose_VLow = C_PowerLose_VLow;

	Buf_Vol_LowErr_In = C_Vol_LowErr_In - C_Vol_Comp;
	Buf_Vol_LowErr_Out = C_Vol_LowErr_Out - C_Vol_Comp;	
	Buf_Vol_HighErr_In	= C_Vol_HighErr_In - C_Vol_Comp;
	Buf_Vol_HighErr_Out = C_Vol_HighErr_Out - C_Vol_Comp;

	//Buf_AD_Pan_FanOn = C_AD_Pan_FanOn; 
	//Buf_AD_Pan_FanOff = C_AD_Pan_FanOff;
	Buf_AD_Pan_OverIn = C_AD_Pan_OverIn;
	Buf_AD_Pan_OverOut = C_AD_Pan_OverOut;

	//Buf_AD_Igbt_FanOn = C_AD_Igbt_FanOn;
	//Buf_AD_Igbt_FanOff = C_AD_Igbt_FanOff;
	Buf_AD_Igbt_OverIn = C_AD_Igbt_OverIn; 
	Buf_AD_Igbt_OverOut = C_AD_Igbt_OverOut;  
	Buf_AD_Igbt_HighIn = C_AD_Igbt_HighIn;
	Buf_AD_Igbt_HighOut = C_AD_Igbt_HighOut; 	
	//---------------------------------------------------------------------				
	ValueOPA_Set = 0B00000000;	

	#ifdef _TEST_PGAOUT_
	ValuePGA_Set = 0B10000010 | ((C_Times_PGA & 0x03) << 5);	
	#else
	ValuePGA_Set = 0B10000000 | ((C_Times_PGA & 0x03) << 5);	
	#endif	

	#ifdef _TEST_CMPOUT_
	ValueCM_Test = 0x08 | (C_CMOUTEN_Num & 0x07);
	#else
	ValueCM_Test = 0;
	#endif
 
	ValuePPG_Single = C_PPGSingle; 	 
	ValueCnt_HaveNoPan = C_Vaule_HaveNoPan; 
	
	ValueTime_PPGDly = C_PPGDly;  
 
	ValueVol_VolSurge = C_Vol_VolSurge;	
	ValueVol_CurrSurge = C_Vol_CurrSurge;
	ValueVol_OnStep = C_Vol_OnStep;	

	ValueVol_BackPress1 = C_Vol_BackPress1;
	ValuePPG_BackPress1 = C_PPG_BackPress1;
	ValueVol_BackPress2 = C_Vol_BackPress2;		
} 

/**************************************************************************
* 函数名称：Clean_Ram
* 函数功能：清通用寄存器
* 入口参数：无
* 出口参数：无 
* 备    注：初始化调用
**************************************************************************/
void Clean_Ram()
{
	CLRWDT();		//清看门狗						 
		
	for(FSR = 0x20; FSR < 0x7F; FSR++)
	{
		INDF = 0;
	}
	for(FSR = 0xA0; FSR < 0xEF; FSR++)
	{
		INDF = 0;
	}	
	IRP = 1;
	
	for(FSR = 0x20; FSR < 0x7F; FSR++)
	{
		INDF = 0;
	}
	for(FSR = 0xA0; FSR < 0xEF; FSR++)
	{
		INDF = 0;
	}
	IRP = 0;
}
 
/**************************************************************************
* 函数名称：Init_System
* 函数功能：初始化系统
* 入口参数：无
* 出口参数：无 
* 备    注：初始化调用
**************************************************************************/
void Init_System()
{
	NOP();
	INTCON = 0;		 				//中断禁止
	NOP();	
	NOP();
	NOP();	
	NOP();		
	CLRWDT();						//清看门狗
 
	OSCCON = 0x70;					//内部振荡频率为32MHz（默认为16MHz）	
	OPTION_REG = 0B00001100;  		//WDT分频：1/16
	
	PORTA = C_Init_PORTA;	 
	PORTB = C_Init_PORTB;			//初始化IO电平
	PORTC = C_Init_PORTC;		 
 
	#if _MODE_COMM_NO_ || !_MODE_COMM_HAVE_	 
	WPUA = SetWPUX_CommNo(0xA); 
	WPUB = SetWPUX_CommNo(0xB);		//初始化IO上拉
	WPUC = SetWPUX_CommNo(0xC);	
	
	TRISA = SetTRISX_CommNo(0xA);
	TRISB = SetTRISX_CommNo(0xB);	//初始化IO方向	
	TRISC = SetTRISX_CommNo(0xC);
	#else
	WPUA = SetWPUX_CommHave(0xA); 
	WPUB = SetWPUX_CommHave(0xB);	//初始化IO上拉
	WPUC = SetWPUX_CommHave(0xC);	
	
	TRISA = SetTRISX_CommHave(0xA);
	TRISB = SetTRISX_CommHave(0xB);	//初始化IO方向	
	TRISC = SetTRISX_CommHave(0xC);
	#endif
 
	PORTA = C_Init_PORTA;	 
	PORTB = C_Init_PORTB;			//初始化IO电平
	PORTC = C_Init_PORTC;		 

	INTCON = 0;		 				//中断禁止	
	INTCON2 = 0;		
	PIR1 = 0; 						//清中断请求标志
	PIR2 = 0;			
	PIR3 = 0;				
	PIE1 = 0; 						//清中断允许标志 
	PIE2 = 0; 		
	PIE3 = 0;
  
	OPTION_REG = 0B00000111;		//Timer0分频：1/256	
	TMR0 = 0;  
	
	TMR1H = 0;
	TMR1L = 0;		  
	T1CON = 0B00110001;				//使能Timer1，预分频=8 
 
	PR2 = C_TimeUS_Int * 4;			//设置Timer2时间
	T2CON = 0B00001100;				//使能Timer2，预分频=1，后分频=2
	TMR2IE = 1;						//Timer2中断使能

	INTCON = 0xC0;					//中断使能	
}

/**************************************************************************
* 函数名称：Refurbish_IO
* 函数功能：刷新IO
* 入口参数：无
* 出口参数：无 
* 备    注：主循环调用，可增强抗干扰能力
**************************************************************************/
void Refresh_IO()
{
	#if !_MODE_COMM_NO_ && !_MODE_COMM_HAVE_	 	
	if(B_Comm_No)
	#endif
	#if _MODE_COMM_NO_ || !_MODE_COMM_HAVE_ 
	{
		WPUA = SetWPUX_CommNo(0xA); 
		WPUB = SetWPUX_CommNo(0xB); 	
		WPUC = SetWPUX_CommNo(0xC);	
		
		TRISA = SetTRISX_CommNo(0xA);
		TRISB = SetTRISX_CommNo(0xB);	
		TRISC = SetTRISX_CommNo(0xC);	
	}
	#endif
	#if !_MODE_COMM_NO_ && !_MODE_COMM_HAVE_			
	else	
	#endif
	#if !_MODE_COMM_NO_	 			
	{
		WPUA = SetWPUX_CommHave(0xA); 
		WPUB = SetWPUX_CommHave(0xB); 	
		WPUC = SetWPUX_CommHave(0xC);	
		
		TRISA = SetTRISX_CommHave(0xA);
		TRISB = SetTRISX_CommHave(0xB);	
		TRISC = SetTRISX_CommHave(0xC);	
	}
	#endif		 
}

/**************************************************************************
* 函数名称：Refurbish_Sfr
* 函数功能：刷新特殊功能寄存器
* 入口参数：无
* 出口参数：无 
* 备    注：主循环调用，可增强抗干扰能力 
**************************************************************************/
void Refresh_Sfr()
{
	PIE1 = 0x02;		
	PIE2 = 0;
	PIE3 = 0; 	
	INTCON = 0xC0;					//中断使能
	INTCON2 = 0;			
 
	OPTION_REG = 0B00000111;		//Timer0分频：1/256	
 
	if(0B00110001 != T1CON) 		//T1CON值不变就不写
	{								//写操作会影响计时，在T1CON没有乱的时候不写
		T1CON = 0B00110000;			
		T1CON |= 0x01;
	}
 
	PR2 = C_TimeUS_Int * 4;			//设置Timer2时间	
	if(0B00001100 != T2CON) 		//T2CON值不变就不写
	{								//写操作会影响计时，在T2CON没有乱的时候不写
		T2CON = 0B00001000;			
		T2CON |= 0x04;
	}
}
 
/**************************************************************************
-------------------------------- 主循环处理 -------------------------------
**************************************************************************/
void main() @0x0800
{
	CRCDL--;CRCDH--;
	
	Clean_Ram();					//清通用寄存器 
	Init_System();   				//初始化系统
	
	#if !_MODE_IHDEBUG_ && (_MODE_COMM_NO_ || !_MODE_COMM_HAVE_) 
	Init_HMI();						//初始化人机界面	
	#endif		
	Delay_Xms(200);  				//上电延时			
	Init_UserRam();					//初始化用户寄存器			 
 
	while(1)
	{
		if(TMR1IF)
		{
			TMR1IF = 0;
			TMR1L = 0xFFFF - C_TimeMS_Main * 4000;				 			
			TMR1H = (0xFFFF - C_TimeMS_Main * 4000) >> 8;
 
			CLRWDT();				//清看门狗						
			Refresh_IO();  			//刷新IO
			Refresh_Sfr();  		//刷新特殊功能寄存器			
			
			Deal_Adc();				//ADC处理
			Set_OutPut();			//输出设置					
			Set_PPGWork();  		//PPG工作设置		

			#if _MODE_IHDEBUG_			
			REL_IhDebug_Main();		//IH调试处理（主循环）	
			#else
			#if !_MODE_COMM_NO_ && !_MODE_COMM_HAVE_	  
			if(B_Comm_No)
			#endif
			#if _MODE_COMM_NO_ || !_MODE_COMM_HAVE_  
			{									 
				Deal_HMI();			//人机界面处理					
			}
			#endif	
			#if !_MODE_COMM_NO_ && !_MODE_COMM_HAVE_			
			else
			#endif	
			#if !_MODE_COMM_NO_	 
			{
				Deal_CommData();	//通信数据处理
			}
			#endif	
			#endif										
		}
	}
}

 