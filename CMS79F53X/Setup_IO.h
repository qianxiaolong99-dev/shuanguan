/**************************************************************************
**************************************************************************/
#ifndef _SETUP_IO_H_
#define _SETUP_IO_H_
 
/**************************************************************************
-------------------------------- IO口配置 ---------------------------------			   
**************************************************************************/
//配置内容：IO脚位排序
//配置说明：有定义 = 该型号对应IO脚位排序 
//#define  	_IO_CMS79F533					//CMS79F533脚位
#define  	_IO_CMS79F536					//CMS79F536脚位
//=========================================================================
//配置内容：功能对应IO脚位 
//配置说明：无相应功能，可直接屏蔽或者删除 
//-------------------------------------------------------------------------	
//#define  	_pIGBT_SUP			_Pin13		//IGBT辅助驱动口
#define  	_pBUZZ				_Pin6		//蜂鸣器控制口 
#define  	_pFAN 				_Pin5		//风扇控制口

//#define  	_pAD_TOP			_Pin		//顶部测温ADC口
#define  	_pAD_PAN			_Pin8		//锅底测温ADC口
#define  	_pAD_IGBT			_Pin18//_Pin2		//IGBT测温ADC口
#define  	_pAD_VOL			_Pin1		//电压ADC口

#define  	_pCON1				_Pin6		//CON1口
#define  	_pCON2				_Pin10		//CON2口
#define  	_pCON3				_Pin9		//CON3口 
//-------------------------------------------------------------------------	
#define  	_pCOMM_IN			_pCON2		//通信输入口（有通信）
#define  	_pCOMM_OUT			_pCON3		//通信输出口（有通信）
 
//#define  	_pSDA				_pCON1		//驱动SDA口（无通信）
//#define  	_pSCK				_pCON2		//驱动SCK口（无通信）
//#define  	_pSTB				_pCON3		//驱动STB口（无通信） 
//=========================================================================
//配置内容：上电初始化IO电平
//配置说明：0 = 低电平，1 = 高电平
#define 	C_Init_PORTA		0B00000000
#define 	C_Init_PORTB		0B00000000 
#define 	C_Init_PORTC		0B00000000

/**************************************************************************
-------------------------------- AD通道配置 -------------------------------			   
**************************************************************************/
//配置内容：工作电流ADC通道
//配置说明：通道27 = 有RC滤波、通道28 = 无RC滤波
#define     C_AdcChannel_Curr	28			 

/**************************************************************************
* 芯片测试脚位  	   
**************************************************************************/
#define  	_Pin_CMPOUT1		0xA0		//比较器测试输出口1
#define  	_Pin_CMPOUT2		0xB7		//比较器测试输出口2	
#define  	_Pin_PGAOUT			0xC0		//PGA测试输出口   
/**************************************************************************
* CMS79F533脚位（16pin） 			   
**************************************************************************/
#ifdef		_IO_CMS79F533
#define  	_Pin1				0xA1		//Pin1 
//#define  	_Pin2				PPG			//Pin2 
#define  	_Pin3				0xB7		//Pin3
#define  	_Pin4				0xB6		//Pin4 
#define  	_Pin5				0xB4		//Pin5 
#define  	_Pin6				0xB3		//Pin6 
#define  	_Pin7				0xB2		//Pin7 
#define  	_Pin8				0xB1		//Pin8 
//#define  	_Pin9				VDD			//Pin9
//#define  	_Pin10				GND			//Pin10 
#define  	_Pin11				0xA7		//Pin11 
#define  	_Pin12				0xA5		//Pin12 
#define  	_Pin13				0xC0		//Pin13 
#define  	_Pin14				0xA4		//Pin14 
#define  	_Pin15				0xA3		//Pin15 
#define  	_Pin16				0xA2		//Pin16 
#endif
/**************************************************************************
* CMS79F536脚位（20pin）
**************************************************************************/
#ifdef		_IO_CMS79F536
#define  	_Pin1				0xA1		//Pin1 
#define  	_Pin2				0xA0		//Pin2
//#define  	_Pin3				PPG			//Pin3 
#define  	_Pin4				0xB7		//Pin4
#define  	_Pin5				0xB6		//Pin5 
#define  	_Pin6				0xB5		//Pin6 
#define  	_Pin7				0xB4		//Pin7 
#define  	_Pin8				0xB3		//Pin8 
#define  	_Pin9				0xB2		//Pin9 
#define  	_Pin10				0xB1		//Pin10 
//#define  	_Pin11				VDD			//Pin11
//#define  	_Pin12				GND			//Pin12 
#define  	_Pin13				0xC2		//Pin13 
#define  	_Pin14				0xC1		//Pin14
#define  	_Pin15				0xA7		//Pin15 
#define  	_Pin16				0xA5		//Pin16 
#define  	_Pin17				0xC0		//Pin17 
#define  	_Pin18				0xA4		//Pin18 
#define  	_Pin19				0xA3		//Pin19 
#define  	_Pin20				0xA2		//Pin20   
#endif 

