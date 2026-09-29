/**************************************************************************
**************************************************************************/
#include "Define_Global.h"
 
/**************************************************************************
* 函数名称：Check_AD_Start
* 函数功能：ADC启动
* 入口参数：无
* 出口参数：无
* 备    注：内部调用 
**************************************************************************/
void Check_AD_Start()
{
	if(B_GetAD_MaxMin || Delay_Adc)
	{									//ADC手动启动
		ADCON2 = 0B00000000;
		ADCON1 = 0B10010011;			//右对齐，Fosc/8，采样周期=8，清ADC自动累加结果		
	}
	else
	{									//ADC自动启动
		if(B_Adc_SetOk)
		{
			return;
		}
		else
		{
			B_Adc_SetOk = 1;
			ADIF = 0;					//清ADC中断标志位
			
			if(B_GetAD_AllSum)
			{
				ADCON2 = 0B01000111;	//ADC自动累和8192次（达到时间停止）	
			}
			else
			{
				ADCON2 = 0B01000011;	//ADC自动累和512次（达到次数停止）				
			}
			
			ADCON1 = 0B10011011;		//右对齐，Fosc/8，采样周期=8，清ADC自动累加结果，硬件自动启动ADC
		}
	}
 
	if(B_Channel_Vol)
	{
		ADCON0 = C_AdcSet_Vol;			//市电电压AD检测设置
	}	
	else if(B_Channel_Curr)
	{
		ADCON0 = C_AdcSet_Curr;			//工作电流AD检测设置
	}	
	else if(B_Channel_Igbt)
	{
		ADCON0 = C_AdcSet_Igbt;			//IGBT温度AD检测设置
	}	
	else if(B_Channel_Pan)
	{
		ADCON0 = C_AdcSet_Pan;			//锅底温度AD检测设置
	}		
	else if(B_Channel_Top)
	{
		ADCON0 = C_AdcSet_Top;			//顶部温度AD检测设置
	}			
	else if(B_Channel_VRef)
	{
		ADCON0 = C_AdcSet_VRef;			//内部参考电压AD检测设置
	}		
	else if(B_Channel_Gnd)
	{
		ADCON0 = C_AdcSet_Gnd;			//GND-AD检测设置
	}	
	else if(B_Channel_Vdd)
	{
		ADCON0 = C_AdcSet_Vdd;			//VDD-AD检测设置
	}		

	NOP();
	NOP();	
	GODONE = 1;
}
/**************************************************************************
* 函数名称：Check_AD_End
* 函数功能：ADC结束
* 入口参数：无
* 出口参数：无
* 备    注：内部调用 
**************************************************************************/
void Check_AD_End()
{
	ADCON0 = 0;
	ADCON1 = 0;
	ADCON2 = 0;		
}
/**************************************************************************
* 函数名称：Check_AD_Ok
* 函数功能：ADC完成
* 入口参数：无
* 出口参数：无
* 备    注：内部调用 
**************************************************************************/
void Check_AD_Ok()
{
	Flag1_Adc = 0;
	B_Adc_Ok = 1;	
		
	Check_AD_End();
}
/**************************************************************************
* 函数名称：Check_AD_Error
* 函数功能：ADC错误
* 入口参数：无
* 出口参数：无
* 备    注：内部调用 
**************************************************************************/
void Check_AD_Error()
{
	Flag1_Adc = 0;
	B_Adc_Error = 1;	
	
	Check_AD_End();	
} 
/**************************************************************************
* 函数名称：Check_AD
* 函数功能：AD检测 
* 入口参数：无
* 出口参数：无
* 备    注：中断调用 
**************************************************************************/
void Check_AD()
{
	volatile unsigned char i;	
	//---------------------------------------------------------------------
	if(B_Zero_Adc)				 
	{										//ADC过零处理
		B_Zero_Adc = 0;						//每次过零周期进来处理一次
		
		if(B_Adc_Curr || B_Adc_Vol)
		{	
			if(!B_Adc_On && !B_Adc_Ok && !B_Adc_Error)
			{								//有ADC操作时，跳开避免冲突
				if(B_Adc_Curr)
				{
					Channel_Adc = 0x02;		//ADC通道切换为检测工作电流	
				}
				else  
				{
					Channel_Adc = 0x01;		//ADC通道切换为检测市电电压				
				}	
				
				B_GetAD_MaxMin = 0;
				B_GetAD_AllSum = 1;			//求AD总和值		
				
				Delay_Adc = 2;
				Flag1_Adc = 0;
				B_Adc_On = 1;				
			}
			
			B_Adc_Curr = 0;
			B_Adc_Vol = 0;
		}		
	}
	//---------------------------------------------------------------------
	if(B_Adc_On)
	{		
		if(Delay_Adc)
		{									//ADC延时
			if(--Delay_Adc)
			{				
				Check_AD_Start();			//提前启动，稳定ADC模块		
			}
			else
			{
				Flag1_Adc &= 0x01;
				
				AdcCount[0] = 0;
				AdcCount[1] = 0;							
				AdcCount[2] = 0;
					
				AdcData[0] = 0;
				AdcData[1] = 0;							
				AdcData[2] = 0;	
				
				Check_AD_Start();			//ADC启动											
			}
		}
		else
		{									//ADC开启	
			if(B_GetAD_AllSum)
			{								//求AD总和值	
				if(B_Zero_60Hz)
				{
					i = 8333/C_TimeUS_Int;	//8.333ms 
				}
				else
				{
					i = 10000/C_TimeUS_Int;	//10ms 
				}
			
				if(++AdcCount[2] >= i)
				{
					AdcCount[2] = 0;					
					ADAUTO_SUM = 0;			//禁止ADC结果自动累加，累加结果保持 
					
					AdcCount[0] = ADCCNTL;
					AdcCount[1] = ADCCNTH;					
					
					AdcData[0] = ADCSUM0;
					AdcData[1] = ADCSUM1;					
					AdcData[2] = ADCSUM2;	
					
					Check_AD_Ok();								
				}			
			}
			else if(B_GetAD_MaxMin)
			{								//求AD最大最小值
				if(GODONE)
				{
					Check_AD_Error();		//若没有完成标志则表示错误，重新设置ADC
				}
				else
				{	
					i = (((unsigned int)ADRESH << 8) + ADRESL) >> 2;	//获得8bit-ADC结果 
 
					if(0 == AdcCount[0])
					{
						AdcData[1] = 0x00;	//ADC最大值	
						AdcData[0] = 0xFF;	//ADC最小值							
					}					
					if(i > AdcData[1])
					{
						AdcData[1] = i;		//更新ADC最大值
					}
					if(i < AdcData[0])
					{
						AdcData[0] = i;		//更新ADC最小值
					}	
										
					if(B_Zero_60Hz)
					{
						i = (8333+2000)/C_TimeUS_Int;	//8.333ms+2ms
					}
					else
					{
						i = (10000+2000)/C_TimeUS_Int;	//10ms+2ms
					}	
							
					if(++AdcCount[0] >= i)
					{
						AdcCount[0] = 0;						
						Check_AD_Ok();		//ADC完成
					}
					else
					{
						Check_AD_Start();	//ADC启动								
					}			
				}
			}
			else
			{								//求AD平均值 	
				if(ADCSUM_OV)
				{
					Check_AD_Error();		//ADC错误	
				}
				else
				{
					if(ADIF)
					{
						ADIF = 0;			//ADC自动累加完成
						
						AdcData[0] = ADCSUM1;
						AdcData[1] = ADCSUM2;	
						
						Check_AD_Ok();		//ADC完成					
					}
				}
			}
		}
	}
	else
	{
		Check_AD_End();				
	}
}

