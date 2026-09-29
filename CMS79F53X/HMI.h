/**************************************************************************
**************************************************************************/
#if 	!_MODE_IHDEBUG_ && (_MODE_COMM_NO_ || !_MODE_COMM_HAVE_) 
#ifndef	_HMI_H_
#define	_HMI_H_

/**************************************************************************
--------------------------------- 测试设置 --------------------------------
**************************************************************************/
//设置说明：有定义 = 测试显示，无定义 = 正常显示  
//#define  	_TEST_DISP_			 		
//-------------------------------------------------------------------------
//参数名称：测试显示数据1 
//参数说明：数据为16bit
//#define  	C_Data1_TestDisp	SlopeCurr
#define  	C_Data1_TestDisp	(AD_TmprIgbt << 8) + AD_TmprPan
//#define 	C_Data1_TestDisp	AD_Current_Fst 
//#define 	C_Data1_TestDisp	AD_Current 
//#define 	C_Data1_TestDisp	AD_Volatage
//#define 	C_Data1_TestDisp	(AD_Volatage << 8) + Vol.flo.integer
//#define 	C_Data1_TestDisp	(Time_Syn << 8) + Count_Syn
//#define 	C_Data1_TestDisp	Buf_PPGTMR
//#define 	C_Data1_TestDisp	Status_PPGStop//PowerWork
//#define 	C_Data1_TestDisp	RealPower//(AD_Current << 8) + PPGTMRL
//#define 	C_Data1_TestDisp	(PPGTMRH << 8) + PPGTMRL
//#define 	C_Data1_TestDisp	(Flag2_Error << 8) + Flag1_Error
//#define 	C_Data1_TestDisp	(AD_VRef<< 8) + AD_Vdd 
//#define 	C_Data1_TestDisp	(Num_LxLowOn << 8) + Num_LxLowOff
//#define  	C_Data1_TestDisp	(AD_Current << 8) + AD_Volatage
//#define  	C_Data1_TestDisp	(AD_VRef << 8) + AD_Volatage
//#define  	C_Data1_TestDisp	(Status_PPGStop << 8) + AD_TmprIgbt

//参数名称：测试显示数据2  
//参数说明：数据为16bit
#define		C_Data2_TestDisp	C_Data1_TestDisp//(AD_Current << 8) + AD_Volatage// 				
  
/**************************************************************************
--------------------------------- 按键设置 --------------------------------
**************************************************************************/
#define  	C_Key_OnOff			TestOne(data[4],0)		//开关键 
#define  	C_Key_Add			TestOne(data[4],3)		//加键	
#define  	C_Key_Sub			TestOne(data[3],3)		//减键
#define  	C_Key_Time			TestOne(data[3],0)		//时间键
//-------------------------------------------------------------------------
#define  	C_KeyValue_OnOff	1 						//开关键 
#define  	C_KeyValue_Add	 	2 						//加键	  
#define  	C_KeyValue_Sub		3 						//减键 
#define  	C_KeyValue_Time		4 						//时间键    

/**************************************************************************
--------------------------------- 显示设置 --------------------------------
**************************************************************************/
#define 	Smg_Data1			ShowData[0]				//数码管1
#define 	Smg_Data2			ShowData[2]				//数码管2
#define 	Smg_Data3			ShowData[4]				//数码管3
#define 	Smg_Data4			ShowData[6]				//数码管4
 
#define 	Led_Data1 			ShowData[8]				//LED1
#define 	Led_Data2 			ShowData[10]			//LED2
#define 	Led_Data3 			ShowData[12]			//LED3
//-------------------------------------------------------------------------
#define  	On_Smg_XSD     		SetBit(Smg_Data1,4)  	//数码管小数点 
#define  	On_Smg_MH      		SetBit(Smg_Data2,4)  	//数码管冒号  
 
