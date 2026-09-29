/**************************************************************************
**************************************************************************/
#include "Define_Global.h"
#if !_MODE_IHDEBUG_ && !_MODE_COMM_NO_

/**************************************************************************
* 函数名称：Communicate
* 函数功能：芯片通讯
* 入口参数：无
* 出口参数：无 
* 备    注：中断调用
**************************************************************************/
void Communicate()
{
	//收码相关寄存器定义	
	static unsigned int  CntReceNo;
	static unsigned char CountRece;	
	static unsigned char DataRece[4]; 
 
	//发码相关寄存器定义
	static unsigned char TimeSend,CountSend;
	static unsigned char DataSend[4];	
 
	/**********************************************************************
	收码	
	**********************************************************************/ 
	if(!B_Comm_No)									//无通讯，不收码
	{	
		if(RCIF)
		{
			if(FERR)
			{
				RCREG;								//帧错误
			}
			else
			{
				CntReceNo = 0;								
				DataRece[CountRece] = RCREG; 		//将接收缓冲区内容读出
		 
				if(++CountRece >= sizeof(DataRece))	//接收完一帧数据，处理数据
				{
					CountRece = 0;
					B_Deal_DataRece = 1;			//收码数据处理		
									
					Buf_DataRece[0] = DataRece[3];
					Buf_DataRece[1] = DataRece[2];
					Buf_DataRece[2] = DataRece[1];
					Buf_DataRece[3] = DataRece[0];
				}	
				
				if(OERR)							//如果有溢出错误
				{
					CREN = 0;						//清零CREN位可将OERR位清零
					CREN = 1;						//再次将CREN置一，以允许继续接收
				}
			}
		}

		if(++CntReceNo >= 5000/C_TimeUS_Int)		//5ms
		{
			CountRece = 0;		
		}
		else if(CntReceNo >= 15000/C_TimeUS_Int)	//15ms
		{
			CntReceNo = 0;
			
			CREN = 0;
			SPBRG = 103;							//设置波特率为9600bps，误差0%	
			STOPBIT = 0;             				//停止位为1bit
			SYNC = 0;								//0为异步模式，1为同步模式
			SCKP = 0;
			//SPEN = 1;								//允许串口操作
			RX9EN = 0;								//0为8位接收，1为9位接收
			CREN = 1;								//0为禁止接收，1为使能接收
			
			/*if(!SPEN)
			{
				SPEN = 1;
			}*/				
		}
	}
	
	/**********************************************************************
	发码	
	**********************************************************************/ 
	if(B_Comm_Have)									//有通讯，才发码
	{	
		if(TXIF)
		{
			if(CountSend)
			{				
				TXREG = DataSend[(unsigned)CountSend - 1];
				CountSend--;
			}
		}
		
		if(CountSend)
		{
			TimeSend = 0;
		}
		else if(++TimeSend >= 10000/C_TimeUS_Int)	//10ms
		{
			TimeSend = 0;
			CountSend = sizeof(DataSend);			//发码数据字节数 
					
			DataSend[0] = Buf_DataSend[0];
			DataSend[1] = Buf_DataSend[1];
			DataSend[2] = Buf_DataSend[2];
			DataSend[3] = Buf_DataSend[3];
 
			B_Refrsh_DataSend = 1;					//发码数据刷新
		}
	}
}

/**************************************************************************
* 函数名称：Deal_CommData
* 函数功能：通讯数据处理
* 入口参数：无
* 出口参数：无 
* 备    注：主循环调用
**************************************************************************/
void Deal_CommData()
{			
	static unsigned int  Time_NoComm;
	static unsigned char Num_CommSend;	
	static bit B_Init_Uart;	
				
	volatile unsigned char i,j;

	/**********************************************************************
	UART初始化
	**********************************************************************/		
	if(!B_Init_Uart)
	{
		B_Init_Uart = 1;		//UART初始化完成
		
		SPBRG = 103;			//设置波特率为9600bps，误差0%	
		STOPBIT =0;             //停止位为1bit
		SYNC = 0;				//0为异步模式，1为同步模式
		SCKP = 0;
		SPEN = 1;				//允许串口操作
		RX9EN = 0;				//0为8位接收，1为9位接收
		TX9EN = 0;				//0为8位发送，1为9位发送
		CREN = 1;				//0为禁止接收，1为使能接收
		TXEN = 1;				//0为禁止发送，1为使能发送
	}

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
			B_Comm_Have = 1;						//有通讯					
			B_Comm_Error = 0;						//清通讯故障标志					
			Time_NoComm = 0;						//清无通讯计时				
 				 
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
	通讯状态确认	
	**********************************************************************/		
	if(B_Comm_Have)
	{												//已确认有通讯
		if(++Time_NoComm > 2000/C_TimeMS_Main)		//2s
		{
			Time_NoComm = 0;
			
			B_Comm_Error = 1;						//通讯故障	
			Buzz_Short_One();						//蜂鸣器短鸣1声		
		}
	}
	else if(!B_Comm_No)	
	{												//未确认有无通讯
		if(++Time_NoComm > 250/C_TimeMS_Main)		//250ms
		{
			Time_NoComm = 0;
			B_Comm_No = 1;							//无通讯 
			
			TXSTA = 0;								//关闭UART模块
			RCSTA = 0;					
		}	
	}	
	else
	{												//已确认无通讯
		Time_NoComm = 0;			
	}
}

/**************************************************************************
**************************************************************************/ 
#endif
		
		