/**************************************************************************
* IO口定义
**************************************************************************/
#ifndef		_pIGBT_SUP
ExteralPin(PORTC,7,Pin_Igbt_Sup); 
#elif		(_pIGBT_SUP>>4)==0xA 			
ExteralPin(PORTA,(_pIGBT_SUP&0xF),Pin_Igbt_Sup); 
#elif		(_pIGBT_SUP>>4)==0xB 			
ExteralPin(PORTB,(_pIGBT_SUP&0xF),Pin_Igbt_Sup); 
#elif		(_pIGBT_SUP>>4)==0xC 			
ExteralPin(PORTC,(_pIGBT_SUP&0xF),Pin_Igbt_Sup); 
#else
ExteralPin(PORTC,7,Pin_Igbt_Sup); 
#endif
//-------------------------------------------------------------------------
#ifndef		_pFAN
ExteralPin(PORTC,7,Pin_Fan); 
#elif		(_pFAN>>4)==0xA
ExteralPin(PORTA,(_pFAN&0xF),Pin_Fan);  
#elif		(_pFAN>>4)==0xB
ExteralPin(PORTB,(_pFAN&0xF),Pin_Fan);  
#elif		(_pFAN>>4)==0xC
ExteralPin(PORTC,(_pFAN&0xF),Pin_Fan);  
#else
ExteralPin(PORTC,7,Pin_Fan); 
#endif
//-------------------------------------------------------------------------
#ifndef		_pCOMM_IN
ExteralPin(PORTC,7,Pin_CommIn); 
#elif		(_pCOMM_IN>>4)==0xA
ExteralPin(PORTA,(_pCOMM_IN&0xF),Pin_CommIn); 
#elif		(_pCOMM_IN>>4)==0xB
ExteralPin(PORTB,(_pCOMM_IN&0xF),Pin_CommIn); 
#elif		(_pCOMM_IN>>4)==0xC
ExteralPin(PORTC,(_pCOMM_IN&0xF),Pin_CommIn); 
#else
ExteralPin(PORTC,7,Pin_CommIn); 
#endif
//-------------------------------------------------------------------------
#ifndef		_pCOMM_OUT
ExteralPin(PORTC,7,Pin_CommOut); 
#elif		(_pCOMM_OUT>>4)==0xA
ExteralPin(PORTA,(_pCOMM_OUT&0xF),Pin_CommOut); 
#elif		(_pCOMM_OUT>>4)==0xB
ExteralPin(PORTB,(_pCOMM_OUT&0xF),Pin_CommOut); 
#elif		(_pCOMM_OUT>>4)==0xC
ExteralPin(PORTC,(_pCOMM_OUT&0xF),Pin_CommOut); 
#else
ExteralPin(PORTC,7,Pin_CommOut); 
#endif	
//-------------------------------------------------------------------------
#ifndef		_pSDA
ExteralPin(PORTC,7,Pin_SDA);
ExteralPin(TRISC,7,Io_SDA); 
#elif		(_pSDA>>4)==0xA
ExteralPin(PORTA,(_pSDA&0xF),Pin_SDA);
ExteralPin(TRISA,(_pSDA&0xF),Io_SDA);
#elif		(_pSDA>>4)==0xB
ExteralPin(PORTB,(_pSDA&0xF),Pin_SDA);
ExteralPin(TRISB,(_pSDA&0xF),Io_SDA);
#elif		(_pSDA>>4)==0xC
ExteralPin(PORTC,(_pSDA&0xF),Pin_SDA);
ExteralPin(TRISC,(_pSDA&0xF),Io_SDA);
#else
ExteralPin(PORTC,7,Pin_SDA);
ExteralPin(TRISC,7,Io_SDA); 
#endif
//-------------------------------------------------------------------------
#ifndef		_pSCK
ExteralPin(PORTC,7,Pin_SCK); 
#elif		(_pSCK>>4)==0xA
ExteralPin(PORTA,(_pSCK&0xF),Pin_SCK);
#elif		(_pSCK>>4)==0xB
ExteralPin(PORTB,(_pSCK&0xF),Pin_SCK);
#elif		(_pSCK>>4)==0xC
ExteralPin(PORTC,(_pSCK&0xF),Pin_SCK);
#else
ExteralPin(PORTC,7,Pin_SCK); 
#endif
//-------------------------------------------------------------------------
#ifndef		_pSTB
ExteralPin(PORTC,7,Pin_STB); 
#elif		(_pSTB>>4)==0xA
ExteralPin(PORTA,(_pSTB&0xF),Pin_STB);
#elif		(_pSTB>>4)==0xB
ExteralPin(PORTB,(_pSTB&0xF),Pin_STB);
#elif		(_pSTB>>4)==0xC
ExteralPin(PORTC,(_pSTB&0xF),Pin_STB);
#else
ExteralPin(PORTC,7,Pin_STB); 
#endif