#define  	On_Led_M1   		SetBit(Led_Data1,2)  	//M1指示灯 
#define  	On_Led_M2			SetBit(Led_Data1,3)  	//M2指示灯  
#define  	On_Led_M3			SetBit(Led_Data1,4)  	//M3指示灯
#define  	On_Led_M4  			SetBit(Led_Data1,5)  	//M4指示灯
#define  	On_Led_M5   		SetBit(Led_Data1,6)  	//M5指示灯
#define  	On_Led_M6 			SetBit(Led_Data1,1)  	//M6指示灯
#define  	On_Led_M7 			SetBit(Led_Data1,0)  	//M7指示灯 

/**************************************************************************
* 数码管显示编码：bgc_deaf
**************************************************************************/  
#define  	Aa					0x02     	
#define  	Bb					0x80
#define  	Cc					0x20 
#define  	Dd					0x08
#define  	Ee					0x04   
#define  	Ff					0x01
#define  	Gg					0x40  
//-------------------------------------------------------------------------
#define		SMG_0		  		Aa|Bb|Cc|Dd|Ee|Ff		
#define		SMG_1		  		Bb|Cc		
#define		SMG_2		  		Aa|Bb|Dd|Ee|Gg		
#define		SMG_3		  		Aa|Bb|Cc|Dd|Gg
#define		SMG_4		  		Bb|Cc|Ff|Gg		
#define		SMG_5		  		Aa|Cc|Dd|Ff|Gg			
#define		SMG_6		 	 	Aa|Cc|Dd|Ee|Ff|Gg 	
#define		SMG_7		  		Aa|Bb|Cc		
#define		SMG_8		 	 	Aa|Bb|Cc|Dd|Ee|Ff|Gg		
#define		SMG_9		 	 	Aa|Bb|Cc|Dd|Ff|Gg		
#define		SMG_A		  		Aa|Bb|Cc|Ee|Ff|Gg	
#define		SMG_B		  		Cc|Dd|Ee|Ff|Gg		
#define		SMG_C		  		Aa|Dd|Ee|Ff		
#define		SMG_D		  		Bb|Cc|Dd|Ee|Gg		
#define		SMG_E		  		Aa|Dd|Ee|Ff|Gg		
#define		SMG_F		  		Aa|Ee|Ff|Gg    
                                    
#define		SMG_GG		  		Gg
#define		SMG_L				Dd|Ee|Ff
#define		SMG_O_MIN			Cc|Dd|Ee|Gg
#define		SMG_C_MIN			Dd|Ee|Gg
#define		SMG_K_MIN			Dd|Ee|Ff|Gg	

/**************************************************************************
* 锅底温度对应AD值 
**************************************************************************/ 
#define		C_TmprPan_300C		245  
#define		C_TmprPan_270C		225 
#define		C_TmprPan_260C		220 
#define		C_TmprPan_240C		219 
#define		C_TmprPan_230C		217
#define		C_TmprPan_220C		216 
#define		C_TmprPan_210C		210  
#define		C_TmprPan_200C		208 
#define		C_TmprPan_190C		204
#define		C_TmprPan_180C		195 
#define		C_TmprPan_170C		192
#define		C_TmprPan_160C		188 
#define		C_TmprPan_150C		179
#define		C_TmprPan_140C		170
#define		C_TmprPan_135C		163
#define		C_TmprPan_130C		155
#define		C_TmprPan_120C		142
#define		C_TmprPan_110C		125
#define		C_TmprPan_100C		111
#define		C_TmprPan_90C		94
#define		C_TmprPan_80C		74	 
#define		C_TmprPan_70C		62	 
#define		C_TmprPan_60C		55 	
#define		C_TmprPan_56C		53 			 
#define		C_TmprPan_50C		37
#define		C_TmprPan_40C		16

/**************************************************************************
* 连续低功率时间 
**************************************************************************/ 
#define		C_Num_LxLowOn_1D	1
#define		C_Num_LxLowOff_1D	9 

#define		C_Num_LxLowOn_2D	2
#define		C_Num_LxLowOff_2D	8

#define		C_Num_LxLowOn_3D	3
#define		C_Num_LxLowOff_3D	7

