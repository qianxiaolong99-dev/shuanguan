/**************************************************************************
**************************************************************************/
#include "Define_Global.h"
#if      _MODE_IHDEBUG_ 

/**************************************************************************
--------------------------------- 变量定义 --------------------------------
**************************************************************************/
volatile unsigned char Del_SendStatus;
volatile unsigned int  StatusWork;
volatile unsigned int  StatusError;
volatile unsigned int  StatusProtect;

volatile unsigned char Count_IhDebugComm;	
volatile unsigned int  Del_IhDebugComm;		
//------------------------------------------------------------------------- 
volatile bit B_SendStatus_IhDebug;
volatile bit B_TestComm_IhDebug;
volatile bit B_CommErr_IhDebug;
//*************************************************************************
volatile unsigned int  Bak_StatusWork;
volatile unsigned int  Bak_StatusError;
volatile unsigned int  Bak_StatusProtect; 

volatile unsigned char Bak_PowerWork;
volatile unsigned char Bak_TimeAll_JXHeat;
volatile unsigned char Bak_TimeOn_JXHeat; 
volatile unsigned char Bak_Num_LxLowAll;
volatile unsigned char Bak_Num_LxLowOn;
 
volatile unsigned int  Bak_Num_TestCM; 
//------------------------------------------------------------------------- 
volatile bit B_Bak_JXNoCheck;
volatile bit B_Bak_JXCheckPan;
volatile bit B_Bak_LxLow; 

/**************************************************************************
* 函数名称：REL_DealSendCurve
* 函数功能：发送数据处理（曲线）
* 入口参数：id = 发送ID值
* 出口参数：发送数据值 
* 备    注：内部调用
**************************************************************************/
unsigned int REL_DealSendCurve(unsigned char id)
{
	switch(id)
	{		
		case 0:	//【曲线1数据】 	
				return (unsigned int)AD_Volatage;					//市电电压AD【9bit】		 
		case 1:	//【曲线2数据】 			
				return (unsigned int)AD_Current;					//工作电流AD【9bit】				 
		case 2:	//【曲线3数据】 	
				return (unsigned int)AD_TmprPan;					//锅底温度AD【8bit】  				 
		case 3:	//【曲线4数据】 	
				#ifndef	_pAD_TOP
				return	0;
				#else
				return (unsigned int)AD_TmprTop;					//顶部温度AD【8bit】 	 
				#endif
		case 4:	//【曲线5数据】 	
				return (unsigned int)AD_TmprIgbt;					//IGBT温度AD【8bit】	 
		case 5:	//【曲线6数据】 	
				return (unsigned int)AD_VRef;						//内部基准电压AD【9bit】	 
		case 6:	//【曲线7数据】 	
				return (unsigned int)Vol.flo.integer + C_Vol_Comp;	//市电电压（V）
		case 7:	//【曲线8数据】 							
				return (unsigned int)CurrentReal;					//实际工作电流（A*256）			 				 
		case 8:	//【曲线9数据】 	
				return (unsigned int)PowerReal;						//实际输出功率（W）				
		case 9:	//【曲线10数据】 	
				return (unsigned int)PowerSet;						//设置输出功率（W） 
		case 10://【曲线11数据】 	
				return (unsigned int)Buf_PPGTMR;					//当前PPGTMR值	 				 
		case 11://【曲线12数据】 
				return (unsigned int)Count_Syn;						//检锅振荡个数				
		case 12://【曲线13数据】
				return (unsigned int)Time_Syn;						//检锅振荡脉宽				
		case 13://【曲线14数据】 
				return (unsigned int)Count_BackPress;				//1级过压保护计数			
		case 14://【曲线15数据】 					
				return (unsigned int)Count_IgbtProtect;				//IGBT硬件保护计数 									 				
		default: 	
				return 0;								
	}		
}

/**************************************************************************
* 函数名称：REL_DealSendStatus
* 函数功能：发送数据处理（状态）
* 入口参数：id = 发送ID值
* 出口参数：发送ID值 + 发送数据值
* 备    注：内部调用
**************************************************************************/
unsigned long REL_DealSendStatus(unsigned char id)
{
	volatile unsigned long data;	
	//---------------------------------------------------------------------	
	if(B_SendStatus_IhDebug)
	{
		B_SendStatus_IhDebug = 0;	//完成状态发送			
		
		switch(id)
		{
			case 1:	//【工作状态】	
					data = 0xE0;
					data <<= 16;
					data += StatusWork;		
					return data;
			case 2:	//【故障状态】 	 		
					data = 0xE1;
					data <<= 16;
					data += StatusError;
					return data;		
			case 3:	//【保护状态】	
					data = 0xE2;
					data <<= 16;
					data += StatusProtect;		
					return data;			
			default:	
					return 0;					
		}
	}
	else
	{
		return 0;
	}		
}

