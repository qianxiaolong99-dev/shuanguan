/**************************************************************************
**************************************************************************/
#include "Define_Global.h"
#if !_MODE_IHDEBUG_ && (_MODE_COMM_NO_ || !_MODE_COMM_HAVE_) 

/**************************************************************************
* 函数名称：DealMode_CleanRam
* 函数功能：清模式处理寄存器 
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void DealMode_CleanRam()
{
	Sec_Mode = Min_Mode = 0;	
	Cnt1_Mode = Cnt2_Mode = 0;			 
	B_TmprStop = 0; 	
}
 
/**************************************************************************
* 函数名称：WriteData8Bits
* 函数功能：1628芯片8bit送码
* 入口参数：data=被传送的数据
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void WriteData8Bits(unsigned char data)
{
	volatile unsigned char i;
	
	Pin_SDA = 0;
	Pin_STB = 0;
	
	for(i = 8; i > 0; i--)
	{
		Pin_SCK = 0;
		
		if(data & 0x1)
		{
			Pin_SDA = 1;
		}
		else 
		{
			Pin_SDA = 0;
		}
		data >>= 1;			
			
		NOP();
		NOP();
		Pin_SCK = 1;
	}
} 
 
/**************************************************************************
* 函数名称：Display
* 函数功能：显示输出
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void Display()
{		
	volatile unsigned char cnt;	
	 
	Pin_STB = 1;		
	WriteData8Bits(0x03);			//7*10	
	Pin_STB = 1;
	
	WriteData8Bits(0x40);			//地址自增、送数据	
	Pin_STB = 1;
	
	WriteData8Bits(0xC0);			//从00H地址开始送数据

	for(cnt = 0; cnt < 14; cnt++)
	{
		WriteData8Bits(ShowData[cnt]);  
	}	
	Pin_STB = 1;
	
	WriteData8Bits(0x8A);			//按照设置亮度，显示输出 
	Pin_STB = 1;
}
 
/**************************************************************************
* 函数名称：KeyScan
* 函数功能：按键扫描 
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void KeyScan()
{	
	static unsigned char CntKey_Old,Delay_KeyScan;
	
	volatile unsigned char cnt,i;		
	volatile unsigned char data[5];
	//---------------------------------------------------------------------
	Io_SDA = 0;
	Pin_STB = 1;
	WriteData8Bits(0x42);			//读扫键数据
	
	Io_SDA = 1;
	Pin_SCK = 1;
	Pin_STB = 0;
	Delay_Xus(40); 					//延时40us	

	for(cnt = 40; cnt > 0;)
	{
		Pin_SCK = 0;
		Delay_Xus(10); 				//延时10us			
		data[0] >>= 1;
			
		if(Pin_SDA)
		{
			SetBit(data[0],7);
		}
		Pin_SCK = 1;
		Delay_Xus(10); 				//延时10us
				
		cnt--;
		if(0 == (cnt & 0x07))
		{
			if(0xFF != data[0])
			{
				i = (unsigned)cnt >> 3;
				data[i] = data[0];
			}
		}
	}
	
	Pin_STB = 1;
	Io_SDA = 0;	
	//---------------------------------------------------------------------
	while(1)
	{ 
		cnt = 0;		
		cnt++;	if(C_Key_OnOff)		break;	//开关键 
		cnt++;	if(C_Key_Add)		break;	//加键 	  	 	
		cnt++;	if(C_Key_Sub)		break;	//减键 		
		cnt++;	if(C_Key_Time)		break;	//时间键 		 	   				    	  					 	 			
		cnt = 0;					break;
	}
	//---------------------------------------------------------------------
	if(cnt == CntKey_Old)
	{
		if(++Delay_KeyScan & 0x08)		//消抖
		{
			Delay_KeyScan = 0;					
			KeyValue = CntKey_Old;	 				
		}
	}
	else
	{
		CntKey_Old = cnt;		
		Delay_KeyScan = 0;
	}
}

/**************************************************************************
* 函数名称：DealKey_Com
* 函数功能：按键共用处理
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void DealKey_Com()	
{		
	Delay_Sec = Delay_5S = 0;
	Delay_NoPan = Delay_Error = 0;
	Wait_Flash = Delay_Flash = 0;
	B_DispFlash = 0;				
	
	if(!B_KeyLong)
	{
		Buzz_Short_One();			//蜂鸣器短鸣1声	
	}	
}
/**************************************************************************
* 函数名称：DealKey_OnOff
* 函数功能：开关键处理
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void DealKey_OnOff()
{  	
	DealMode_CleanRam();			//清模式处理寄存器							
	
	if(B_Adjust_SlopeCurr)
	{
		DealKey_Com();				//按键有效，进行相应处理	
		SlopeCurr_Adjust = 0x80;	//电流斜率调整值回到默认值
	}
	else if(B_OnOff)
	{	
		DealKey_Com();				//按键有效，进行相应处理		
		
		Flag1_Error = 0;			//清硬件故障标志	 	
		B_OnOff = 0;  				//关机
		DangWei = 0;	
				
		B_DingShi = 0;				//退出定时功能
		B_TimeSet = 0;				//退出时间设定
			
		Hour_Set = Min_Set = 0;	
		Delay_DS = 0;								
	}
	else 
	{	
		DealKey_Com();				//按键有效，进行相应处理
		  	
		B_OnOff = 1; 				
		DangWei = 10; 				//默认工作功率		

		B_DingShi = 0;				//退出定时功能
		B_TimeSet = 0;				//退出时间设定
			
		Min_Set = 0;				//默认定时时间为2小时
		Hour_Set = 2;
		Delay_DS = 0;	
	} 
}
/**************************************************************************
* 函数名称：DealKey_Time
* 函数功能：时间键处理
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void DealKey_Time()
{  
	if(B_KeyLong)
	{
		DealKey_Com();				//按键有效，进行相应处理
		Buzz_Short_One();			//蜂鸣器短鸣1声	
 
		B_Adjust_SlopeCurr = 1;		//电流斜率调节			
	}
	else if(B_OnOff)				//关机，不进入处理
	{	 		
		DealKey_Com();				//按键有效，进行相应处理
			
		if(!B_DingShi)
		{			
			B_DingShi = 1;			//开启定时功能								
			B_TimeSet = 1;			//进行时间设定
			
			Min_Set = Hour_Set = 0;				
			Delay_DS = 0;
		}
		else if(!B_TimeSet)
		{		
			B_DingShi = 1;			//开启定时功能								
			B_TimeSet = 1;			//进行时间设定				
			Delay_DS = 0;					
		}
		else
		{
			B_DingShi = 0;			//退出定时功能
			B_TimeSet = 0;			//退出时间设定			

			Min_Set = 0;			//默认定时时间为2小时
			Hour_Set = 2;
			Delay_DS = 0;										
		}
	}
}		
/**************************************************************************
* 函数名称：DealKey_Add
* 函数功能：加键处理
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void DealKey_Add()
{  
	if(B_Adjust_SlopeCurr)
	{
		DealKey_Com();				//按键有效，进行相应处理	
		SlopeCurr_Adjust--;			//电流斜率调整值-1（功率增加）		
	}
	else if(B_OnOff)				//关机，不进入处理
	{				
		if(B_TimeSet)
		{				
			DealKey_Com();			//按键有效，进行相应处理	
			Delay_DS = 0;
			
			if(B_KeyLong)
			{
				Min_Set += 10;		//+10				
			}
			else
			{
				Min_Set++;			//+1
			}
			
			if(B_DingShi && (Hour_Set >= 3))
			{
				Min_Set = Hour_Set = 0; 
			}
			else if(Min_Set >= 60)
			{		
				if(B_DingShi && (Hour_Set < 3))	//定时最大时间为3:00
				{
					Min_Set = 0;
					Hour_Set++;
				}								
				else
				{
					Min_Set = Hour_Set = 0;  
				}
			}	
		}		
		else if(!B_KeyLong)				//长按键无效
		{	
			if(DangWei < 20)			//最大工作档位=20
			{
				DealKey_Com();			//按键有效，进行相应处理				
				
				B_DispTurn = 0;			//显示功率			
				DangWei++;				//档位+1
									
				DealMode_CleanRam();	//清模式处理寄存器	
			}
		}	
	}
}
/**************************************************************************
* 函数名称：DealKey_Sub
* 函数功能：减键处理
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void DealKey_Sub()
{  
	if(B_Adjust_SlopeCurr)
	{
		DealKey_Com();				//按键有效，进行相应处理	
		SlopeCurr_Adjust++;			//电流斜率调整值+1（功率减小）	
	}
	else if(B_OnOff)				//关机，不进入处理
	{						
		if(B_TimeSet)
		{	
			DealKey_Com();			//按键有效，进行相应处理							
			Delay_DS = 0;				

			if(B_KeyLong)
			{
				Min_Set -= 10;		//-10				
			}
			else
			{
				Min_Set--;			//-1			
			}
							
			if(Min_Set >= 60) 
			{				
				if(Hour_Set)		//最小设定定时时间为0:00
				{
					Min_Set -= 196;	
					Hour_Set--;										
				}
				else
				{
					Min_Set = 0;
					Hour_Set = 3;	//定时最大时间3:00				
				} 
			}				 			
		}						
		else if(!B_KeyLong)			//长按键无效
		{	 		
			if(DangWei > 1)
			{ 
				DealKey_Com();			//按键有效，进行相应处理					
				
				B_DispTurn = 0;			//显示功率				
				DangWei--;				//档位-1
				
				DealMode_CleanRam();	//清模式处理寄存器	
			}
		}
	}
}
/**************************************************************************
* 函数名称：Deal_Key 
* 函数功能：按键处理 
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void Deal_Key()
{
	static unsigned int KeyValue_Old;
	static unsigned int Del_KeyLong; 			
	
 	volatile unsigned char i;
	//---------------------------------------------------------------------	
	if((!B_Dly_PowerOn && (C_KeyValue_Time != KeyValue)) ||    
       ((Flag1_Error || Flag2_Error) && (C_KeyValue_OnOff != KeyValue)))	
	{				//上电延时并且非时间键、通信故障、硬件故障并且非开关键 
		i = 0;  	//按键无效	
	}	
	else
	{
		i = KeyValue;
	}	
	//---------------------------------------------------------------------					
	if(i)
	{		
		if(B_TimeSet && ((C_KeyValue_Add == i) || (C_KeyValue_Sub == i)))	 
		{			
			Wait_Flash = Delay_Flash = 0;
			B_DispFlash = 0;						
		}		

		if(i != KeyValue_Old)
		{
			B_KeyLong = 0;	
			
			if(C_KeyValue_Time == i)
			{
				Del_KeyLong = 950/C_TimeMS_Main;	//950ms
			}
			else
			{
				Del_KeyLong = 300/C_TimeMS_Main;	//300ms
			}		
			
			switch(i)
			{		
				case 1:	DealKey_OnOff();	break;	//开关键
				case 2:	DealKey_Add();		break;	//加键
				case 3:	DealKey_Sub();		break;	//减键					 
				case 4:	DealKey_Time();		break;	//时间键						 		
				default: 					break;				
			}	

			KeyValue_Old = i;					
		}
		else if(Del_KeyLong)
		{		
			Del_KeyLong--;
		}		
		else
		{					
			if(C_KeyValue_Time == i)  
			{				
				if(!B_KeyLong && !B_Dly_PowerOn)
				{
					B_KeyLong = 1;
					DealKey_Time();		//时间键长按 	
				}
			}		
			else if((C_KeyValue_Add == i) || (C_KeyValue_Sub == i))  
			{									
				Del_KeyLong = 100/C_TimeMS_Main;	//100ms	
				B_KeyLong = 1;	
												
				if(C_KeyValue_Add == i)	
				{
					DealKey_Add();		//加键长按
				}
				else
				{
					DealKey_Sub();		//减键长按	
				} 							 			
			}
		}																				
	}
	else 
	{			
		KeyValue_Old = 0;			 				
	}
}	

/**************************************************************************
* 函数名称：Dec_to_Hex
* 函数功能：256以内HEX转BCD
* 入口参数：data=被转换的数据
* 出口参数：i=转换结果 
* 备    注：内部调用
**************************************************************************/
unsigned int Dec_to_Hex(unsigned char data)
{
	volatile unsigned int i = 0;
	
	while(data > 99)
	{
		data -= 100;
		i += 0x100;
	}
	while(data > 9)
	{
		data -= 10;
		i += 0x10;
	}
	i += data;
	return i; 
} 
/**************************************************************************
* 函数名称：SetDispPower
* 函数功能：功率显示
* 入口参数：i=功率档位
* 出口参数：无 
* 备    注：内部调用
**************************************************************************/
void SetDispPower(unsigned char i)
{
	volatile unsigned char x;	
	
	x = (unsigned)Tab_DispPower[i] & 0xF0;
	if(x)	
	{	
		x = ((unsigned)Tab_DispPower[i] >> 4) & 0x0F;
		Smg_Data1 = Tab_Smg[x];		//功率显示		
	}		
	x = (unsigned)Tab_DispPower[i] & 0x0F;
	Smg_Data2 = Tab_Smg[x];		
		
	Smg_Data3 = SMG_0; 
	Smg_Data4 = SMG_0;			
}
/**************************************************************************
* 函数名称：SetDispTime
* 函数功能：时间显示
* 入口参数：i=时，j=分
* 出口参数：无 
* 备    注：内部调用
**************************************************************************/
void SetDispTime(unsigned char i,unsigned char j)
{
	volatile unsigned char x;
	
	if(i >= 10)	
	{
		x = (Dec_to_Hex(i) >> 4) & 0x0F;
		Smg_Data1 = Tab_Smg[x];		//时显示
	}
	x = Dec_to_Hex(i) & 0x0F;
	Smg_Data2 = Tab_Smg[x];			
	
	x = (Dec_to_Hex(j) >> 4) & 0x0F;
	Smg_Data3 = Tab_Smg[x];			//分显示
	x = Dec_to_Hex(j) & 0x0F;	
	Smg_Data4 = Tab_Smg[x];	 	
}
/**************************************************************************
* 函数名称：Set_DispData
* 函数功能：显示数据设置 
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void Set_DispData()
{
	volatile unsigned char i,j;	
	
	for(i = 0; i < sizeof(ShowData); i++)
	{
		ShowData[i] = 0;	//清显示寄存器
	}		
	/**********************************************************************
	测试显示
    **********************************************************************/	
	#ifdef _TEST_DISP_	
	static unsigned char TestRam[2];	
	static unsigned int  Delay_TestDisp;	
	//---------------------------------------------------------------------	
	if(++Delay_TestDisp & 0x100)
	{
		TestRam[0] = C_Data1_TestDisp >> 8; 
		TestRam[1] = C_Data1_TestDisp; 
	}
	else
	{
		TestRam[0] = C_Data2_TestDisp >> 8; 
		TestRam[1] = C_Data2_TestDisp; 				
	}
	//---------------------------------------------------------------------	
	i = (TestRam[0] >> 4) & 0x0F;
	j = TestRam[0] & 0x0F; 	
		
	Smg_Data1 = Tab_Smg[i];		//高2位
	Smg_Data2 = Tab_Smg[j];	
	
	i = (TestRam[1] >> 4) & 0x0F;
	j = TestRam[1] & 0x0F; 
	
	Smg_Data3 = Tab_Smg[i];		//低2位
	Smg_Data4 = Tab_Smg[j];		
	//---------------------------------------------------------------------	
	if(B_PPGDP_Dis)				On_Led_M1;	//M1指示灯	
	if(Status_PPGDP & 0x02)		On_Led_M2;	//M2指示灯		
	if(B_Pan_Iron)				On_Led_M3;	//M3指示灯		
	if(B_Protect_BackPress)		On_Led_M4;	//M4指示灯				
	if(B_Zero_No)				On_Led_M5;	//M5指示灯
	if(B_Zero_60Hz)				On_Led_M6;	//M6指示灯		
	if(B_Pan_No)				On_Led_M7;	//M7指示灯	
	
	return;			
	#endif	
    /**********************************************************************
	电流斜率调节显示
    **********************************************************************/	
	if(B_Adjust_SlopeCurr)		 
	{		
		i = ((unsigned)PowerWork >> 4) & 0x0F;
		j = (unsigned)PowerWork & 0x0F; 
		
		Smg_Data1 = Tab_Smg[i];		//高2位	 
		Smg_Data2 = Tab_Smg[j];		

		i = ((unsigned)SlopeCurr_Adjust >> 4) & 0x0F;
		j = (unsigned)SlopeCurr_Adjust & 0x0F; 
			
		Smg_Data3 = Tab_Smg[i];		//低2位	 
		Smg_Data4 = Tab_Smg[j];
		
		Led_Data1  = 0xFF; 	
		Led_Data2  = 0xFF; 			
		Led_Data3  = 0xFF; 			
		return;	
	}
    /**********************************************************************
	上电显示
    **********************************************************************/	
	if(!B_Dly_PowerOn)				//上电延时，显示全亮 
    {		
		Smg_Data1 = 0xFF;			
		Smg_Data2 = 0xFF;		
		Smg_Data3 = 0xFF;
		Smg_Data4 = 0xFF;
				
		Led_Data1  = 0xFF; 	
		Led_Data2  = 0xFF; 			
		Led_Data3  = 0xFF;	
		return;	 			
	}
	/**********************************************************************
	正常显示
    **********************************************************************/	
	if(Flag1_Error || Flag2_Error)	//故障显示 
	{
        if(!B_DispFlash)	
        {
			Smg_Data2 = SMG_E;	
	 							
				 if(B_EPanOpen)	  Smg_Data3 = SMG_2;	//锅底NTC开路故障 
			else if(B_EPanClose)  Smg_Data3 = SMG_2;	//锅底NTC短路故障 	
			else if(B_EIgbtOpen)  Smg_Data3 = SMG_1;	//IGBT-NTC开路故障 
			else if(B_EIgbtClose) Smg_Data3 = SMG_1;	//IGBT-NTC短路故障 					
			else if(B_EVolHigh)	  Smg_Data3 = SMG_3;	//市电高压故障 			
			else if(B_EVolLow)	  Smg_Data3 = SMG_4;	//市电低压故障 
			else if(B_EPanOver)	  Smg_Data3 = SMG_5;	//锅底NTC超温故障 
			else if(B_EIgbtOver)  Smg_Data3 = SMG_6;	//IGBT-NTC超温故障 	
			
			else if(B_Comm_Error) Smg_Data3 = SMG_C;	//通信故障 					
		}
	}
	else if(B_OnOff)				//开机显示	
	{		
		if(B_Pan_No)				//无锅显示
		{
			if(!B_DispFlash)	
			{	
				Smg_Data2 = SMG_E;			
				Smg_Data3 = SMG_0;	//无锅显示E0
			}	
		}
		else if(B_TimeSet || (B_DingShi && B_DispTurn))	//定时显示
		{ 
			if(!B_TimeSet || !B_DispFlash)
			{
				SetDispTime(Hour_Set,Min_Set);	//显示时间
			}
			if(B_TimeSet || !B_DispFlash)
			{			
				On_Smg_MH;			//数码管冒号
			}																			
		}
		else  
		{		    
			SetDispPower(DangWei); 
		}
	}
	else 
	{
		Smg_Data1 = SMG_GG;			
		Smg_Data2 = SMG_GG;		
		Smg_Data3 = SMG_GG;
		Smg_Data4 = SMG_GG;				
	}		
	//---------------------------------------------------------------------
	if(B_PPGDP_Dis)				On_Led_M1;	//M1指示灯	
	if(Status_PPGDP & 0x02)		On_Led_M2;	//M2指示灯		
	if(B_Pan_Iron)				On_Led_M3;	//M3指示灯		
	if(B_Protect_BackPress)		On_Led_M4;	//M4指示灯				
	if(B_Zero_No)				On_Led_M5;	//M5指示灯
	if(B_Zero_60Hz)				On_Led_M6;	//M6指示灯		
	if(B_Pan_No)				On_Led_M7;	//M7指示灯
}
 