/**************************************************************************
* PWM通道定义
**************************************************************************/
#if			_pBUZZ==0xA0				
#define  	C_PwmChannel_Buzz	0x0A	
#elif		_pBUZZ==0xB5				
#define  	C_PwmChannel_Buzz	0x0B
#elif		_pBUZZ==0xB2				
#define  	C_PwmChannel_Buzz	0x0C
#elif		_pBUZZ==0xC2			
#define  	C_PwmChannel_Buzz	0x0D

#elif		_pBUZZ==0xB7				
#define  	C_PwmChannel_Buzz	0x1A
#elif		_pBUZZ==0xB4				
#define  	C_PwmChannel_Buzz	0x1B
#elif		_pBUZZ==0xB1				
#define  	C_PwmChannel_Buzz	0x1C
#elif		_pBUZZ==0xC1				
#define  	C_PwmChannel_Buzz	0x1D
	
#elif		_pBUZZ==0xB6				
#define  	C_PwmChannel_Buzz	0x2A		
#elif		_pBUZZ==0xB3				
#define  	C_PwmChannel_Buzz	0x2B		
#elif		_pBUZZ==0xB0				
#define  	C_PwmChannel_Buzz	0x2C		
#elif		_pBUZZ==0xC0				
#define  	C_PwmChannel_Buzz	0x2D

#else
#define  	C_PwmChannel_Buzz	0xFF	
#endif
//-------------------------------------------------------------------------
#if			!_FAN_OUT_PWM_
#define  	C_PwmChannel_Fan	0xFF

#elif		_pFAN==0xA0				
#define  	C_PwmChannel_Fan	0x0A
#elif		_pFAN==0xB5				
#define  	C_PwmChannel_Fan	0x0B
#elif		_pFAN==0xB2				
#define  	C_PwmChannel_Fan	0x0C
#elif		_pFAN==0xC2			
#define  	C_PwmChannel_Fan	0x0D
	
#elif		_pFAN==0xB7				
#define  	C_PwmChannel_Fan	0x1A
#elif		_pFAN==0xB4				
#define  	C_PwmChannel_Fan	0x1B
#elif		_pFAN==0xB1				
#define  	C_PwmChannel_Fan	0x1C
#elif		_pFAN==0xC1				
#define  	C_PwmChannel_Fan	0x1D	
	
#elif		_pFAN==0xB6				
#define  	C_PwmChannel_Fan	0x2A		
#elif		_pFAN==0xB3				
#define  	C_PwmChannel_Fan	0x2B		
#elif		_pFAN==0xB0				
#define  	C_PwmChannel_Fan	0x2C		
#elif		_pFAN==0xC0				
#define  	C_PwmChannel_Fan	0x2D

#else
#define  	C_PwmChannel_Fan	0xFF	
#endif

/**************************************************************************
* 非固定ADC通道定义
**************************************************************************/
#if			_pAD_VOL==0xA0				
#define  	C_AdcChannel_Vol	0	
#elif		_pAD_VOL==0xA1
#define  	C_AdcChannel_Vol	1	
#elif		_pAD_VOL==0xA2
#define  	C_AdcChannel_Vol	2	
#elif		_pAD_VOL==0xA3
#define  	C_AdcChannel_Vol	3	
#elif		_pAD_VOL==0xA4
#define  	C_AdcChannel_Vol	4	
#elif		_pAD_VOL==0xA5
#define  	C_AdcChannel_Vol	5
#elif		_pAD_VOL==0xA6
#define  	C_AdcChannel_Vol	6
#elif		_pAD_VOL==0xA7
#define  	C_AdcChannel_Vol	7
	
