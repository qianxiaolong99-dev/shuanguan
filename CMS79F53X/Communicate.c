/**************************************************************************
**************************************************************************/
#include "Define_Global.h"
#if !_MODE_IHDEBUG_ && !_MODE_COMM_NO_

/**************************************************************************
* 函数名称：Communicate
* 函数功能：芯片通信
* 入口参数：无
* 出口参数：无 
* 备    注：中断调用
**************************************************************************/
void Communicate()
{
	//收码相关寄存器定义
	static unsigned char TimeRece_High,TimeRece_Low,CountRece;
	static CommType DataRece;
	static bit B_Rece_Old;
	
	//发码相关寄存器定义
	static unsigned char TimeSend,CountSend;
	static CommType DataSend;
	static bit B_Send_Start,B_Send_Bak; 		
	
    volatile unsigned char i;
						 
	/**********************************************************************
	收码	
	**********************************************************************/ 
	if(!B_Comm_No)									//无通信，不收码
	{			
		if(!Pin_CommIn && !Pin_CommIn && !Pin_CommIn && !Pin_CommIn && !Pin_CommIn)
		{
			TimeRece_Low++;							//低电平计时
			
			if(!B_Rece_Old)
			{
				TimeRece_High = 0;
			}
			else
			{										//下降沿接收数据
				B_Rece_Old = 0;						//收码电平旧值（低）
				
				if(1 == TimeRece_Low)
				{									//第一次下降沿
					CountRece = 32;
				}
				else
				{									//非第一次下降沿
					DataRece.all <<= 1;	
										
					if(TimeRece_Low >= TimeRece_High) 
					{								//低电平>高电平为1，否则为0
						DataRece.one[0] |= 0x01;
					}

					TimeRece_High = 0;
					TimeRece_Low = 1;
					
					if(0 == --CountRece)
					{
						B_Deal_DataRece = 1;		//收码数据处理
						
						Buf_DataRece[3] = DataRece.one[3];	  	 
						Buf_DataRece[2] = DataRece.one[2];			 
						Buf_DataRece[1] = DataRece.one[1];		
						Buf_DataRece[0] = DataRece.one[0];						
					}
				}
			}
		}
		else if(Pin_CommIn && Pin_CommIn && Pin_CommIn && Pin_CommIn && Pin_CommIn)
		{
			B_Rece_Old = 1;							//收码电平旧值（高）
			
			if(TimeRece_High >= 5000/C_TimeUS_Int)	//5ms
			{										//长时间高电平，重新收码
				TimeRece_Low = 0;					 
				CountRece = 32;
			}
			else
			{
				TimeRece_High++;					//高电平计时
			}
		}
	}
	
	/**********************************************************************
	发码	
	**********************************************************************/ 
	if(B_Comm_Have)									//有通信，才发码
	{
		if(B_Send_Start)
		{
			if(0 == --TimeSend)
			{										//已发完一位码
				Pin_CommOut = 0;					//输出低电平
			
				if(0 == CountSend)
				{								
 					B_Send_Start = 0;				//发码完成	
					TimeSend = 0;										
				}
				else
				{									//发送下一位数据
					CountSend--;
					TimeSend = 2000/C_TimeUS_Int;	//2ms
					
					if(DataSend.one[3] & 0x80)
					{
						B_Send_Bak = 1;				//发1
					}
					else
					{
						B_Send_Bak = 0;				//发0
					}	
								 		
					DataSend.all <<= 1;		
				}				
			}	
			else
			{										//还没有发完一位码
				if(B_Send_Bak)
				{
					i = 500/C_TimeUS_Int;			//0.5ms
				}
				else
				{
					i = 1500/C_TimeUS_Int;			//1.5ms
				}		
				
				if(TimeSend > i)
				{
					Pin_CommOut = 0;				//输出低电平						
				}
				else
				{
					Pin_CommOut = 1;				//输出高电平			
				}
			}
		}
		else
		{
			if(TimeSend >= 10000/C_TimeUS_Int)		//10ms
			{
				B_Send_Start = 1;					//发码开始
												
				TimeSend = 1;	
				CountSend = 32;						//发码数据位数：32bit
		
				DataSend.one[3] = Buf_DataSend[3];	  	 
				DataSend.one[2] = Buf_DataSend[2];			 
				DataSend.one[1] = Buf_DataSend[1];	
				DataSend.one[0] = Buf_DataSend[0];	
		
				B_Refrsh_DataSend = 1;				//发码数据刷新 							
			}
			else
			{
				TimeSend++;
			}
			
			if(TimeSend >= 500/C_TimeUS_Int)		//0.5ms		
			{			
				Pin_CommOut = 1;					//输出高电平			
			}		
			else		
			{
				Pin_CommOut = 0;					//输出低电平	
			}
		}
	}
}
 
