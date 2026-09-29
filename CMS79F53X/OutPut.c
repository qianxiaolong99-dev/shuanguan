/**************************************************************************
**************************************************************************/
#include "Define_Global.h"
 
/**************************************************************************
* 函数名称：OutPut_PWM
* 函数功能：PWM输出 
* 入口参数：无
* 出口参数：无
* 备    注：内部调用
**************************************************************************/
void OutPut_PWM()
{	
	volatile unsigned char i;		
	volatile unsigned char bak_PWMCON0 = 0B01100000;	//PWM时钟分频=32 
	volatile unsigned char bak_PWMCON1 = 0;				
	//---------------------------------------------------------------------
	//风扇：
	#if _FAN_OUT_PWM_	
	if(PwmDuty_Fan)
	{
		PwmDuty_Fan--; 
		PwmDuty_Fan &= 0x3FF; 
	}
	if(PwmPeriod_Fan)
	{		
		PwmPeriod_Fan--; 
		PwmPeriod_Fan &= 0x3FF; 
	}	

	#if (C_PwmChannel_Fan>>4)==0		
	PWMDH &= ~(3 << 0);				//Bit0~Bit1：PWM0占空比高2位	
	if(PwmDuty_Fan & 0x0200)	
	{
		PWMDH |= (1 << 1);	
	}
	if(PwmDuty_Fan & 0x0100)	
	{
		PWMDH |= (1 << 0);	
	}				
	PWMD0L = PwmDuty_Fan;			//PWM0占空比低8位
	
	PWMTH &= ~(3 << 0);				//Bit0~Bit1：PWM0\PWM1周期高2位
	if(PwmPeriod_Fan & 0x0200)	
	{
		PWMTH |= (1 << 1);	
	}
	if(PwmPeriod_Fan & 0x0100)	
	{
		PWMTH |= (1 << 0);	
	}				
	PWMTL = PwmPeriod_Fan;			//PWM0~PWM1周期低8位		

	if(PwmDuty_Fan)
	{
		bak_PWMCON0 |= (1 << 0);	//使能PWM0		
	}
	#if (C_PwmChannel_Fan&0xF)==0xA	
	bak_PWMCON1 |= 0;				//PWM的IO位置选择：PWM0A  
	#elif (C_PwmChannel_Fan&0xF)==0xB	
	bak_PWMCON1 |= 1;				//PWM的IO位置选择：PWM0B		
	#elif (C_PwmChannel_Fan&0xF)==0xC	
	bak_PWMCON1 |= 2;				//PWM的IO位置选择：PWM0C	
	#elif (C_PwmChannel_Fan&0xF)==0xD	
	bak_PWMCON1 |= 3;				//PWM的IO位置选择：PWM0D	
	#endif			
 	
	#elif (C_PwmChannel_Fan>>4)==1
	PWMDH &= ~(3 << 2);				//Bit2~Bit3：PWM1占空比高2位
	if(PwmDuty_Fan & 0x0200)	
	{
		PWMDH |= (1 << 3);	
	}
	if(PwmDuty_Fan & 0x0100)	
	{
		PWMDH |= (1 << 2);	
	}	
	PWMD1L = PwmDuty_Fan;			//PWM1占空比低8位
	
	PWMTH &= ~(3 << 0);				//Bit0~Bit1：PWM0\PWM1周期高2位
	if(PwmPeriod_Fan & 0x0200)	
	{
		PWMTH |= (1 << 1);	
	}
	if(PwmPeriod_Fan & 0x0100)	
	{
		PWMTH |= (1 << 0);	
	}				
	PWMTL = PwmPeriod_Fan;			//PWM0~PWM1周期低8位		
 
	if(PwmDuty_Fan)
	{
		bak_PWMCON0 |= (1 << 1);	//使能PWM1		
	}	
	#if (C_PwmChannel_Fan&0xF)==0xA	
	bak_PWMCON1 |= (0 << 2);		//PWM的IO位置选择：PWM1A  
	#elif (C_PwmChannel_Fan&0xF)==0xB	
	bak_PWMCON1 |= (1 << 2);		//PWM的IO位置选择：PWM1B		
	#elif (C_PwmChannel_Fan&0xF)==0xC	
	bak_PWMCON1 |= (2 << 2);		//PWM的IO位置选择：PWM1C	
	#elif (C_PwmChannel_Fan&0xF)==0xD	
	bak_PWMCON1 |= (3 << 2);		//PWM的IO位置选择：PWM1D	
	#endif		
 						
	#elif (C_PwmChannel_Fan>>4)==2
	PWMDH &= ~(3 << 4);				//Bit4~Bit5：PWM2占空比高2位
	if(PwmDuty_Fan & 0x0200)	
	{
		PWMDH |= (1 << 5);	
	}
	if(PwmDuty_Fan & 0x0100)	
	{
		PWMDH |= (1 << 4);	
	}		
	PWMD2L = PwmDuty_Fan;			//PWM2占空比低8位	
	
	PWMTH &= ~(3 << 2);				//Bit2~Bit3：PWM2周期高2位
	if(PwmPeriod_Fan & 0x0200)	
	{
		PWMTH |= (1 << 3);	
	}
	if(PwmPeriod_Fan & 0x0100)	
	{
		PWMTH |= (1 << 2);	
	}					
	PWM2TL = PwmPeriod_Fan;			//PWM2周期低8位
	
	if(PwmDuty_Fan)
	{
		bak_PWMCON0 |= (1 << 2);	//使能PWM2		
	}	
	#if (C_PwmChannel_Fan&0xF)==0xA	
	bak_PWMCON1 |= (0 << 4);		//PWM的IO位置选择：PWM2A  
	#elif (C_PwmChannel_Fan&0xF)==0xB	
	bak_PWMCON1 |= (1 << 4);		//PWM的IO位置选择：PWM2B		
	#elif (C_PwmChannel_Fan&0xF)==0xC	
	bak_PWMCON1 |= (2 << 4);		//PWM的IO位置选择：PWM2C	
	#elif (C_PwmChannel_Fan&0xF)==0xD	
	bak_PWMCON1 |= (3 << 4);		//PWM的IO位置选择：PWM2D	
	#endif				
	#endif	
	#endif						 
	//---------------------------------------------------------------------
	//蜂鸣器：
	if(PwmDuty_Buzz)
	{
		PwmDuty_Buzz--; 
		PwmDuty_Buzz &= 0x3FF;
	}
	if(PwmPeriod_Buzz) 	
	{	
		PwmPeriod_Buzz--; 
		PwmPeriod_Buzz &= 0x3FF; 
	}	
  
	#if (C_PwmChannel_Buzz>>4)==0		
	PWMDH &= ~(3 << 0);				//Bit0~Bit1：PWM0占空比高2位	
	if(PwmDuty_Buzz & 0x0200)	
	{
		PWMDH |= (1 << 1);	
	}
	if(PwmDuty_Buzz & 0x0100)	
	{
		PWMDH |= (1 << 0);	
	}				
	PWMD0L = PwmDuty_Buzz;			//PWM0占空比低8位
	
	PWMTH &= ~(3 << 0);				//Bit0~Bit1：PWM0\PWM1周期高2位
	if(PwmPeriod_Buzz & 0x0200)	
	{
		PWMTH |= (1 << 1);	
	}
	if(PwmPeriod_Buzz & 0x0100)	
	{
		PWMTH |= (1 << 0);	
	}				
	PWMTL = PwmPeriod_Buzz;			//PWM0~PWM1周期低8位		

	if(PwmDuty_Buzz)
	{
		bak_PWMCON0 |= (1 << 0);	//使能PWM0		
	}
	#if (C_PwmChannel_Buzz&0xF)==0xA	
	bak_PWMCON1 |= 0;				//PWM的IO位置选择：PWM0A  
	#elif (C_PwmChannel_Buzz&0xF)==0xB	
	bak_PWMCON1 |= 1;				//PWM的IO位置选择：PWM0B		
	#elif (C_PwmChannel_Buzz&0xF)==0xC	
	bak_PWMCON1 |= 2;				//PWM的IO位置选择：PWM0C	
	#elif (C_PwmChannel_Buzz&0xF)==0xD	
	bak_PWMCON1 |= 3;				//PWM的IO位置选择：PWM0D	
	#endif			
 	
	#elif (C_PwmChannel_Buzz>>4)==1
	PWMDH &= ~(3 << 2);				//Bit2~Bit3：PWM1占空比高2位
	if(PwmDuty_Buzz & 0x0200)	
	{
		PWMDH |= (1 << 3);	
	}
	if(PwmDuty_Buzz & 0x0100)	
	{
		PWMDH |= (1 << 2);	
	}	
	PWMD1L = PwmDuty_Buzz;			//PWM1占空比低8位
	
	PWMTH &= ~(3 << 0);				//Bit0~Bit1：PWM0\PWM1周期高2位
	if(PwmPeriod_Buzz & 0x0200)	
	{
		PWMTH |= (1 << 1);	
	}
	if(PwmPeriod_Buzz & 0x0100)	
	{
		PWMTH |= (1 << 0);	
	}				
	PWMTL = PwmPeriod_Buzz;			//PWM0~PWM1周期低8位		
 
	if(PwmDuty_Buzz)
	{
		bak_PWMCON0 |= (1 << 1);	//使能PWM1		
	}	
	#if (C_PwmChannel_Buzz&0xF)==0xA	
	bak_PWMCON1 |= (0 << 2);		//PWM的IO位置选择：PWM1A  
	#elif (C_PwmChannel_Buzz&0xF)==0xB	
	bak_PWMCON1 |= (1 << 2);		//PWM的IO位置选择：PWM1B		
	#elif (C_PwmChannel_Buzz&0xF)==0xC	
	bak_PWMCON1 |= (2 << 2);		//PWM的IO位置选择：PWM1C	
	#elif (C_PwmChannel_Buzz&0xF)==0xD	
	bak_PWMCON1 |= (3 << 2);		//PWM的IO位置选择：PWM1D	
	#endif		
 						
	#elif (C_PwmChannel_Buzz>>4)==2
	PWMDH &= ~(3 << 4);				//Bit4~Bit5：PWM2占空比高2位
	if(PwmDuty_Buzz & 0x0200)	
	{
		PWMDH |= (1 << 5);	
	}
	if(PwmDuty_Buzz & 0x0100)	
	{
		PWMDH |= (1 << 4);	
	}		
	PWMD2L = PwmDuty_Buzz;			//PWM2占空比低8位	
	
	PWMTH &= ~(3 << 2);				//Bit2~Bit3：PWM2周期高2位
	if(PwmPeriod_Buzz & 0x0200)	
	{
		PWMTH |= (1 << 3);	
	}
	if(PwmPeriod_Buzz & 0x0100)	
	{
		PWMTH |= (1 << 2);	
	}					
	PWM2TL = PwmPeriod_Buzz;		//PWM2周期低8位
	
	if(PwmDuty_Buzz)
	{
		bak_PWMCON0 |= (1 << 2);	//使能PWM2		
	}	
	#if (C_PwmChannel_Buzz&0xF)==0xA	
	bak_PWMCON1 |= (0 << 4);		//PWM的IO位置选择：PWM2A  
	#elif (C_PwmChannel_Buzz&0xF)==0xB	
	bak_PWMCON1 |= (1 << 4);		//PWM的IO位置选择：PWM2B		
	#elif (C_PwmChannel_Buzz&0xF)==0xC	
	bak_PWMCON1 |= (2 << 4);		//PWM的IO位置选择：PWM2C	
	#elif (C_PwmChannel_Buzz&0xF)==0xD	
	bak_PWMCON1 |= (3 << 4);		//PWM的IO位置选择：PWM2D	
	#endif				
	#endif																	
	//---------------------------------------------------------------------
	PWMCON1 = bak_PWMCON1;			//PWM的IO位置设置 
	
	if(bak_PWMCON0 != PWMCON0)
	{
		PWMCON0 = bak_PWMCON0;		//PWM时钟分频设置，使能\禁止PWM  
	}	
}
 