/**************************************************************************
* 函数名称：Adc_Channel_Set
* 函数功能：ADC通道设置 
* 入口参数：i = ADC通道（0=其他、1=市电电压、2=工作电流）
* 出口参数：无 
* 备    注：内部调用 
**************************************************************************/
void Adc_Channel_Set(unsigned char i)
{
	Delay_AdcNoOk = 0;		

	if(i & 0x03)
	{									//检测市电电压或工作电流的AD
		if(B_Zero_No)
		{								//无过零
			if(i & 0x01)
			{
				Channel_Adc = 0x01;		//ADC通道切换为检测市电电压	
			}
			else
			{
				Channel_Adc = 0x02;		//ADC通道切换为检测工作电流
			}
			
			B_GetAD_MaxMin = 0;
			B_GetAD_AllSum = 1;			//求AD总和值
			
			Delay_Adc = 2;
			Flag1_Adc = 0;
			B_Adc_On = 1; 	
		}
		else
		{								//有过零
			Flag1_Adc = 0;			
		
			if(!B_Adc_Auto)				//非ADC自动开启
			{
				if(i & 0x01)
				{								
					B_Adc_Vol = 1;		//检测市电电压AD	
					B_Adc_Curr = 0;	
				}
				else
				{
					B_Adc_Vol = 0;				
					B_Adc_Curr = 1;		//检测工作电流AD				
				}	
			}				
		}
	}
	else							 
	{									//检测其他的AD
		B_GetAD_MaxMin = 0;
		B_GetAD_AllSum = 0;	
		
		Delay_Adc = 2;
		Flag1_Adc = 0;
		B_Adc_On = 1; 		
	}
} 
 