/**************************************************************************
* 函数名称：DealMode_HandCom
* 函数功能：手动模式处理
* 入口参数：dang=工作档位，tmproff=高温停止温度AD值，tmpron=高温恢复温度AD值
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
void DealMode_HandCom(unsigned char dang,unsigned char tmproff,unsigned char tmpron)	
{	
	volatile unsigned int data;	
	//---------------------------------------------------------------------	
	DangOut = dang;					//工作功率

	if(dang >= 10)	
	{								//>=10档 
		Num_LxLowOn = 0;
		Num_LxLowOff = 0;	
	}			
	else  							 
	{								//<10档
		Num_LxLowOn = Tab_LxLowNum[dang];		//连续低功率开个数
		Num_LxLowOff = Tab_LxLowNum[dang] >> 8;	//连续低功率关个数				
	}		
	//--------------------------------------------------------------------- 
	if(AD_TmprPan <= tmpron)
	{
		Cnt2_Mode = 0;	
		
		if(++Cnt1_Mode & 0x04)		//消抖  
		{		
			Cnt1_Mode = 0;				
			B_TmprStop = 0;			//锅底高温恢复 
		}		
	}
	else if(AD_TmprPan >= tmproff)
	{
		Cnt1_Mode = 0;	
		
		if(++Cnt2_Mode & 0x04)		//消抖  
		{		
			Cnt2_Mode = 0;				
			B_TmprStop = 1;			//锅底高温 
		}
	}	
	else
	{
		Cnt1_Mode = Cnt2_Mode = 0;					
	}		
}
/**************************************************************************
* 函数名称：DealMode_HandTmpr
* 函数功能：手动温度模式处理
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
/*void DealMode_HandTmpr()	
{	
	DealMode_HandCom(DangWei,Tab_ModeTmpr[DangWei]>>8,Tab_ModeTmpr[DangWei]);			
}*/
/**************************************************************************
* 函数名称：DealMode_HandPower
* 函数功能：手动功率模式处理
* 入口参数：无
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
/*void DealMode_HandPower()	
{	
	DealMode_HandCom(DangWei,C_TmprPan_120C,C_TmprPan_110C);	
}*/
/**************************************************************************
* 函数名称：DealMode_Water
* 函数功能：烧水模式处理
* 入口参数：无 
* 出口参数：无  
* 备    注：内部调用
**************************************************************************/
/*void DealMode_Water()
{
	volatile unsigned char dang;	
		
	if(++Sec_Mode >= 60000/C_TimeMS_Main)	//1min
	{
		Sec_Mode = 0;
		Min_Mode++;								 
	}		
		
	if(Min_Mode >= 20)				//20min
	{ 
		Min_Mode = 0;
		DealKey_OnOff();			//时间到，自动关机		
	}		
	else 
	{
		if(Min_Mode >= 15)			//15min
		{ 
			dang = 8;				//第8档功率 				
		}
		else
		{
			dang = 20;				//第20档功率 	 			
		}
 
		DealMode_HandCom(dang,C_TmprPan_120C,C_TmprPan_110C);	
	}			
}*/
/**************************************************************************
* 函数名称：Deal_Mode
* 函数功能：模式处理 
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用
**************************************************************************/
void Deal_Mode()
{		
	if(!B_Heat_En)	
	{	
		DangOut = 0; 
		DealMode_CleanRam();		//清模式处理寄存器		
	}	
	else		 								  
	{    
		DealMode_HandCom(DangWei,C_TmprPan_180C,C_TmprPan_170C); 
	}		
}
 
