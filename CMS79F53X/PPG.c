/**************************************************************************
**************************************************************************/
#include "Define_Global.h"
  
/**************************************************************************
* 函数名称：Calc_Electric 
* 函数功能：计算工作电量
* 入口参数：无
* 出口参数：无
* 备    注：内部调用，计算结果放于Elec结构中
* 			P*t = 0.01度 = 1度/100 = 1KW.h/100 = 36000W.s
* 			P*7/1000 = 36*7 = 252
**************************************************************************/
void Calc_Electric()
{
	static unsigned char ElecTime,ElecBasic;
	//---------------------------------------------------------------------	
	if(B_PPG_Out && (0 == --ElecTime))	 
	{
		//256*256*256*7/1000/2 = 0xE560，放大256^3倍
		//运算结果取最高位还原数据
		ElecBasic += ((unsigned long)0xE560 * PowerReal) >> 24; 
		
		//1s为7个值，误差补偿4s即28个值，252-4*7=224
		if(ElecBasic >= 224)
		{
			ElecBasic -= 224;
			
			if(++Elec.flo.radix >= 100)	//0.01度
			{
				Elec.flo.radix = 0;
				Elec.flo.integer++;		//1度
			}
		}
		
		ElecTime = 500/C_TimeMS_Main;	//0.5s
	}
}  
 
/**************************************************************************
* 函数名称：Igbt_Protect 
* 函数功能：IGBT硬件保护 
* 入口参数：无
* 出口参数：无
* 备    注：内部调用
**************************************************************************/
void Igbt_Protect() 
{ 
	static unsigned int Delay_ProtectStop;
	static bit B_Bak_IgbtProtect;		
	
	volatile unsigned char i;	
	//---------------------------------------------------------------------	
	if(CM1COF || CM2IF || CM3IF || CM4IF || CM5IF || PPGWDTIF)
	{
		i = 0;		
		if(CM1COF)		SetBit(i,0); 	//1级过压比较器计数溢出
		if(CM2IF)		SetBit(i,1); 	//电压浪涌
		if(CM3IF)		SetBit(i,2); 	//电流浪涌
		if(CM4IF)		SetBit(i,3); 	//2级过压
		if(CM5IF)		SetBit(i,4); 	//导通台阶过高
		if(PPGWDTIF)	SetBit(i,5); 	//PPG看门狗溢出	
		
		Status_PPGStop = i;				//PPG停止状态	
  
		B_ProtectCM_Restart = 1;		//保护比较器重启
		B_Protect_Stop = 1;				//硬件保护停止
		Delay_ProtectStop = 0;
		
		if(!B_Bak_IgbtProtect)
		{
			if(Count_IgbtProtect < 0xFFFF)
			{
				Count_IgbtProtect++;	//IGBT硬件保护计数+1
			}
		}
		B_Bak_IgbtProtect = 1;			//备份状态 
	}
	else
	{
		B_Bak_IgbtProtect = 0;			//备份状态 
	}
	//---------------------------------------------------------------------	
	if(B_Protect_Stop)
	{	  								//硬件保护停止
		Flag_PPG = 0;					//PPG输出关闭
		B_PPG_On = B_PPG_Out = 0; 	
		B_Pan_Error = B_Pan_No = B_Pan_Have = 0;			 

		if(++Delay_ProtectStop > 2000/C_TimeMS_Main)	//2s
		{
			Delay_ProtectStop = 0;	
					
			B_Protect_Stop = 0;			//退出硬件保护停止状态
			Status_PPGStop = 0;	
		}
	}
	//---------------------------------------------------------------------		
	CM1COF = 0;							//清比较器标志
	CM2IF = 0;						 
	CM3IF = 0;					 
	CM4IF = 0;					 
	CM5IF = 0;						 
	PPGWDTIF = 0;		
	
	if(!B_Heat_En)
	{									//加热关闭
		Count_IgbtProtect = 0;			//清IGBT硬件保护计数
	}
} 
 