/**************************************************************************
* 函数名称：REL_DealReceData
* 函数功能：接收数据处理
* 入口参数：id = 接收ID值、value = 接收数据值
* 出口参数：发送ID值 + 发送数据值
* 备    注：内部调用
**************************************************************************/
unsigned long REL_DealReceData(unsigned char id, unsigned int value)			
{
	volatile unsigned long data;	 
	//*********************************************************************		
	if((id & 0xF0) == 0xE0)				//【发送回传】	
	{
		switch(id & 0x0F)
		{
			case 0:
					Bak_StatusWork = value;		//工作状态	 
					break;	 						
			case 1:	
					Bak_StatusError = value;	//故障状态	
					break;	 
			case 2:		
					Bak_StatusProtect = value;	//保护状态	
					break;	 				 	 							 		
			default: 					
					break;	
		}	
	}
	//*********************************************************************	
	else if(id == 0xA0)					//【工作命令】
	{	
		if(TestOne(value,0))
		{
			if(!B_Heat_En)
			{
				SetBit(value,1);		//风扇强制使能
				B_Fan_En = 1;  			//风扇使能
			}						
			B_Heat_En = 1;  			//加热使能		
		}
		else
		{
			B_Heat_En = 0;  			 			
		}	

		if(TestOne(value,1))
		{		
			B_Fan_En = 1;  				//风扇使能
		}
		else
		{	
			B_Fan_En = 0; 				 		
		}	

		if(TestOne(value,2))
		{			
			B_Buzz_En = 1;		  		//蜂鸣器使能		
		}
		else
		{
			B_Buzz_En = 0;					 
		}

		if(TestOne(value,3))
		{		
			ClrBit(value,3);						
			ClrBit(value,2);			//蜂鸣器强制关闭
			B_Buzz_En = 0;		
	 
			if(!B_Buzz_Out)
			{
				Buzz_Short_One();		//蜂鸣器短鸣1声
			}				
		}
 
		if(TestOne(value,4))
		{	
			ClrBit(value,4);										
			ClrBit(value,2);			//蜂鸣器强制关闭
			B_Buzz_En = 0;					
 					
			if(!B_Buzz_Out)
			{
				Buzz_Long_One();		//蜂鸣器长鸣1声
			}
		}	
		
		if(TestOne(value,5))  
		{
			if(!B_Bak_JXNoCheck)
			{
				ClrBit(value,6);		//间隙加热使能（检锅）强制关闭
				B_Bak_JXCheckPan = 0;
			}
			B_Bak_JXNoCheck = 1;  		//间隙加热使能（不检锅）
		}
		else
		{
			B_Bak_JXNoCheck = 0;
		}
		
		if(TestOne(value,6))  
		{
			if(!B_Bak_JXCheckPan)
			{
				ClrBit(value,5);		//间隙加热使能（不检锅）强制关闭	
				B_Bak_JXNoCheck = 0;
			}	
			B_Bak_JXCheckPan = 1;  		//间隙加热使能（检锅）
		}
		else
		{
			B_Bak_JXCheckPan = 0;
		}
 
		if(TestOne(value,7))
		{
			if(!B_PPGDP_Can)
			{	
				ClrBit(value,9);		//连续低功率强制关闭
				B_Bak_LxLow = 0;
			}
			B_PPGDP_Can = !C_PPGDP_ForceOff; // 总开关关闭时屏蔽调试使能
		}
		else
		{
			if(B_PPGDP_Can)
			{
				SetBit(value,8);		//抖频使能强制禁止			
				B_PPGDP_Dis = 1;	 	//抖频禁止
			}
			B_PPGDP_Can = 0;		  
		}
		
		if(TestOne(value,8))
		{
			B_PPGDP_Dis = 1;		 	//抖频禁止
		}		
		else
		{
			B_PPGDP_Dis = C_PPGDP_ForceOff; // 总开关关闭时不允许调试恢复抖频
		}	
 
		if(TestOne(value,9))
		{
			if(!B_Bak_LxLow)
			{
				ClrBit(value,7);		//抖频使能强制关闭
				B_PPGDP_Can = 0;
				SetBit(value,8);		//抖频使能强制禁止			
				B_PPGDP_Dis = 1;	 	//抖频禁止
			}
			B_Bak_LxLow = 1;			//连续低功率使能
		}
		else
		{
			B_Bak_LxLow = 0;		  
		}	
	}	
	//---------------------------------------------------------------------		
	else if((id & 0xF0) == 0xB0)		//【工作参数】 
	{		
		switch(id & 0x0F)
		{
			case 0:	//加热输出功率（×10W） 
					Bak_PowerWork = value;	 
					break;	 						
			case 1:	//加热间隙总时间（×0.5秒）
					Bak_TimeAll_JXHeat = value;
					break;	 
			case 2:	//加热间隙开时间（×0.5秒）	
					Bak_TimeOn_JXHeat = value;		
					break;	 				 
			case 3:	//加热连续低功率总个数	
					Bak_Num_LxLowAll = value;	
					break;	 
			case 4:	//加热连续低功率开个数	
					Bak_Num_LxLowOn = value;				
					break;	 							 		
			default: 					
					break;	
		}
	}
	//---------------------------------------------------------------------		
	else if((id & 0xF0) == 0xC0)		//【配置参数】 
	{
		switch(id & 0x0F)
		{
			case 0:	//PPG导通延时 	
					ValueTime_PPGDly = value;	
					break;			
			case 1:	//电压浪涌 
					ValueVol_VolSurge = value;
					break;	 						
			case 2:	//电流浪涌 
					ValueVol_CurrSurge = value;
					break;	
			case 3:	//导通台阶 
					ValueVol_OnStep = value;
					break;					 
			case 4:	//1级过压 	
					ValueVol_BackPress1 = value;
					break;	 
			case 5:	//1级过压PPG下降步进 	
					ValuePPG_BackPress1 = value;
					break;	 				 
			case 6:	//2级过压 	
					ValueVol_BackPress2 = value;
					break;	 	 							 		
			default: 					
					break;	
		}	
	}		
	//*********************************************************************	
	else if(id == 0x80)					//【测试命令】
	{	
		if(TestOne(value,0))
		{
			if(!B_Test_PanCheck)
			{
				ClrBit(value,1); 		//PPG测试强制关闭
				B_Test_PPGOut = 0; 
			}
			B_Test_PanCheck = 1;  		//检锅测试开启		
		}
		else
		{
			B_Test_PanCheck = 0;  			 			
		}
		
		if(TestOne(value,1))
		{
			if(!B_Test_PPGOut)
			{
				ClrBit(value,0);		//检锅测试强制关闭	
				B_Test_PanCheck = 0;
			}		
			B_Test_PPGOut = 1;  		//PPG测试开启		
		}
		else
		{
			B_Test_PPGOut = 0;  			 			
		}		

		if(TestOne(value,2))
		{		
			B_Test_CM_RA0 = 1;  		//比较器测试开启（RA0）		
		}
		else
		{
			B_Test_CM_RA0 = 0;  			 			
		}	
		
		if(TestOne(value,3))
		{		
			B_Test_CM_RB7 = 1;  		//比较器测试开启（RB7）		
		}
		else
		{
			B_Test_CM_RB7 = 0;  			 			
		}	
		
		if(TestOne(value,4))
		{		
			B_Test_PGA = 1;  			//PGA测试开启（RC0）		
		}
		else
		{
			B_Test_PGA = 0;  			 			
		} 

		if(TestOne(value,5))
		{
			B_Test_SlopeCurr = 1;  		//电流斜率测试开启		
		}
		else
		{
			B_Test_SlopeCurr = 0;  			 			
		}
		
		if(TestOne(value,6))
		{		
			B_ErrorCheck_Dis = 1;  		//禁止硬件故障检测 		
		}
		else
		{
			B_ErrorCheck_Dis = 0;  			 			
		}
		
		if(TestOne(value,7))
		{		
			B_LosePanCheck_Dis = 1;  	//禁止电流丢锅检测 		
		}
		else
		{
			B_LosePanCheck_Dis = 0;  			 			
		}											
	}
	//---------------------------------------------------------------------		
	else if((id & 0xF0) == 0x90)		//【测试参数】 
	{		
		switch(id & 0x0F)
		{
			case 0:	//检锅测试-PPGTMR值
					ValuePPG_Single = value;	 
					break;	 						
			case 1:	//PPG测试-PPGTMR值
					PPGTMR_TestPPG = value;
					break;	 
			case 2:	//比较器测试-通道	
					Bak_Num_TestCM = 0xFF00 | value;	
					break;	 				 
			case 3:	//电流斜率测试-斜率
					SlopeCurr_Test = value;				
					break;	 							 		
			default: 					
					break;	
		}
	}
	//*********************************************************************		
	Del_IhDebugComm = 0;		//清通信故障计时
	B_CommErr_IhDebug = 0;		//清通信故障标志
	B_TestComm_IhDebug = 1;		//开始检测通信故障	

	if((id >= 0xE0) || (id < 0x80))
	{
		return 0;
	}
	else
	{
		data = id;
		data <<= 16;
		data += value;
		return data;
	}				
}