#define		C_Num_LxLowOn_4D	4
#define		C_Num_LxLowOff_4D	6

#define		C_Num_LxLowOn_5D	5	
#define		C_Num_LxLowOff_5D	5	

#define		C_Num_LxLowOn_6D	6	
#define		C_Num_LxLowOff_6D	4	

#define		C_Num_LxLowOn_7D	7	
#define		C_Num_LxLowOff_7D	3	

#define		C_Num_LxLowOn_8D	8	
#define		C_Num_LxLowOff_8D	2	

#define		C_Num_LxLowOn_9D	9	
#define		C_Num_LxLowOff_9D	1	

#define		C_Num_LxLowOn_10D	0	
#define		C_Num_LxLowOff_10D	0	
//-------------------------------------------------------------------------
#define		C_Num_LxLow_1D		(C_Num_LxLowOff_1D<<8) + C_Num_LxLowOn_1D
#define		C_Num_LxLow_2D		(C_Num_LxLowOff_2D<<8) + C_Num_LxLowOn_2D
#define		C_Num_LxLow_3D		(C_Num_LxLowOff_3D<<8) + C_Num_LxLowOn_3D
#define		C_Num_LxLow_4D		(C_Num_LxLowOff_4D<<8) + C_Num_LxLowOn_4D
#define		C_Num_LxLow_5D		(C_Num_LxLowOff_5D<<8) + C_Num_LxLowOn_5D
#define		C_Num_LxLow_6D		(C_Num_LxLowOff_6D<<8) + C_Num_LxLowOn_6D
#define		C_Num_LxLow_7D		(C_Num_LxLowOff_7D<<8) + C_Num_LxLowOn_7D
#define		C_Num_LxLow_8D		(C_Num_LxLowOff_8D<<8) + C_Num_LxLowOn_8D
#define		C_Num_LxLow_9D		(C_Num_LxLowOff_9D<<8) + C_Num_LxLowOn_9D
#define		C_Num_LxLow_10D		(C_Num_LxLowOff_10D<<8)+ C_Num_LxLowOn_10D 
 
/**************************************************************************
* 数码管编码表格
**************************************************************************/
const unsigned char Tab_Smg[] =
{
	SMG_0,		// 0
	SMG_1,		// 1
	SMG_2,		// 2
	SMG_3,		// 3
	SMG_4,		// 4
	SMG_5,		// 5
	SMG_6,		// 6
	SMG_7,		// 7
	SMG_8,		// 8
	SMG_9,		// 9
	SMG_A,		// A
	SMG_B,		// B
	SMG_C,		// C
	SMG_D,		// D
	SMG_E,		// E
	SMG_F,		// F
};

/**************************************************************************
* 功率显示表格
**************************************************************************/
const unsigned char Tab_DispPower[] =
{
	0, 			 
	0x01,		// 1档--100W
	0x02,		// 2档--200W
	0x03,		// 3档--300W
	0x04,		// 4档--400W
	0x05,		// 5档--500W
	0x06,		// 6档--600W
	0x07,		// 7档--700W
	0x08,		// 8档--800W
	0x09,		// 9档--900W
	0x10,		//10档--1000W	
	0x11,		//11档--1100W
	0x12,		//12档--1200W
	0x13,		//13档--1300W
	0x14,		//14档--1400W
	0x15,		//15档--1500W
	0x16,		//16档--1600W
	0x17,		//17档--1700W
	0x18,		//18档--1800W
	0x19,		//19档--1900W
	0x20,		//20档--2000W		 	 	
};
 
/**************************************************************************
* 连续低功率个数表格
**************************************************************************/
const unsigned int Tab_LxLowNum[] = 
{
	0,
	C_Num_LxLow_1D,		// 1档
	C_Num_LxLow_2D,		// 2档
	C_Num_LxLow_3D,		// 3档
	C_Num_LxLow_4D,		// 4档
	C_Num_LxLow_5D,		// 5档
	C_Num_LxLow_6D,		// 6档
	C_Num_LxLow_7D,		// 7档
	C_Num_LxLow_8D,		// 8档
	C_Num_LxLow_9D,		// 9档
	C_Num_LxLow_10D,	//10档
};