/**************************************************************************
* 函数名称：Set_SlopeCurr
* 函数功能：电流斜率设置
* 入口参数：无
* 出口参数：无
* 备    注：内部调用
**************************************************************************/
void Set_SlopeCurr()
{  	
	static unsigned char OldAdjust;
	static bit B_Init_CurrSlope;
	
	volatile unsigned char i,j;			
	//---------------------------------------------------------------------	
	if(!B_Init_CurrSlope)					//电流斜率调整值初始化
	{
		i = Memory_Read(C_EEAddr_CheckAA);
		j = Memory_Read(C_EEAddr_Check55);
	 
		if((0xAA == i) && (0x55 == j))		
		{
			i = Memory_Read(C_EEAddr_DataP);
			j = (unsigned)~Memory_Read(C_EEAddr_DataI);
			
			if(i == j)
			{ 
				SlopeCurr_Adjust = i;		//EE数据校验通过，选用EE读取值
			}
			else
			{
				SlopeCurr_Adjust = 0x80;	//EE数据校验不通过，选用默认值				
			}		
		}
		else
		{
			SlopeCurr_Adjust = 0x80;		//EE数据校验不通过，选用默认值
		}
		
		OldAdjust = SlopeCurr_Adjust;
		B_Init_CurrSlope = 1;				//初始化完成			
	}
	//---------------------------------------------------------------------	
	if(OldAdjust != SlopeCurr_Adjust)		//电流斜率调整值写入EE
	{  	
		i = 0;
		i |= Memory_Write(C_EEAddr_CheckAA,0xAA);
		i |= Memory_Write(C_EEAddr_Check55,0x55);
		i |= Memory_Write(C_EEAddr_DataP,SlopeCurr_Adjust);
		i |= Memory_Write(C_EEAddr_DataI,(unsigned)~SlopeCurr_Adjust);

		if(0 == i)	
		{									//写入EE成功
			OldAdjust = SlopeCurr_Adjust; 	//更新电流斜率调整值旧值
		}		
	}
	//---------------------------------------------------------------------		
	if(SlopeCurr_Adjust & 0x80)				//电流斜率调整
	{
		SlopeCurr = SlopeCurr_Set + (((unsigned int)SlopeCurr_Adjust & 0x7F) << 4); 
	} 
	else  
	{
		SlopeCurr = SlopeCurr_Set - (((unsigned int)0x80 - SlopeCurr_Adjust) << 4); 
	}		
	
	#if _MODE_IHDEBUG_
	if(B_Test_SlopeCurr)					//电流斜率测试
	{	
		SlopeCurr = SlopeCurr_Test;			//测试电流斜率
	}
	#endif	
}