/**************************************************************************
* 函数名称：Deal_Time
* 函数功能：时间处理 
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用
**************************************************************************/
void Deal_Time()
{		
	volatile unsigned char i;
	//---------------------------------------------------------------------
	if(++Delay_Flash >= 500/C_TimeMS_Main)	//0.5s
	{
		Delay_Flash = 0;
		
		if(Wait_Flash)	
		{	
			Wait_Flash--;
		}
		else
		{
			B_DispFlash = ~B_DispFlash;		//显示闪烁 			
		}		
	}	
	//---------------------------------------------------------------------
	if(++Delay_Sec >= 1000/C_TimeMS_Main)	//1s
	{
		Delay_Sec = 0;	
		B_Dly_PowerOn = 1;  				//上电延时完成					 						
				
		if(++Delay_5S >= 5)					//5s
		{ 
 			Delay_5S = 0;		
				
			B_Adjust_SlopeCurr = 0;			//退出电流斜率调节	
			B_DispTurn = ~B_DispTurn;		//显示交替
	 
			if(B_DingShi && B_TimeSet)
			{	
				B_TimeSet = 0;				//退出时间设定
				Delay_DS = 0;
				
				if(Min_Set || Hour_Set) 		
				{				
					B_DispTurn = 1;			//显示时间	
				}	
				else
				{
					B_DingShi = 0;			//退出定时功能

					Min_Set = 0;			//默认定时时间为2小时
					Hour_Set = 2;										
				}				
			}		
		}		
 	
		if(B_OnOff && B_Pan_No)			 
		{				
			if(++Delay_NoPan >= 60)
			{ 	
				Delay_NoPan = 0;	
				DealKey_OnOff();			//无锅，自动关机
			}
			
			if(Delay_NoPan & 0x01)			//每2秒
			{	
				Buzz_Short_One();			//蜂鸣器短鸣1声				
			}			
		}
		else
		{
			Delay_NoPan = 0;	
		}
 	
		if(B_OnOff && Flag1_Error)		 
		{							
			if(++Delay_Error >= 60)
			{ 	
				Delay_Error = 0;	
				DealKey_OnOff();			//硬件故障，自动关机
			}
			
			if(Delay_Error & 0x01)			//每2秒
			{	
				Buzz_Short_One();			//蜂鸣器短鸣1声				
			}				
		}
		else
		{
			Delay_Error = 0;	
		}
	}			
	//---------------------------------------------------------------------
	if(Flag1_Error || Flag2_Error || !B_OnOff || B_TimeSet)
	{
		Time_HMI = 0;
		Delay_DS = 0;		//故障、关机、时间设置中
	}
	else	
	{
		if(!B_Zero_60Hz || B_Zero_No)
		{
			i = 100;		//10ms*100=1s			
		}
		else
		{
			i = 120;		//8.333ms*120=1s
		}
		
		if(Time_HMI >= i)		 
		{
			Time_HMI = 0;
			
			if(++Delay_DS >= 60)	//1min
			{
				Delay_DS = 0;

				if(Min_Set)
				{
					Min_Set--;
				}
				else if(Hour_Set)
				{
					Min_Set = 59;
					Hour_Set--;
				}
			}
		}
			
		if((0 == Min_Set) && (0 == Hour_Set))	 
		{			
			if(B_OnOff)	
			{
				DealKey_OnOff();	//定时时间到，自动关机
			}
		}									
	}	
}	
 