/**************************************************************************
* 函数名称：Get_AdResult
* 函数功能：获取AD结果 
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用 
**************************************************************************/
void Get_AdResult()					 
{
	volatile unsigned int data;	
	volatile unsigned long temp;					
	//---------------------------------------------------------------------
	if(B_Adc_Ok)
	{								//ADC完成，进行数据处理			
		if(B_GetAD_AllSum)
		{ 							//求AD总和值（处理结果为9bit）
			temp = (unsigned long)AdcData[2] << 16;
			temp += (unsigned long)AdcData[1] << 8;
			temp += AdcData[0];	
			
			data = (unsigned int)AdcCount[1] << 8;
			data += AdcCount[0];
			
			temp /= data;
			data = temp >> 1;		 					
		}
		else if(B_GetAD_MaxMin)
		{							//求AD最大最小值（处理结果为8bit）
			data = ((unsigned int)AdcData[1] + AdcData[0]) >> 1;
		}
		else
		{							//求AD平均值（处理结果为9bit） 
			data = (((unsigned int)AdcData[1] << 8) + AdcData[0]) >> 2; 
		}
		
		if(B_Channel_Vol)
		{							//检测市电电压AD值
			if(AD_Volatage)
			{	
				temp = data + AD_Volatage;
				data = temp >> 1;
			}		 
	
			AD_Volatage = data;
			B_VolAdc_Once = 1;		//市电电压ADC完成一次
		}				
		else if(B_Channel_Curr)
		{							//检测工作电流AD值						
			if(!B_CurrAdc_Fst)
			{
				if(data >> 8)
				{
					AD_Current_Fst = 0xFF;
				}
				else
				{
					AD_Current_Fst = data;	//工作电流AD初值				
				}

				B_CurrAdc_Fst = 1;	//工作电流AD初值检测完成
			}
			else
			{		
				if(data >= AD_Current_Fst)
				{					//减去工作电流AD初值
					data -= AD_Current_Fst;		 
				}
				else
				{
					data = 0;		
				}					
		 
				if(AD_Current)
				{
					temp = data + AD_Current;
					data = temp >> 1;
				}

				AD_Current = data;
				B_CurrAdc_Once = 1;	//工作电流ADC完成一次				
			}
		}
		else if(B_Channel_Igbt)
		{							//检测IGBT温度AD值		
			data >>= 1;				//9bit转为8bit	
						
			#if	_UP_NTC_IGBT_
			AD_TmprIgbt = data;		//NTC上拉，AD结果不取反
			#else
			AD_TmprIgbt = ~data;	//NTC下拉，AD结果取反		
			#endif
		}	
		else if(B_Channel_Pan)
		{							//检测锅底温度AD值		
			data >>= 1;				//9bit转为8bit	
										
			#if	_UP_NTC_PAN_
			AD_TmprPan = data;		//NTC上拉，AD结果不取反
			#else
			AD_TmprPan = ~data;		//NTC下拉，AD结果取反		
			#endif
		}			
		else if(B_Channel_Top)
		{							//检测顶部温度AD值		
			data >>= 1;				//9bit转为8bit	
						
			#if	_UP_NTC_TOP_
			AD_TmprTop = data;		//NTC上拉，AD结果不取反
			#else
			AD_TmprTop = ~data;		//NTC下拉，AD结果取反		
			#endif	
		}		
		else if(B_Channel_VRef)
		{							//检测内部基准电压AD值		 
			AD_VRef = data;			
		}	
		else if(B_Channel_Gnd)
		{							//检测GND-AD值
			data >>= 1;				//9bit转为8bit			
			AD_Gnd = data;		
		}	
		else if(B_Channel_Vdd)
		{							//检测VDD-AD值 
			data >>= 1;				//9bit转为8bit			
			AD_Vdd = data;		
		}					
	}
	//---------------------------------------------------------------------
	if(!B_Adc_On && !B_CurrAdc_Fst)	
	{								//工作电流AD初值检测未完成
		Adc_Channel_Set(2);			//检测工作电流AD 	
	}
	else if(B_Adc_Ok || B_Adc_Error || (++Delay_AdcNoOk >= 1000/C_TimeMS_Main))
	{								//ADC完成、错误、超时未完成（1秒）
		if(!Channel_Adc || B_Channel_End)
		{
			Channel_Adc = 0x01;		//ADC通道切换为检测市电电压		
		}	
		else
		{
			Channel_Adc <<= 1;		//ADC通道切换
		}
 
		if(B_Channel_Vol)			
		{
			Adc_Channel_Set(1);		//检测市电电压AD  		
		}		
		else if(B_Channel_Curr)
		{
			Adc_Channel_Set(2);		//检测工作电流AD 
		}		
		else 
		{
			Adc_Channel_Set(0);		//检测其他AD 
		}
	}
} 
 