/**************************************************************************
* 函数名称：Buzz_Short_One 
* 函数功能：蜂鸣器短鸣一声
* 入口参数：无
* 出口参数：无
* 备    注： 
**************************************************************************/
void Buzz_Short_One()
{
	TimeBuzz = 200/C_TimeMS_Main;	//200ms		
	B_Buzz_Out = 1;					//蜂鸣器输出
}

/**************************************************************************
* 函数名称：Buzz_Long_One 
* 函数功能：蜂鸣器长鸣一声
* 入口参数：无
* 出口参数：无
* 备    注： 
**************************************************************************/
void Buzz_Long_One()
{
	TimeBuzz = 400/C_TimeMS_Main;	//400ms		
	B_Buzz_Out = 1;					//蜂鸣器输出
}

/**************************************************************************
* 函数名称：OutPut_Buzz
* 函数功能：蜂鸣器输出 
* 入口参数：无
* 出口参数：无
* 备    注：内部调用
**************************************************************************/
void OutPut_Buzz()
{
	if(TimeBuzz)					//蜂鸣器鸣叫计时
	{
		TimeBuzz--;
	}	
	else
	{
		B_Buzz_Out = 0;				//蜂鸣器输出停止
	}		
	//---------------------------------------------------------------------
	PwmPeriod_Buzz = 250;			//蜂鸣器PWM周期：250us		
	
	if(B_Buzz_En || B_Buzz_Out)				
	{								//蜂鸣器输出	
		PwmDuty_Buzz = 125;			//蜂鸣器PWM占空比：125us			
	}
	else
	{								//蜂鸣器关闭
		PwmDuty_Buzz = 0;			//蜂鸣器PWM占空比：0us		
	}			
}
 