#elif		_pAD_VOL==0xB0
#define  	C_AdcChannel_Vol	8		
#elif		_pAD_VOL==0xB1
#define  	C_AdcChannel_Vol	9	
#elif		_pAD_VOL==0xB2
#define  	C_AdcChannel_Vol	10	
#elif		_pAD_VOL==0xB3
#define  	C_AdcChannel_Vol	11	
#elif		_pAD_VOL==0xB4
#define  	C_AdcChannel_Vol	12	
#elif		_pAD_VOL==0xB5
#define  	C_AdcChannel_Vol	13
#elif		_pAD_VOL==0xB6
#define  	C_AdcChannel_Vol	14
#elif		_pAD_VOL==0xB7
#define  	C_AdcChannel_Vol	15	

#elif		_pAD_VOL==0xC0
#define  	C_AdcChannel_Vol	16	
#elif		_pAD_VOL==0xC1
#define  	C_AdcChannel_Vol	17	
#elif		_pAD_VOL==0xC2
#define  	C_AdcChannel_Vol	18	

#else
#define  	C_AdcChannel_Vol	C_AdcChannel_VRef
#endif
//-------------------------------------------------------------------------
#ifndef		_pAD_IGBT
#define  	C_AdcChannel_Igbt	C_AdcChannel_VRef

#elif		_pAD_IGBT==0xA0					 
#define  	C_AdcChannel_Igbt	0	
#elif		_pAD_IGBT==0xA1
#define  	C_AdcChannel_Igbt	1	
#elif		_pAD_IGBT==0xA2
#define  	C_AdcChannel_Igbt	2	
#elif		_pAD_IGBT==0xA3
#define  	C_AdcChannel_Igbt	3	
#elif		_pAD_IGBT==0xA4
#define  	C_AdcChannel_Igbt	4	
#elif		_pAD_IGBT==0xA5
#define  	C_AdcChannel_Igbt	5
#elif		_pAD_IGBT==0xA6
#define  	C_AdcChannel_Igbt	6
#elif		_pAD_IGBT==0xA7
#define  	C_AdcChannel_Igbt	7

#elif		_pAD_IGBT==0xB0
#define  	C_AdcChannel_Igbt	8		
#elif		_pAD_IGBT==0xB1
#define  	C_AdcChannel_Igbt	9	
#elif		_pAD_IGBT==0xB2
#define  	C_AdcChannel_Igbt	10	
#elif		_pAD_IGBT==0xB3
#define  	C_AdcChannel_Igbt	11	
#elif		_pAD_IGBT==0xB4
#define  	C_AdcChannel_Igbt	12	
#elif		_pAD_IGBT==0xB5
#define  	C_AdcChannel_Igbt	13
#elif		_pAD_IGBT==0xB6
#define  	C_AdcChannel_Igbt	14
#elif		_pAD_IGBT==0xB7
#define  	C_AdcChannel_Igbt	15	

#elif		_pAD_IGBT==0xC0
#define  	C_AdcChannel_Igbt	16	
#elif		_pAD_IGBT==0xC1
#define  	C_AdcChannel_Igbt	17	
#elif		_pAD_IGBT==0xC2
#define  	C_AdcChannel_Igbt	18	

#else
#define  	C_AdcChannel_Igbt	C_AdcChannel_VRef
#endif
//-------------------------------------------------------------------------
#ifndef		_pAD_PAN
#define  	C_AdcChannel_Pan	C_AdcChannel_VRef

#elif		_pAD_PAN==0xA0					 
#define  	C_AdcChannel_Pan	0	
#elif		_pAD_PAN==0xA1
#define  	C_AdcChannel_Pan	1	
#elif		_pAD_PAN==0xA2
#define  	C_AdcChannel_Pan	2	
#elif		_pAD_PAN==0xA3
#define  	C_AdcChannel_Pan	3	
#elif		_pAD_PAN==0xA4
#define  	C_AdcChannel_Pan	4	
#elif		_pAD_PAN==0xA5
#define  	C_AdcChannel_Pan	5
#elif		_pAD_PAN==0xA6
#define  	C_AdcChannel_Pan	6
#elif		_pAD_PAN==0xA7
#define  	C_AdcChannel_Pan	7	

#elif		_pAD_PAN==0xB0
#define  	C_AdcChannel_Pan	8
#elif		_pAD_PAN==0xB1
#define  	C_AdcChannel_Pan	9	
#elif		_pAD_PAN==0xB2
#define  	C_AdcChannel_Pan	10	
#elif		_pAD_PAN==0xB3
#define  	C_AdcChannel_Pan	11	
#elif		_pAD_PAN==0xB4
#define  	C_AdcChannel_Pan	12	
#elif		_pAD_PAN==0xB5
#define  	C_AdcChannel_Pan	13
#elif		_pAD_PAN==0xB6
#define  	C_AdcChannel_Pan	14
#elif		_pAD_PAN==0xB7
#define  	C_AdcChannel_Pan	15	