/**************************************************************************
* 函数名称：Set_PPGPower 
* 函数功能：PPG功率设置
* 入口参数：无
* 出口参数：无
* 备    注：内部调用 
**************************************************************************/
void Set_PPGPower()
{
	static unsigned char Cnt_AddPPG,Cnt_SubPPG;
	static unsigned int  Delay_LosePan;	
	static bit B_PPGStop1,B_PPGStop2;	 
 
	volatile unsigned int realpower,setpower;
	volatile unsigned int data;		 			
	volatile unsigned char i,j,x,y;		
	/*********************************************************************/	
	if(!B_CurrAdc_Fst || !B_PPG_En)
	{										//电流AD初值检测未完成、未使能PPG输出
		B_PPG_On = 0;						//PPG停止
		B_PPG_Out = 0;						//PPG输出关闭		

		B_PPGStop1 = 1;						//PPG停止
		B_PPGStop2 = 0;	
		
		Delay_CheckPan = 0;					//清检锅延时时间																		
	}
	else if(B_PPGStop_CheckPan || B_PPGStop_NoCheck)
	{									
		B_PPG_On = 0;						//PPG停止
		B_PPG_Out = 0;						//PPG输出关闭
 
		B_PPGStop1 = 0;		
		B_PPGStop2 = 1;						//PPG暂停	 
 
		if(B_PPGStop_CheckPan || B_Pan_No)
		{				
			if(Delay_CheckPan)
			{								//检锅延时计时  
				Delay_CheckPan--;
			}
			else
			{								//检锅延时完成
				Delay_CheckPan = C_Time_StopCheck;	//PPG暂停检锅间隔时间 
							
				B_Pan_Check = 1;			//检锅 									
			}				
		}
		else
		{
			Delay_CheckPan = 0;				//清检锅延时时间
		}					
	}
	else
	{	
		if(!B_Pan_Have || B_PPGStop1 || B_PPGStop2)
		{						
			B_PPG_Out = 0;					//PPG输出关闭
 
			if(!Delay_CheckPan || B_PPGStop2)
			{								//检锅延时完成 				
				Delay_CheckPan = C_Time_CheckPan;	//正常工作检锅间隔时间 
				
				B_Pan_Check = 1;			//检锅							
			}				
			else			
			{								//检锅延时计时 
				Delay_CheckPan--;
			}	
			
			B_PPGStop1 = B_PPGStop2 = 0;				
		}		
		else if(B_Pan_Check)
		{
			B_PPG_Out = 0;
		}
		else
		{ 
			Delay_CheckPan = 0;				//清检锅延时时间		
			
			if(!B_PPG_Out)
			{
				B_PPG_Out = 1;				//PPG输出

				B_PPGTMR_WriteLock = 1;		//写上锁										
				if(B_Vol_Low)
				{					
					if(B_Pan_Iron) 
					{
						Buf_PPGTMR = C_PPGFirst_VL_Iron;	
					}
					else
					{
						Buf_PPGTMR = C_PPGFirst_VL_Steel;	
					}					
				}
				else
				{
					if(B_Pan_Iron) 
					{
						Buf_PPGTMR = C_PPGFirst_Iron;	
					}
					else
					{
						Buf_PPGTMR = C_PPGFirst_Steel;	
					}									
				}			
				B_PPGTMR_WriteLock = 0;		//写解锁								
			} 								
		}
		
		B_PPG_On = 1;						//PPG开启	
	}
	//---------------------------------------------------------------------	
	if(!B_PPG_Out)	
	{										//PPG输出关闭
		B_CurrAdc_Once = 0;	
		CurrentReal = PowerReal = 0;		
		
		B_Pan_LoseDly = 0;					 
		return;	
	}		
	/*********************************************************************/	
	//实际功率计算
	//斜率放大了256^2倍，运算结果取中间2字节相当于实际电流放大了256倍
	CurrentReal = ((unsigned long)SlopeCurr * AD_Current) >> 8;
	i = SlopeCurr >> 8;	
	
	if(B_Vol_Low)
	{										//低压区间的实际功率计算
		PowerReal = ((unsigned long)C_Vol_Low * CurrentReal) >> 8;
		realpower = ((unsigned long)C_Vol_Low * (CurrentReal + i)) >> 8;		
	}
	else								 
	{	
		PowerReal = (((unsigned long)Vol.all + (C_Vol_Comp << 8)) * CurrentReal) >> 16;
		realpower = (((unsigned long)Vol.all + (C_Vol_Comp << 8)) * (CurrentReal + i)) >> 16;	
	}   
	//---------------------------------------------------------------------	
	//设定功率计算
	PowerSet = (unsigned int)PowerWork * C_Mult_Power;	//其他区间的设定功率值计算 

	if(B_Vol_Low)
	{										//低压区间的设定功率值计算	
		PowerSet = Buf_Power_CurrerConst;	
	}
	else if(B_Vol_Mid)
	{										//低中压区间的设定功率值计算	
		if(Vol.flo.integer < (C_Vol_Low - C_Vol_Comp))
		{
			PowerSet = Buf_Power_CurrerConst;	
		}
		else if(Vol.flo.integer <= (C_Vol_Mid - C_Vol_Comp))
		{									//线性下降功率，相同档位下，不同的电压采用不同的功率
			if(PowerSet > Buf_Power_CurrerConst)
			{
				data = PowerSet - Buf_Power_CurrerConst;	
				data = (unsigned long)data * 256 / (C_Vol_Mid - C_Vol_Low);		
				data = ((unsigned long)data * (unsigned char)(C_Vol_Mid - C_Vol_Comp - Vol.flo.integer)) >> 8;
			
				PowerSet -= data;			
			}					
		}
	}
	/*********************************************************************/	
	if(!B_Pan_LoseDly)
	{										//电流丢锅检测延时未完成 
		B_CurrAdc_Once = 0; 		
	} 	
	#ifdef _TEST_PPGOUT_ 					//PPG输出测试
	else if(1) 
	{
		B_CurrAdc_Once = 0; 	
		
		B_PPGTMR_WriteLock = 1;				//写上锁										
		Buf_PPGTMR = _TEST_PPGOUT_;						
		B_PPGTMR_WriteLock = 0;				//写解锁	
	}	
	#elif _MODE_IHDEBUG_
	else if(B_Test_PPGOut && PPGTMR_TestPPG)
	{
		B_CurrAdc_Once = 0; 	
		
		B_PPGTMR_WriteLock = 1;				//写上锁										
		Buf_PPGTMR = PPGTMR_TestPPG;			
		B_PPGTMR_WriteLock = 0;				//写解锁	
	}	
	#endif	
	//---------------------------------------------------------------------	
	//调整功率：根据实际功率和设定功率，输出PPG			
	else if(B_CurrAdc_Once)
	{						
		B_CurrAdc_Once = 0; 				//工作电流ADC完成一次，调整一次功率

		B_PPGTMR_ReadLock = 1;				//读上锁
		Value_PPGTMR = Buf_PPGTMR;			//读取当前PPGTMR值
		B_PPGTMR_ReadLock = 0;				//读解锁	
	 
		B_PPGTMR_Adjust = 1;				//PPGTMR调整
	  
		if((PowerReal < PowerSet) && (realpower < PowerSet))
		{									//实际功率比设定功率小，增加PPG									
			Cnt_SubPPG = 0;
						
			data = PowerSet - PowerReal;	//获得功率差值
			x = data;
			y = data >> 8; 
			
			if(0 == y)
			{
								{i = 2;	j = 1;}		//功率差值<8W
				if(x & 0x08)	{i = 2;	j = 1;}		//功率差值>=8W
				if(x & 0x10)	{i = 2;	j = 2;}		//功率差值>=16W
				if(x & 0x20)	{i = 1;	j = 3;}		//功率差值>=32W			
				if(x & 0x40)	{i = 1;	j = 4;}		//功率差值>=64W					
				if(x & 0x80)	{i = 1;	j = 5;}		//功率差值>=128W					
			}			
			else 					
			{								//功率差值>=256W 
				i = 0;	j = 10;			
			} 

			if(B_Protect_BackPress)
			{								//过压保护
				Cnt_AddPPG = 0;
			}
			else if(++Cnt_AddPPG >= i)
			{
				Cnt_AddPPG = 0;		 
				Value_PPGTMR += j;			//PPG增加
		 
					 if(B_Pan_Iron)		data = Buf_PPGMax_Iron; 
				else if(B_Pan_Steel)	data = Buf_PPGMax_Steel; 
				else 					data = Buf_PPGMax;

				if(Value_PPGTMR > data) 	//判断是否已达到设定最大值
				{				
					Value_PPGTMR = data;			
				}
			}
		}
		else if((PowerReal > PowerSet) && (realpower > PowerSet))
		{									//实际功率比设定功率大，减小PPG
			Cnt_AddPPG = 0;
			
			data = realpower - PowerSet;	//获得功率差值
			x = data;
			y = data >> 8;  
   
			if(0 == y)
			{
								{i = 4;	j = 1;}		//功率差值<8W
				if(x & 0x08)	{i = 4;	j = 1;}		//功率差值>=8W
				if(x & 0x10)	{i = 4;	j = 2;}		//功率差值>=16W
				if(x & 0x20)	{i = 2;	j = 3;}		//功率差值>=32W			
				if(x & 0x40)	{i = 1;	j = 16;}	//功率差值>=64W					
				if(x & 0x80)	{i = 0;	j = 32;}	//功率差值>=128W					
			}			
			else 					
			{								//功率差值>=256W 
				i = 0;	j = 64;			
			} 

			if(++Cnt_SubPPG >= i)
			{
				Cnt_SubPPG = 0;	 
				Value_PPGTMR -= j;			//PPG减小	
	 
				data = Buf_PPGMin;
				 
				if(Value_PPGTMR < data)		//判断是否已达到设定最小值
				{				
					Value_PPGTMR = data;			
				}
			}
		}			
		else
		{									//实际功率跟设定功率相等	
			Cnt_AddPPG = 0;
			Cnt_SubPPG = 0;			
		}

		if(B_PPGTMR_Adjust)
		{
			B_PPGTMR_Adjust = 0;
			
			B_PPGTMR_WriteLock = 1;			//写上锁	
			Buf_PPGTMR = Value_PPGTMR;		//写回当前PPGTMR值		
			B_PPGTMR_WriteLock = 0;			//写解锁	
		}		
	}
	/*********************************************************************/	
	//电流丢锅判断：
	if(!B_Pan_LoseDly)						//开启PPG延时一定时间，才检测丢锅
	{									 
		if(B_Vol_Low)						
		{
			if(B_Pan_Iron) 
			{
				data = C_PPGStart_VL_Iron;	
			}
			else
			{
				data = C_PPGStart_VL_Steel;	
			}			
		}
		else 
		{
			if(B_Pan_Iron) 
			{
				data = C_PPGStart_Iron;	
			}
			else
			{
				data = C_PPGStart_Steel;	
			}	
		}
 
		B_PPGTMR_ReadLock = 1;				//读上锁
		Value_PPGTMR = Buf_PPGTMR;			//读取当前PPGTMR值
		B_PPGTMR_ReadLock = 0;				//读解锁	
			 
		if(Value_PPGTMR < data)				//功率爬升
		{		
			B_PPGTMR_WriteLock = 1;			//写上锁
			Buf_PPGTMR += 4;
			B_PPGTMR_WriteLock = 0;			//写解锁		
		}
		else
		{
			B_Pan_LoseDly = 1;				//延时完成	
			Delay_LosePan = 1500/C_TimeMS_Main;	//1.5s	
		}
	}  	
	#if _MODE_IHDEBUG_
	else if(B_LosePanCheck_Dis)				//禁止电流丢锅检测
	{
		Delay_LosePan = 1000/C_TimeMS_Main;	//1s 		
	}	
	#endif	
	else 
	{										//开始判断
		if(!B_Vol_Mid)	
		{									//其他区间，电流丢锅值计算
			setpower = Buf_PowerLose_VHigh;	//非低压电流丢锅判断功率
			i = C_Vol_High;						
		}		
		else
		{									//低压\低中压区间，电流丢锅值计算			
			setpower = Buf_PowerLose_VLow;	//低压电流丢锅判断功率
			i = C_Vol_Low; 			
		}
		realpower = ((unsigned long)i * CurrentReal) >> 8;		
 		
		if(setpower > realpower)
		{		
			if(Delay_LosePan)
			{
				Delay_LosePan--;
			}						 
			else 
			{
				B_Pan_LoseDly = 0;					
				
				B_Pan_No = 1;				//无锅状态			
				B_Pan_Have = 0;	 				
			}																	
		}
		else if(Delay_LosePan > 500/C_TimeMS_Main)
		{
			Delay_LosePan--;
		}
		else
		{
			Delay_LosePan = 500/C_TimeMS_Main;	//500ms 	
		}
	}
}
 
/**************************************************************************
* 函数名称：Set_PPGWork 
* 函数功能：PPG工作设置
* 入口参数：无
* 出口参数：无
* 备    注：主循环调用
**************************************************************************/
void Set_PPGWork()
{	
	__CMS_Plus_PPG();		//PPG增强处理
	
	Igbt_Protect();			//IGBT硬件保护 
	Set_SlopeCurr();		//电流斜率设置			
	Set_PPGPower();			//PPG功率设置
	
	Calc_Electric();		//计算工作电量			
}
 
