/**************************************************************************
**************************************************************************/
#ifndef _REL_IHDEBUG_
#define _REL_IHDEBUG_
#if		_MODE_IHDEBUG_
  
/**************************************************************************
--------------------------------- 常量定义 --------------------------------
**************************************************************************/
#define C_IhDebug_Baudrate		9600	//波特率（最大支持波特率为9600）
//-------------------------------------------------------------------------	
#define C_IhDebug_SendTypeID	0		//发送数据类型ID
#define C_IhDebug_SendCntSum	15		//一共需要发送结构体个数
#define C_IhDebug_SendCntOnce	2 		//一次循环最多发送结构体个数
//*************************************************************************  
const unsigned char REL_IhDebug_Baudrate = 1000000 / C_IhDebug_Baudrate - 1; 
//-------------------------------------------------------------------------	
const unsigned char REL_IhDebug_SendTypeID = C_IhDebug_SendTypeID;
const unsigned char REL_IhDebug_SendCntOnce = C_IhDebug_SendCntOnce;

#if (C_IhDebug_SendCntSum % C_IhDebug_SendCntOnce)
const unsigned char REL_IhDebug_PackIDX_Max = C_IhDebug_SendCntSum / C_IhDebug_SendCntOnce;
const unsigned char REL_IhDebug_PackSend_CntFinal = C_IhDebug_SendCntSum % C_IhDebug_SendCntOnce;
#else
const unsigned char REL_IhDebug_PackIDX_Max = (C_IhDebug_SendCntSum - 1) / C_IhDebug_SendCntOnce;
const unsigned char REL_IhDebug_PackSend_CntFinal = C_IhDebug_SendCntOnce;
#endif 

/**************************************************************************
--------------------------------- 变量定义 --------------------------------
**************************************************************************/
volatile unsigned int  PPGTMR_TestPPG; 
volatile unsigned int  SlopeCurr_Test;
//-------------------------------------------------------------------------	
volatile bit B_Test_PGA;
volatile bit B_Test_CM_RA0;
volatile bit B_Test_CM_RB7; 

volatile bit B_Test_PanCheck;
volatile bit B_Test_PPGOut; 
volatile bit B_Test_SlopeCurr;
volatile bit B_ErrorCheck_Dis;
volatile bit B_LosePanCheck_Dis; 
 
/**************************************************************************
--------------------------------- 函数声明 --------------------------------
**************************************************************************/
extern void REL_IhDebug_Main();
extern void REL_IhDebug_Int();	
 
/**************************************************************************
**************************************************************************/
#endif
#endif
 