/**************************************************************************
* 函数名称：Delay_CheckError 
* 函数功能：硬件故障检测延时
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用  
**************************************************************************/ 
void Delay_CheckError()
{
	static unsigned int Delay_CheckError;		

	if(!B_Heat_En)
	{
		B_Dly_CheckErr = 0;		
		Delay_CheckError = 0;
		Min_CheckError = 0;
	}
	else  
	{
		if(++Delay_CheckError >= 1000/C_TimeMS_Main*60)	//1min
		{
			Delay_CheckError = 0;
			Min_CheckError++;
		}
		
		if(Min_CheckError >= 3)		//3min
		{		
			B_Dly_CheckErr = 1;		//硬件故障检测延时完成
		} 
	}
}
 
/**************************************************************************
* 函数名称：Check_IgbtNtcOpen 
* 函数功能：IGBT-NTC开路检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用  
**************************************************************************/ 
void Check_IgbtNtcOpen()
{
	static unsigned char CounterL,CounterH;

	if(AD_TmprIgbt >= C_AD_Igbt_OpenOut)
	{
		CounterL = 0;
		
		if(++CounterH & 0x80)
		{
			CounterH = 0;			
			B_EIgbtOpen = 0;
		}
	}
	else if(!B_Dly_CheckErr)	//故障检测延时完成，才检测开路
	{
		CounterL = 0;
		CounterH = 0;
	}
	else if(AD_TmprIgbt <= C_AD_Igbt_OpenIn) 
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)
		{
			CounterL = 0;			
			B_EIgbtOpen = 1;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}
 