#elif		_pAD_PAN==0xC0
#define  	C_AdcChannel_Pan	16	
#elif		_pAD_PAN==0xC1
#define  	C_AdcChannel_Pan	17	
#elif		_pAD_PAN==0xC2
#define  	C_AdcChannel_Pan	18

#else
#define  	C_AdcChannel_Pan	C_AdcChannel_VRef	
#endif
//-------------------------------------------------------------------------
#ifndef		_pAD_TOP
#define  	C_AdcChannel_Top	C_AdcChannel_VRef

#elif		_pAD_TOP==0xA0					 
#define  	C_AdcChannel_Top	0	
#elif		_pAD_TOP==0xA1
#define  	C_AdcChannel_Top	1	
#elif		_pAD_TOP==0xA2
#define  	C_AdcChannel_Top	2	
#elif		_pAD_TOP==0xA3
#define  	C_AdcChannel_Top	3	
#elif		_pAD_TOP==0xA4
#define  	C_AdcChannel_Top	4	
#elif		_pAD_TOP==0xA5
#define  	C_AdcChannel_Top	5
#elif		_pAD_TOP==0xA6
#define  	C_AdcChannel_Top	6
#elif		_pAD_TOP==0xA7
#define  	C_AdcChannel_Top	7	

#elif		_pAD_TOP==0xB0
#define  	C_AdcChannel_Top	8
#elif		_pAD_TOP==0xB1
#define  	C_AdcChannel_Top	9	
#elif		_pAD_TOP==0xB2
#define  	C_AdcChannel_Top	10	
#elif		_pAD_TOP==0xB3
#define  	C_AdcChannel_Top	11	
#elif		_pAD_TOP==0xB4
#define  	C_AdcChannel_Top	12	
#elif		_pAD_TOP==0xB5
#define  	C_AdcChannel_Top	13
#elif		_pAD_TOP==0xB6
#define  	C_AdcChannel_Top	14
#elif		_pAD_TOP==0xB7
#define  	C_AdcChannel_Top	15	

#elif		_pAD_TOP==0xC0
#define  	C_AdcChannel_Top	16	
#elif		_pAD_TOP==0xC1
#define  	C_AdcChannel_Top	17	
#elif		_pAD_TOP==0xC2
#define  	C_AdcChannel_Top	18	

#else
#define  	C_AdcChannel_Top	C_AdcChannel_VRef
#endif
/**************************************************************************
* 固定ADC通道定义
**************************************************************************/
#define  	C_AdcChannel_Gnd	29		//GND-ADC通道	 
#define  	C_AdcChannel_Vdd	30		//VDD-ADC通道
#define  	C_AdcChannel_VRef	31		//内部参考电压（1.2V）ADC通道	
/**************************************************************************
* AD检测设置（00xxxx01）
**************************************************************************/
#define  	C_AdcSet_Gnd		0x01 | (C_AdcChannel_Gnd<<2)	//GND 
#define  	C_AdcSet_Vdd		0x01 | (C_AdcChannel_Vdd<<2)	//VDD 
#define  	C_AdcSet_VRef		0x01 | (C_AdcChannel_VRef<<2)	//内部参考电压

#define  	C_AdcSet_Curr		0x01 | (C_AdcChannel_Curr<<2)	//工作电流 
#define  	C_AdcSet_Vol		0x01 | (C_AdcChannel_Vol<<2)	//市电电压

#define  	C_AdcSet_Igbt		0x01 | (C_AdcChannel_Igbt<<2)	//IGBT-NTC  
#define  	C_AdcSet_Pan		0x01 | (C_AdcChannel_Pan<<2)	//锅底NTC  
#define  	C_AdcSet_Top		0x01 | (C_AdcChannel_Top<<2)	//顶部NTC 
 
/**************************************************************************
--------------------------------- 函数声明 --------------------------------
**************************************************************************/ 
unsigned char SetWPUX_CommHave(unsigned char x);   
unsigned char SetWPUX_CommNo(unsigned char x);  
unsigned char SetTRISX_CommHave(unsigned char x); 
unsigned char SetTRISX_CommNo(unsigned char x); 
 
/**************************************************************************
**************************************************************************/
#endif