/**************************************************************************
* 温度控制表格 
**************************************************************************/
/*const unsigned int Tab_ModeTmpr[] =
{
	0, 			 
	(unsigned int)(65 <<8) + 60,	// 1档
	(unsigned int)(75 <<8) + 70,	// 2档	
	(unsigned int)(85 <<8) + 80,	// 3档
	(unsigned int)(95 <<8) + 90,	// 4档	
	(unsigned int)(105 <<8) + 100,	// 5档
	(unsigned int)(115 <<8) + 110,	// 6档	
	(unsigned int)(125 <<8) + 120,	// 7档
	(unsigned int)(135 <<8) + 130,	// 8档	
	(unsigned int)(145 <<8) + 140,	// 9档
	(unsigned int)(155 <<8) + 150,	//10档	
	(unsigned int)(165 <<8) + 160,	//11档
	(unsigned int)(175 <<8) + 170,	//12档	
	(unsigned int)(185 <<8) + 180,	//13档
	(unsigned int)(195 <<8) + 190,	//14档	
	(unsigned int)(205 <<8) + 200,	//15档
	(unsigned int)(210 <<8) + 205,	//16档	
	(unsigned int)(215 <<8) + 210,	//17档
	(unsigned int)(220 <<8) + 215,	//18档	
	(unsigned int)(225 <<8) + 220,	//19档	
	(unsigned int)(230 <<8) + 225,	//20档	 		
};*/

/**************************************************************************
* 功率输出表格
**************************************************************************/
const unsigned char Tab_DangOut[] = 
{
	0,
	1000/C_Mult_Power,	// 1档--100W 
	1000/C_Mult_Power,	// 2档--200W  
	1000/C_Mult_Power,	// 3档--300W  
	1000/C_Mult_Power,	// 4档--400W  
	1000/C_Mult_Power,	// 5档--500W  
	1000/C_Mult_Power,	// 6档--600W  
	1000/C_Mult_Power,	// 7档--700W  
	1000/C_Mult_Power,	// 8档--800W  
	1000/C_Mult_Power,	// 9档--900W  
	1000/C_Mult_Power,	//10档--1000W  	
	1100/C_Mult_Power,	//11档--1100W 
	1200/C_Mult_Power,	//12档--1200W  
	1300/C_Mult_Power,	//13档--1300W  
	1400/C_Mult_Power,	//14档--1400W  
	1500/C_Mult_Power,	//15档--1500W  
	1600/C_Mult_Power,	//16档--1600W  
	1700/C_Mult_Power,	//17档--1700W  
	1800/C_Mult_Power,	//18档--1800W  
	1900/C_Mult_Power,	//19档--1900W  
	2000/C_Mult_Power,	//20档--2000W 	
};
 
/**************************************************************************
--------------------------------- 函数声明 --------------------------------
**************************************************************************/ 
void Init_HMI();
void Deal_HMI();	
 
/**************************************************************************
--------------------------------- 变量定义 --------------------------------
**************************************************************************/
volatile unsigned char ShowData[14];
volatile unsigned char KeyValue;

volatile unsigned char DangWei,DangOut;

volatile unsigned char Delay_Flash,Wait_Flash;
volatile unsigned char Delay_Sec,Delay_5S;
volatile unsigned char Delay_NoPan,Delay_Error;

volatile unsigned char Delay_DS; 
volatile unsigned char Min_Set,Hour_Set;

volatile unsigned int  Sec_Mode;
volatile unsigned char Min_Mode;
volatile unsigned char Cnt1_Mode,Cnt2_Mode;

volatile bit B_OnOff;
volatile bit B_KeyLong;	
volatile bit B_DingShi,B_TimeSet;
volatile bit B_Dly_PowerOn,B_DispFlash,B_DispTurn;
volatile bit B_TmprStop;

/**************************************************************************
**************************************************************************/ 
#endif
#endif