/**************************************************************************
* 函数名称：OutPut_Fan
* 函数功能：风扇输出 
* 入口参数：无
* 出口参数：无
* 备    注：内部调用
**************************************************************************/
void OutPut_Fan()
{
	static unsigned int Delay_FanOut;			 
	static unsigned char Flag_FanOut;
	//---------------------------------------------------------------------
	if((AD_TmprPan > C_AD_Pan_FanOn) || (AD_TmprIgbt > C_AD_Igbt_FanOn))
	{								//锅底高温、IGBT高温
		SetBit(Flag_FanOut,0);				
	}
	else if((AD_TmprPan < C_AD_Pan_FanOff) && (AD_TmprIgbt < C_AD_Igbt_FanOff))
	{
		ClrBit(Flag_FanOut,0);											
	}	
	//---------------------------------------------------------------------
	if(B_Heat_En)			 
	{								//加热使能	 
		SetBit(Flag_FanOut,1);	
		
		if(TestOne(Flag_FanOut,0))
		{ 							//锅底高温、IGBT高温
			Delay_FanOut = 1000u/C_TimeMS_Main*180;	//3min	
		}
		else
		{							//锅底和IGBT温度正常
			Delay_FanOut = 0;		
		}	
	}
	else
	{
		ClrBit(Flag_FanOut,1);	
	}	
	//---------------------------------------------------------------------
	if(B_Fan_En || B_Zero_Lose)			 
	{								//风扇使能、过零丢失	 
		SetBit(Flag_FanOut,2);		
	}
	else
	{
		ClrBit(Flag_FanOut,2);	
	}	

	//---------------------------------------------------------------------
	if(Delay_FanOut)		 
	{								//风扇延时
		Delay_FanOut--;
		SetBit(Flag_FanOut,3);		
	}
	else
	{
		ClrBit(Flag_FanOut,3);		
	}
	//---------------------------------------------------------------------
	#if !_FAN_OUT_PWM_
	if(Flag_FanOut && B_CurrAdc_Fst)			
	{									
		Pin_Fan = 1;				//风扇输出（IO口输出高）	
	}
	else
	{			
		Pin_Fan = 0;				//风扇输出停止（IO口输出低）		
	}		
	//---------------------------------------------------------------------
	#else	
	PwmPeriod_Fan = 60;				//风扇PWM周期：60us
	
	if(Flag_FanOut && B_CurrAdc_Fst)			
	{	
		if((AD_TmprPan > C_AD_Pan_FanLev3) || (AD_TmprIgbt > C_AD_Igbt_FanLev3))//有一个比3档风速温度高
		{
			PwmDuty_Fan = 60;			//风扇PWM占空比：60us		
		}
		else if ((AD_TmprPan < C_AD_Pan_FanLev2) && (AD_TmprIgbt < C_AD_Igbt_FanLev2))//两个都比2档风速温度低
		{
			PwmDuty_Fan = 30;
		}
		else
		{
			PwmDuty_Fan = 15;
		}
	}
	else
	{	
		PwmDuty_Fan = 0;			//风扇PWM占空比：0us		
	}
	#endif			
}
 