/**************************************************************************
* 函数名称：Set_Work
* 函数功能：工作设置
* 入口参数：无
* 出口参数：无
* 备    注：内部调用
**************************************************************************/
void Set_Work()
{ 		
	if(B_Adjust_SlopeCurr)
	{
		PowerWork = Tab_DangOut[10];		//工作功率值			
		
		B_Heat_En = 1;  					//加热使能		
		B_Fan_En = 1;  						//风扇使能	
		
		B_HeatStop_NoCheck = 0;
		B_HeatStop_CheckPan = 0;
	}
	else
	{	
		PowerWork = Tab_DangOut[DangOut];	//工作功率值		

		B_HeatStop_CheckPan = 0;	
			
		if(B_OnOff && !Flag1_Error && !Flag2_Error) 
		{
			B_Heat_En = 1;  				//加热使能		
			B_Fan_En = 1;  					//风扇使能

			if(B_TmprStop)
			{  
				B_HeatStop_NoCheck = 1;		//不加热不检锅
			}
			else
			{
				B_HeatStop_NoCheck = 0;
			}				
		}
		else
		{
			B_Heat_En = 0;  				//加热关闭		
			B_Fan_En = 0; 					//风扇关闭
			
			B_HeatStop_NoCheck = 0;		
		}
	}		
}

/**************************************************************************
* 函数名称：Init_HMI
* 函数功能：初始化人机界面 
* 入口参数：无
* 出口参数：无  
* 备    注：初始化调用 
**************************************************************************/
void Init_HMI() 
{
	volatile unsigned char i;
		
	for(i = 0; i < sizeof(ShowData); i++)
	{
		ShowData[i] = 0xFF;		//显示全亮
	}		
	Display();					//显示输出	
} 

/**************************************************************************
* 函数名称：Deal_HMI
* 函数功能：人机界面处理
* 入口参数：无
* 出口参数：无  
* 备    注：主循环调用
**************************************************************************/
void Deal_HMI()
{
	KeyScan();					//按键扫描	
	Display();					//显示输出	
	Set_DispData();				//显示数据设置		
	Deal_Key();  				//按键处理		
				
    Deal_Time();				//时间处理
	Deal_Mode();				//模式处理	
	Set_Work();					//工作设置 	
}
 
/**************************************************************************
**************************************************************************/ 
#endif