/**************************************************************************
* 函数名称：Check_IgbtNtcClose 
* 函数功能：IGBT-NTC短路检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用
**************************************************************************/ 
void Check_IgbtNtcClose()
{
	static unsigned char CounterL,CounterH;

	if(AD_TmprIgbt >= C_AD_Igbt_CloseIn)
	{
		CounterL = 0;
		
		if(++CounterH & 0x80)
		{
			CounterH = 0;	
					
			if(B_Heat_En)
			{
				B_EIgbtClose = 1;
			}
		}
	}
	else if(AD_TmprIgbt <= C_AD_Igbt_CloseOut)	
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)
		{
			CounterL = 0;						
			B_EIgbtClose = 0;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}
 
/**************************************************************************
* 函数名称：Check_IgbtNtcHigh 
* 函数功能：IGBT-NTC高温检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用，如果高温就降功率 
**************************************************************************/
void Check_IgbtNtcHigh()
{
	static unsigned char CounterL,CounterH;	
	static unsigned int Delay_IgbtDown;				 	
	//---------------------------------------------------------------------
	if(!B_Heat_En)
	{	
		CounterL = 0;
		CounterH = 0;			
		B_IgbtDown = 0;				
	}
	else if(AD_TmprIgbt >= Buf_AD_Igbt_HighIn)
	{
		CounterL = 0;
		
		if(++CounterH & 0x80)
		{
			CounterH = 0;					
			B_IgbtDown = 1;		//IGBT高温降功率 
		}
	}
	else if(AD_TmprIgbt <= Buf_AD_Igbt_HighOut)
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)
		{
			CounterL = 0;						
			B_IgbtDown = 0;		//IGBT高温降功率恢复 			
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
	//---------------------------------------------------------------------
	if(B_IgbtDown)
	{
		if(Delay_IgbtDown)
		{
			Delay_IgbtDown--;	//功率下降计时	
		}
		else
		{
			Delay_IgbtDown = 60000/C_TimeMS_Main;	//1min 
			
			if(Count_IgbtDown < 10)		//最大10次
			{
				Count_IgbtDown++;  		//功率下降次数+1
			} 			
		}
	}
	else
	{
		Delay_IgbtDown = 0;
		Count_IgbtDown = 0;
	}
}

/**************************************************************************
* 函数名称：Check_IgbtNtcOver 
* 函数功能：IGBT-NTC超温检测 
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用，如果干烧就停止工作 
**************************************************************************/
void Check_IgbtNtcOver()
{
	static unsigned char CounterL,CounterH;

	if(AD_TmprIgbt >= Buf_AD_Igbt_OverIn)
	{
		CounterL = 0;
 
		if(CounterH & 0x80)
		{
			if(B_Heat_En)
			{
				CounterH = 0;				
				B_EIgbtOver = 1;
			}
		}
		else
		{
			CounterH++;
		}
	}
	else if(AD_TmprIgbt <= Buf_AD_Igbt_OverOut)
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)
		{
			CounterL = 0;			
			B_EIgbtOver = 0;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}

/**************************************************************************
* 函数名称：Check_TopNtcOpen
* 函数功能：顶部NTC开路检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用  
**************************************************************************/
#ifdef _pAD_TOP
void Check_TopNtcOpen()
{
	static unsigned char CounterL,CounterH;

	if(AD_TmprTop >= C_AD_Top_OpenOut)
	{
		CounterL = 0;
		
		if(++CounterH & 0x80)
		{			
			CounterH = 0;			
			B_ETopOpen = 0;
		}
	}
	else if(!B_Dly_CheckErr)	//故障检测延时完成，才检测开路
	{
		CounterL = 0;
		CounterH = 0;
	}
	else if(AD_TmprTop <= C_AD_Top_OpenIn) 
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)
		{
			CounterL = 0;			
			B_ETopOpen = 1;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}