/**************************************************************************
* 函数名称：OutPut_Heat
* 函数功能：加热输出 
* 入口参数：无
* 出口参数：无
* 备    注：内部调用
**************************************************************************/
void OutPut_Heat()
{
	if(B_Protect_Stop)
	{								//IGBT保护停止
		Flag_PPG = 0;				//PPG输出关闭
		B_PPG_On = B_PPG_Out = 0; 		
		B_Pan_Error = B_Pan_No = B_Pan_Have = 0;			
	}	
	#ifndef _TEST_PPGOUT_ 			//PPG输出测试	
	#ifndef _TEST_PANCHECK_ 		//检锅测试	
	#if _MODE_IHDEBUG_			
	else if((!B_Heat_En || Flag1_Error || Flag2_Error) &&\
			 !B_Test_PPGOut && !B_Test_PanCheck)
	#else
	else if(!B_Heat_En || Flag1_Error || Flag2_Error)			 
	#endif		 
	{								//加热关闭、故障
		Flag_PPG = 0;				//PPG输出关闭
		B_PPG_On = B_PPG_Out = 0; 	
		B_Pan_Error = B_Pan_No = B_Pan_Have = 0;				
	}
	#endif
	#endif
	else  
	{								//正常加热输出
		B_PPG_En = 1;				//使能PPG输出

		#ifndef _TEST_PPGOUT_ 		//PPG输出测试
		#if _MODE_IHDEBUG_		
		if(!B_Test_PPGOut)
		#endif
		{		
			if(B_HeatStop_CheckPan)
			{
				B_PPGStop_CheckPan = 1;	//PPG暂停但检锅
			}
			else
			{
				B_PPGStop_CheckPan = 0;
			}
	 
			if(B_HeatStop_NoCheck)
			{
				B_PPGStop_NoCheck = 1;	//PPG暂停不检锅
			}
			else
			{
				B_PPGStop_NoCheck = 0;
			}
		}
		#endif 
		
		#ifdef _TEST_PANCHECK_ 		//检锅测试		
		B_PPGStop_CheckPan = 1;		//PPG暂停但检锅	
		#elif _MODE_IHDEBUG_	
		if(B_Test_PanCheck) 		//检锅测试
		{
			B_PPGStop_CheckPan = 1;	//PPG暂停但检锅	
		}
		#endif					
	}	
}

/**************************************************************************
* 函数名称：Set_OutPut
* 函数功能：输出设置
* 入口参数：无
* 出口参数：无
* 备    注：主循环调用
**************************************************************************/
void Set_OutPut()	
{
	OutPut_PWM();		//PWM输出	
	
	OutPut_Heat();		//加热输出 
	OutPut_Fan();		//风扇输出 	
	OutPut_Buzz();		//蜂鸣器输出 		
}

	