/**************************************************************************
* 函数名称：REL_ReceSendCom
* 函数功能：接收发送公共处理
* 入口参数：无
* 出口参数：无 
* 备    注：主循环调用
**************************************************************************/
void REL_ReceSendCom(void)			
{	
	volatile unsigned int  i;		
		
	static unsigned char Del_JXHeat,Cnt_JXHeat;
	static unsigned char Del_DispBackPress;
	static bit B_DispBackPress;
	 
	/**********************************************************************	
	接收
	**********************************************************************/				
	B_HeatStop_CheckPan = 0;  		 
	B_HeatStop_NoCheck = 0;  	
	
	if((B_Bak_JXCheckPan || B_Bak_JXNoCheck) && Bak_TimeOn_JXHeat && (Bak_TimeAll_JXHeat > Bak_TimeOn_JXHeat))	
	{
		if(++Del_JXHeat >= 500/C_TimeMS_Main)	//0.5s
		{
			Del_JXHeat = 0;
	
			if(++Cnt_JXHeat >= Bak_TimeAll_JXHeat)
			{
				Cnt_JXHeat = 0;
			}
		}
		
		if(Cnt_JXHeat >= Bak_TimeOn_JXHeat)
		{
			if(B_Bak_JXCheckPan)  
			{
				B_HeatStop_CheckPan = 1;  		//不加热但检锅
			}
			else  
			{
				B_HeatStop_NoCheck = 1;  		//不加热不检锅
			}			
		}
	}
	else
	{	
		Del_JXHeat = 0;
		Cnt_JXHeat = 0;		
	}
	//---------------------------------------------------------------------		
	PowerWork = Bak_PowerWork;				
	//*********************************************************************		
	if(B_Bak_LxLow && Bak_Num_LxLowOn && (Bak_Num_LxLowAll > Bak_Num_LxLowOn))
	{	
		Num_LxLowOn = Bak_Num_LxLowOn;	
		Num_LxLowOff = (unsigned)Bak_Num_LxLowAll - Bak_Num_LxLowOn;
	}	
	else
	{
		Num_LxLowOn = 0;	
		Num_LxLowOff = 0;	
	}	
	//*********************************************************************		
	if((B_Test_CM_RA0 || B_Test_CM_RB7) && Bak_Num_TestCM)
	{
		ValueCM_Test = 0x08 | ((unsigned)Bak_Num_TestCM & 0x07);
	}
	else		
	{
		ValueCM_Test = 0;	
	}
	//---------------------------------------------------------------------	
	if(B_Test_PGA)
	{	
		ValuePGA_Set |= 0x02;
	}
	else
	{
		ValuePGA_Set &= ~0x02;
	}	

	/**********************************************************************	
	发送
	**********************************************************************/		
	if(B_Protect_BackPress)	
	{
		B_DispBackPress = 1;		//过压保护显示				
	}	
	
	if(B_DispBackPress)
	{
		if(Del_DispBackPress >= 500/C_TimeMS_Main)	//500ms
		{		
			B_DispBackPress = 0;	//退出过压保护显示	
		}	
		else
		{
			Del_DispBackPress++;
		}	
	}
	else 
	{
		Del_DispBackPress = 0;
	}			 
	//---------------------------------------------------------------------	
	i = 0;			
	if(B_Pan_Error)					SetBit(i,0); 	//检锅故障（未接线盘）
	if(B_Pan_No)					SetBit(i,1); 	//无锅			
	if(B_Pan_Have)					SetBit(i,2); 	//有锅
	if(B_Pan_Steel)					SetBit(i,3); 	//钢锅	
	if(B_Pan_Iron)					SetBit(i,4); 	//铁锅
	
	if(B_Zero_No)					SetBit(i,5); 	//无过零
	else
	{if(!B_Zero_60Hz)				SetBit(i,6); 	//过零频率 = 50Hz	
	else							SetBit(i,7);} 	//过零频率 = 60Hz
	
	if(B_PPG_Out)					SetBit(i,8); 	//正常加热	
	if(B_PPGStop_NoCheck)			SetBit(i,9); 	//加热暂停，不检锅
	if(B_PPGStop_CheckPan)			SetBit(i,10); 	//加热暂停，并检锅
	if(Flag1_Error | Flag2_Error)	SetBit(i,11); 	//故障停止加热			
	if(B_Protect_Stop)				SetBit(i,12); 	//保护停止加热
	if(Status_PPGDP & 0x02)			SetBit(i,13); 	//抖频开启
	if(Status_LxLow & 0x02)			SetBit(i,14); 	//连续低功率开启		
		 		
	StatusWork = i;									//工作状态
	//---------------------------------------------------------------------	
	i = 0;			
	if(B_EVolLow)					SetBit(i,0);	//市电电压过低
	if(B_EVolHigh)					SetBit(i,1);	//市电电压过高
	if(B_EIgbtOpen)					SetBit(i,2);	//IGBT-NTC开路
	if(B_EIgbtClose)				SetBit(i,3);	//IGBT-NTC短路
	if(B_EIgbtOver)					SetBit(i,4);	//IGBT-NTC超温
	if(B_EPanOpen)					SetBit(i,5);	//锅底-NTC开路
	if(B_EPanClose)					SetBit(i,6);	//锅底-NTC短路
	if(B_EPanOver)					SetBit(i,7);	//锅底-NTC超温
	if(B_EPanFail)					SetBit(i,8);	//锅底-NTC失效
	if(B_ETopOpen)					SetBit(i,9);	//顶部-NTC开路
	if(B_ETopClose)					SetBit(i,10);	//顶部-NTC短路
	if(B_ETopFail)					SetBit(i,11);	//顶部-NTC失效
	if(B_CommErr_IhDebug)			SetBit(i,12);	//通信故障
	   
	StatusError = i;								//故障状态
	//---------------------------------------------------------------------	
	i = 0;							
	if(TestOne(Status_PPGStop,0))	SetBit(i,0);	//1级过压比较器计数溢出
	if(TestOne(Status_PPGStop,1))	SetBit(i,1);	//电压浪涌
	if(TestOne(Status_PPGStop,2))	SetBit(i,2); 	//电流浪涌
	if(TestOne(Status_PPGStop,3))	SetBit(i,3); 	//2级过压
	if(TestOne(Status_PPGStop,4))	SetBit(i,4); 	//导通台阶过高
	if(TestOne(Status_PPGStop,5))	SetBit(i,5); 	//PPG看门狗溢出		
	if(B_DispBackPress)				SetBit(i,6);	//过压保护	
	
	StatusProtect = i;								//保护状态	
	//---------------------------------------------------------------------		
	if(++Del_SendStatus >= 100/C_TimeMS_Main)		//150ms
	{
		Del_SendStatus = 0;
		B_SendStatus_IhDebug = 1;	//发送状态	
	}	
	
	/**********************************************************************	
	通信故障
	**********************************************************************/		
	if(!B_TestComm_IhDebug)
	{
		Del_IhDebugComm = 0;		//未开始检测通信故障
		B_CommErr_IhDebug = 0;		//清通信故障标志		
	}
	else if(++Del_IhDebugComm >= 1000/C_TimeMS_Main)	//1s	
	{			
		Del_IhDebugComm = 0;
		B_CommErr_IhDebug = 1;		//通信故障		

		if(++Count_IhDebugComm & 0x01)		
		{
			B_Fan_En = 1;
		}
		else
		{
			B_Fan_En = 0;
		}
						 
		B_Heat_En = 0;	
		B_Buzz_En = 0;	  	 			
		B_Test_PanCheck = 0; 
		B_Test_PPGOut = 0; 
	}		
}


/**************************************************************************
**************************************************************************/
#endif