#endif

/**************************************************************************
* 函数名称：Check_TopNtcClose 
* 函数功能：顶部NTC短路检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用
**************************************************************************/
#ifdef _pAD_TOP
void Check_TopNtcClose()
{
	static unsigned char CounterL,CounterH;

	if(AD_TmprTop >= C_AD_Top_CloseIn)	 
	{
		CounterL = 0;
		
		if(++CounterH & 0x80)
		{
			CounterH = 0;	
					
			if(B_Heat_En)
			{
				B_ETopClose = 1;
			}
		}
	}
	else if(AD_TmprTop <= C_AD_Top_CloseOut) 
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)
		{
			CounterL = 0;			
			B_ETopClose = 0;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}
#endif

/**************************************************************************
* 函数名称：Check_PanNtcOpen
* 函数功能：锅底NTC开路检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用  
**************************************************************************/
void Check_PanNtcOpen()
{
	static unsigned char CounterL,CounterH;

	if(AD_TmprPan >= C_AD_Pan_OpenOut)
	{
		CounterL = 0;
		
		if(++CounterH & 0x80)
		{			
			CounterH = 0;			
			B_EPanOpen = 0;
		}
	}
	else if(!B_Dly_CheckErr)	//故障检测延时完成，才检测开路
	{
		CounterL = 0;
		CounterH = 0;
	}
	else if(AD_TmprPan <= C_AD_Pan_OpenIn) 
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)
		{
			CounterL = 0;			
			B_EPanOpen = 1;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}

/**************************************************************************
* 函数名称：Check_PanNtcClose 
* 函数功能：锅底NTC短路检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用
**************************************************************************/
void Check_PanNtcClose()
{
	static unsigned char CounterL,CounterH;

	if(AD_TmprPan >= C_AD_Pan_CloseIn)	 
	{
		CounterL = 0;
		
		if(++CounterH & 0x80)
		{
			CounterH = 0;	
					
			if(B_Heat_En)
			{
				B_EPanClose = 1;
			}
		}
	}
	else if(AD_TmprPan <= C_AD_Pan_CloseOut) 
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)
		{
			CounterL = 0;			
			B_EPanClose = 0;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}

/**************************************************************************
* 函数名称：Check_PanNtcOver
* 函数功能：锅底NTC超温检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用  
**************************************************************************/
void Check_PanNtcOver()
{
	static unsigned char CounterL,CounterH;

	if(B_PanOver_Dis)
	{		
		CounterL = 0;
		CounterH = 0;
			
		B_EPanOver = 0;
	}	
	else if(AD_TmprPan >= Buf_AD_Pan_OverIn) 
	{
		CounterL = 0;
		
		if(++CounterH & 0x80)
		{
			CounterH = 0;	
					
			if(B_Heat_En) 
			{
				B_EPanOver = 1;
			}
		}
	}
	else if(AD_TmprPan <= Buf_AD_Pan_OverOut)
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)  
		{
			CounterL = 0;			
			B_EPanOver = 0;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;	
	}	
}
 
/**************************************************************************
* 函数名称：Check_VolTooHigh 
* 函数功能：市电电压过高检测
* 入口参数：无
* 出口参数：无                                                       
* 备    注：内部调用
**************************************************************************/
void Check_VolTooHigh()
{
	static unsigned char CounterL,CounterH;

	if(Vol.flo.integer <= Buf_Vol_HighErr_Out)	//高压恢复电压点
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)
		{
			CounterL = 0;			
			B_EVolHigh = 0;
		}
	}
	else if(Vol.flo.integer >= Buf_Vol_HighErr_In)	//高压判断电压点 
	{
		CounterL = 0;
		
		if(++CounterH & 0x80) 
		{
			CounterH = 0;	
					
			if(B_Heat_En)
			{
				B_EVolHigh = 1;
			}
		}
	}	
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}