/**************************************************************************
* 函数名称：Deal_CommData
* 函数功能：通信数据处理
* 入口参数：无
* 出口参数：无 
* 备    注：主循环调用
**************************************************************************/
void Deal_CommData()
{			
	static unsigned int  Time_NoComm;
	static unsigned char Num_CommSend;	
		
	volatile unsigned char i,j;
 
	/**********************************************************************
	发码数据处理	
	**********************************************************************/				
	if(B_Comm_Have && B_Refrsh_DataSend)
	{												//发码数据刷新		
		i = 0;			
		if(B_Pan_Error)				SetBit(i,0); 	//检锅故障（未接线盘）	
		if(B_Pan_No)				SetBit(i,1); 	//无锅
		if(B_Pan_Have)				SetBit(i,2); 	//有锅	
		if(B_Pan_Steel)				SetBit(i,3); 	//钢锅			
		if(B_Pan_Iron)				SetBit(i,4); 	//铁锅
		if(B_Zero_No)				SetBit(i,5); 	//无过零
		if(B_Zero_60Hz)				SetBit(i,6); 	//过零频率 = 60Hz
		if(B_Protect_BackPress)		SetBit(i,7); 	//过压保护	
						
		Status1_Work = i;							//刷新工作状态1
		//-----------------------------------------------------------------	
		i = 0;					
		if(B_Protect_Stop)			SetBit(i,0); 	//保护停止加热
		if(B_PPGStop_CheckPan)		SetBit(i,1); 	//加热暂停但检锅	
		if(B_PPGStop_NoCheck)		SetBit(i,2); 	//加热暂停不检锅
		if(Status_PPGDP & 0x02)		SetBit(i,3); 	//抖频开启
		if(Status_LxLow & 0x02)		SetBit(i,4); 	//连续低功率开启		

		Status2_Work = i;							//刷新工作状态2					
		//-----------------------------------------------------------------	
		switch(Num_CommSend)
		{	
			case 0:
			{			
				Buf_DataSend[3] = 0xA0;				//命令字			
				Buf_DataSend[2] = Status1_Work;		//工作状态1				
				Buf_DataSend[1] = Status2_Work;		//工作状态2	
			}
			break;	
			case 1:
			{			
				Buf_DataSend[3] = 0xA1;				//命令字			
				Buf_DataSend[2] = Flag1_Error;		//硬件故障标志1				
				Buf_DataSend[1] = Flag2_Error;		//硬件故障标志2	
			}
			break;				
			case 2:
			{
				Buf_DataSend[3] = 0xA2;				//命令字
				Buf_DataSend[2] = Status_PPGStop;	//保护状态		
			
				i = (AD_Volatage >> 4) & 0xF0;		//市电电压AD（高4位）
				i |= (AD_Current >> 8) & 0x0F;		//工作电流AD（高4位）		
				Buf_DataSend[1] = i;		 
			} 	
			break;			
			case 3:
			{
				Buf_DataSend[3] = 0xA3;				//命令字
				Buf_DataSend[2] = AD_Volatage;		//市电电压AD（低8位）
				Buf_DataSend[1] = AD_Current;		//工作电流AD（低8位）
			}
			break;	
			case 4:
			{
				Buf_DataSend[3] = 0xA4;				//命令字		
				Buf_DataSend[2] = AD_TmprPan;		//锅底温度AD
				Buf_DataSend[1] = AD_TmprIgbt;		//IGBT温度AD
			}
			break;		
			case 5:
			{								
				Buf_DataSend[3] = 0xA5;				//命令字
				Buf_DataSend[2] = AD_TmprTop;		//顶部温度AD 					
				Buf_DataSend[1] = AD_VRef;			//内部基准电压（1.2V）AD									
			}
			break;			
			case 6:
			{								
				Buf_DataSend[3] = 0xA6;				//命令字	
				Buf_DataSend[2] = Vol.flo.integer;	//市电电压 - 80V
				Buf_DataSend[1] = Count_Syn;		//检锅振荡个数								
			}
			break;										
			case 7:
			{			
				Buf_DataSend[3] = 0xA7;				//命令字	
				Buf_DataSend[2] = Time_Syn >> 8;	//检锅振荡脉宽（高8位） 
				Buf_DataSend[1] = Time_Syn;			//检锅振荡脉宽（低8位）					
			}
			break;		
			case 8:
			{			
				Buf_DataSend[3] = 0xA8;				//命令字	
				Buf_DataSend[2] = PowerReal >> 8;	//实际输出功率（高8位） 
				Buf_DataSend[1] = PowerReal;		//实际输出功率（低8位）				
			}
			break;					
			case 9:
			{			
				Buf_DataSend[3] = 0xA9;				//命令字
				Buf_DataSend[2] = Buf_PPGTMR >> 8;	//当前PPG导通值（高8位）
				Buf_DataSend[1] = Buf_PPGTMR;		//当前PPG导通值（低8位）				
			}
			break;		
			case 10:
			{			
				Buf_DataSend[3] = 0xAA;					//命令字
				Buf_DataSend[2] = Count_BackPress >> 8;	//1级过压保护计数（高8位）
				Buf_DataSend[1] = Count_BackPress;		//1级过压保护计数（低8位）				
			}
			break;		
			case 11:
			{			
				Buf_DataSend[3] = 0xAB;						//命令字
				Buf_DataSend[2] = Count_IgbtProtect >> 8;	//IGBT硬件保护计数（高8位）
				Buf_DataSend[1] = Count_IgbtProtect;		//IGBT硬件保护计数（低8位）				
			}
			break;				
			case 12:
			{			
				Buf_DataSend[3] = 0xAC;				//命令字
				Buf_DataSend[2] = Elec.flo.integer;	//电量高位
				Buf_DataSend[1] = Elec.flo.radix;	//电量低位	
			}
			break;								
			default:break;	
		}
		
		i = (unsigned)Buf_DataSend[1] + Buf_DataSend[2] + Buf_DataSend[3];
		Buf_DataSend[0] = (unsigned)~i;				//校验和	
		//-----------------------------------------------------------------		
		B_Refrsh_DataSend = 0;						//发码数据刷新完成	
				
		if(++Num_CommSend >= 13)					//发码切换
		{							
			Num_CommSend = 0;	
		}							 
	}
	
	/**********************************************************************
	收码数据处理	
	**********************************************************************/
	if(!B_Comm_No && B_Deal_DataRece)					
	{	
		B_Deal_DataRece = 0;						//收码数据处理完成 			
								
		i = (unsigned)Buf_DataRece[1] + Buf_DataRece[2] + Buf_DataRece[3];
		i = (unsigned)~i;
		j = (unsigned)Buf_DataRece[3] & 0xF0;
 
		if((i == Buf_DataRece[0]) && ((0xB0 == j) || (0xD0 == j)))	//数据校验
		{				
			B_Comm_Have = 1;						//有通信					
			B_Comm_Error = 0;						//清通信故障标志					
			Time_NoComm = 0;						//清无通信计时				
 				 
			if(0xB0 == Buf_DataRece[3])		
			{			
				Flag1_Work = Buf_DataRece[2];		//工作标志1
				Flag2_Work = Buf_DataRece[1];		//工作标志2					
			}
			else if(0xB1 == Buf_DataRece[3])		
			{			
				PowerWork = Buf_DataRece[2];		//加热功率值
									
			}			
			else if(0xB2 == Buf_DataRece[3])	
			{
				Num_LxLowOn = Buf_DataRece[2];		//连续低功率开个数
				Num_LxLowOff = Buf_DataRece[1];		//连续低功率关个数
			}
			else if(0xD0 == Buf_DataRece[3])	
			{
				i = (unsigned)~Buf_DataRece[1];
				if(Buf_DataRece[2] == i)
				{
					SlopeCurr_Adjust = Buf_DataRece[2];	//电流斜率调整值
				}				
			}				
		}
		//-----------------------------------------------------------------			
		if(TestOne(Flag1_Work,0))
		{
			B_Heat_En = 1;  				//加热使能		
			B_Fan_En = 1;  					//风扇使能
		}
		else
		{
			B_Heat_En = 0;  				//加热关闭		
			B_Fan_En = 0; 					//风扇关闭			
		}	

		if(TestOne(Flag1_Work,1))
		{		
			B_Fan_En = 1;  					//风扇使能
		}
		else
		{	
			B_Fan_En = 0; 					//风扇关闭			
		}	
 
		if(TestOne(Flag1_Work,2))
		{		
			B_Buzz_En = 1;  				//蜂鸣器使能
		}
		else
		{	
			B_Buzz_En = 0; 					//蜂鸣器关闭			
		}	
 
		if(TestOne(Flag1_Work,3))
		{
			if(!B_Buzz_Out)
			{
				Buzz_Short_One();			//蜂鸣器短鸣1声（200ms）
			}
		}
		
		if(TestOne(Flag1_Work,4))
		{
			if(!B_Buzz_Out)
			{
				Buzz_Long_One();			//蜂鸣器长鸣1声（400ms）
			}
		}	
	
		if(TestOne(Flag1_Work,5))  
		{
			B_HeatStop_CheckPan = 1;  		//不加热但检锅
		}
		else
		{
			B_HeatStop_CheckPan = 0;
		}
		
		if(TestOne(Flag1_Work,6))  
		{
			B_HeatStop_NoCheck = 1;  		//不加热不检锅
		}
		else
		{
			B_HeatStop_NoCheck = 0;
		}
 
		if(TestOne(Flag1_Work,7))  
		{
			B_PPGDP_Dis = 1;  				//PPG抖频禁止
		}
		else
		{
			B_PPGDP_Dis = C_PPGDP_ForceOff; // 总开关关闭时不允许通信恢复抖频
		}	
		//-----------------------------------------------------------------			
		if(TestOne(Flag2_Work,0))
		{
			B_PanOver_Dis = 1;  			//锅底NTC超温判断禁止		
		}
		else
		{
			B_PanOver_Dis = 0;  		 				
		}
		
		if(TestOne(Flag2_Work,1))
		{
			B_PanFail_Dis = 1;  			//锅底NTC失效判断禁止		
		}
		else
		{
			B_PanFail_Dis = 0;  		 				
		}
		
		if(TestOne(Flag2_Work,7))  
		{
			B_PPGDP_Can = !C_PPGDP_ForceOff; // 总开关关闭时屏蔽强制使能
		}
		else
		{
			B_PPGDP_Can = 0;  				
		}							
	}
		
	/**********************************************************************
	通信状态确认	
	**********************************************************************/		
	if(B_Comm_Have)
	{												//已确认有通信
		if(++Time_NoComm > 2000/C_TimeMS_Main)		//2s
		{
			Time_NoComm = 0;
			
			B_Comm_Error = 1;						//通信故障	
			Buzz_Short_One();						//蜂鸣器短鸣1声		
		}
	}
	else if(!B_Comm_No)	
	{												//未确认有无通信
		if(++Time_NoComm > 250/C_TimeMS_Main)		//250ms
		{
			Time_NoComm = 0;
			B_Comm_No = 1;							//无通信 	
		}	
	}	
	else
	{												//已确认无通信
		Time_NoComm = 0;			
	}
}

/**************************************************************************
**************************************************************************/ 
#endif

			