/**************************************************************************
* 函数名称：Check_VolTooLow 
* 函数功能：市电电压过低检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用  
**************************************************************************/
void Check_VolTooLow()
{
	static unsigned char CounterL,CounterH;

	if(Vol.flo.integer >= Buf_Vol_LowErr_Out)	//低压恢复电压点  
	{
		CounterL = 0;
		
		if(++CounterH & 0x80)
		{
			CounterH = 0;			
			B_EVolLow = 0;
		}
	}
	else if(Vol.flo.integer <= Buf_Vol_LowErr_In)	//低压判断电压点  
	{
		CounterH = 0;
		
		if(++CounterL & 0x80)
		{
			CounterL = 0;  			
			
			if(B_Heat_En)
			{
				B_EVolLow = 1;
			}
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}

/**************************************************************************
* 函数名称：Check_VolHigh 
* 函数功能：市电高压区间判断 
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用 
**************************************************************************/
void Check_VolHigh()
{
	static unsigned char CounterL,CounterH;

	if(Vol.flo.integer >= (C_Vol_High - C_Vol_Comp))
	{
		CounterL = 0;
		
		if(++CounterH & 0x04)
		{
			CounterH = 0;			
			B_Vol_High = 1;
		}
	}
	else if(Vol.flo.integer <= (C_Vol_High - C_Vol_Comp - 5))
	{
		CounterH = 0;
		
		if(++CounterL & 0x04)
		{
			CounterL = 0;			
			B_Vol_High = 0;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}

/**************************************************************************
* 函数名称：Check_VolMid 
* 函数功能：市电中压区间判断 
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用
**************************************************************************/
void Check_VolMid()
{
	static unsigned char CounterL,CounterH;

	if(Vol.flo.integer >= (C_Vol_Mid - C_Vol_Comp))
	{
		CounterL = 0;
		
		if(++CounterH & 0x04)
		{
			CounterH = 0;			
			B_Vol_Mid = 0;
		}
	}
	else if(Vol.flo.integer <= (C_Vol_Mid - C_Vol_Comp - 3))
	{
		CounterH = 0;
		
		if(++CounterL & 0x04)
		{
			CounterL = 0;			
			B_Vol_Mid = 1;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}

/**************************************************************************
* 函数名称：Check_VolLow 
* 函数功能：市电低压区间判断 
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用 
**************************************************************************/
void Check_VolLow()
{
	static unsigned char CounterL,CounterH;

	if(Vol.flo.integer >= (C_Vol_Low - C_Vol_Comp))
	{
		CounterL = 0;
		
		if(++CounterH & 0x04)
		{
			CounterH = 0;			
			B_Vol_Low = 0;
		}
	}
	else if(Vol.flo.integer <= (C_Vol_Low - C_Vol_Comp - 3))
	{
		CounterH = 0;
		
		if(++CounterL & 0x04)
		{
			CounterL = 0;			
			B_Vol_Low = 1;
		}
	}
	else
	{
		CounterL = 0;
		CounterH = 0;
	}
}
 
/**************************************************************************
* 函数名称：Calc_Volatage 
* 函数功能：计算市电电压
* 入口参数：无
* 出口参数：无
* 备    注：内部调用，计算结果放于Volatage结构中
**************************************************************************/
void Calc_Volatage()
{
	static unsigned char CounterL,CounterH;
	
	volatile VolElecType NowVol;
	//---------------------------------------------------------------------	
	if(AD_Volatage > C_AD_VolMax)
	{		
		NowVol.flo.integer = C_Vol_Max - C_Vol_Comp;
		NowVol.flo.radix = 0;		
	}
	else if(AD_Volatage < C_AD_VolMin)
	{
		NowVol.all = 0;		
	}
	else
	{
		NowVol.all = C_Slope_Volatage * (AD_Volatage - C_AD_VolMin);
	}			
	//---------------------------------------------------------------------	
	if(Vol.all)
	{
		if(Vol.flo.integer > NowVol.flo.integer)
		{
			CounterL = 0;
			
			if((Vol.flo.integer - NowVol.flo.integer) >= 5)
			{
				CounterH = 0;				
				Vol.all = NowVol.all;
			}
			else
			{
				if(++CounterH & 0x02)
				{
					CounterH = 0;					
					Vol.flo.integer--;
					Vol.flo.radix = NowVol.flo.radix;
				}
			}
		}
		else if(Vol.flo.integer < NowVol.flo.integer)
		{
			CounterH = 0;
			
			if((NowVol.flo.integer - Vol.flo.integer) >= 5)
			{
				CounterL = 0;				
				Vol.all = NowVol.all;
			}
			else
			{
				if(++CounterL & 0x02)
				{
					CounterL = 0;									
					Vol.flo.integer++;
					Vol.flo.radix = NowVol.flo.radix;					
				}
			}
		}
		else
		{
			CounterL = 0;
			CounterH = 0;
		}
	}
	else
	{
		Vol.all = NowVol.all;
	}
}
 
/**************************************************************************
* 函数名称：Check_TopNtcFail
* 函数功能：顶部NTC失效检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用  
**************************************************************************/
#ifdef _pAD_TOP
void Check_TopNtcFail()
{
 
}
#endif

/**************************************************************************
* 函数名称：Check_PanNtcFail
* 函数功能：锅底NTC失效检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用  
**************************************************************************/
void Check_PanNtcFail()
{
	static unsigned char CounterL,CounterH;

	if(B_PanFail_Dis)
	{		
		CounterL = 0;
		CounterH = 0;
			
		B_EPanFail = 0;
	}	
	else 
	{
		
	}
}

/**************************************************************************
* 函数名称：Deal_Adc
* 函数功能：ADC处理 
* 入口参数：无
* 出口参数：无 
* 备    注：主循环调用 
**************************************************************************/
void Deal_Adc()
{
	Get_AdResult();				//获取AD结果	
	Delay_CheckError();			//硬件故障检测延时

	#if _MODE_IHDEBUG_
	if(!B_ErrorCheck_Dis)		//禁止硬件故障检测
	#endif
	{
		Check_IgbtNtcOpen();	//IGBT-NTC开路检测
		Check_IgbtNtcClose();	//IGBT-NTC短路检测
		Check_IgbtNtcOver();	//IGBT-NTC超温检测
		Check_IgbtNtcHigh();	//IGBT-NTC高温检测	 

		#ifdef _pAD_TOP
		Check_TopNtcOpen();		//顶部NTC开路检测 
		Check_TopNtcClose();	//顶部NTC短路检测
		Check_TopNtcFail();		//顶部NTC失效检测	 
		#endif 
		
		Check_PanNtcOpen();		//锅底NTC开路检测 
		Check_PanNtcClose();	//锅底NTC短路检测 
		Check_PanNtcOver();		//锅底NTC超温检测
		Check_PanNtcFail();		//锅底NTC失效检测	 

		Check_VolTooHigh();		//市电电压过高检测 
		Check_VolTooLow();		//市电电压过低检测
	}
	#if _MODE_IHDEBUG_	
	else
	{
		Flag1_Error = 0;
		Flag2_Error = 0;
	}	
	#endif
	
	if(B_VolAdc_Once)	
	{					
		B_VolAdc_Once = 0;		//市电电压ADC完成一次，进入处理一次
		
		Calc_Volatage();		//计算市电电压			
		Check_VolHigh();		//市电高压区间检测
		Check_VolMid();			//市电中压区间检测
		Check_VolLow();			//市电低压区间检测	
	}	
}

 