/**************************************************************************
**************************************************************************/
#include "Define_Global.h"
 
/**************************************************************************
* IO口上拉设置
**************************************************************************/
#if !_MODE_COMM_NO_
unsigned char SetWPUX_CommHave(unsigned char x)   
{ 			 
	volatile unsigned char i = 0;
	
	#ifdef _pCOMM_IN 
	if(x == (_pCOMM_IN >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pCOMM_IN);	
	}
	#endif
	return i;  			
}
#endif
/*************************************************************************/
#if _MODE_COMM_NO_ || !_MODE_COMM_HAVE_
unsigned char SetWPUX_CommNo(unsigned char x)  
{ 			 
	volatile unsigned char i = 0;

	return i; 		 	 			
}
#endif

/**************************************************************************
* IO口方向设置
**************************************************************************/
#if !_MODE_COMM_NO_
unsigned char SetTRISX_CommHave(unsigned char x) 
{  	
	volatile unsigned char i = 0;
	//---------------------------------------------------------------------
	#ifdef _pBUZZ 		
	if(x == (_pBUZZ >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pBUZZ);	
	}
	#endif
 
	#ifdef _pFAN
	if(x == (_pFAN >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pFAN);	
	}
	#endif
 
	#ifdef _pIGBT_SUP
	if(x == (_pIGBT_SUP >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pIGBT_SUP);	
	}
	#endif
	#ifdef _pCOMM_OUT
	if(x == (_pCOMM_OUT >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pCOMM_OUT);	
	} 
	#endif 
	//---------------------------------------------------------------------
	#ifdef _TEST_CMPOUT_
	#if _TEST_CMPOUT_ == _Pin_CMPOUT1
	if(x == (_Pin_CMPOUT1 >> 4))
	{
		i |= 1 << (unsigned)(0xF & _Pin_CMPOUT1);	
	}
	#elif _TEST_CMPOUT_ == _Pin_CMPOUT2
	if(x == (_Pin_CMPOUT2 >> 4))
	{
		i |= 1 << (unsigned)(0xF & _Pin_CMPOUT2);	
	}
	#endif	
	#elif _MODE_IHDEBUG_
	if(B_Test_CM_RA0)
	{
		if(x == (_Pin_CMPOUT1 >> 4))
		{
			i |= 1 << (unsigned)(0xF & _Pin_CMPOUT1);	
		}
	}
	else if(B_Test_CM_RB7)
	{
		if(x == (_Pin_CMPOUT2 >> 4))
		{
			i |= 1 << (unsigned)(0xF & _Pin_CMPOUT2);	
		}
	}
	#endif	
	
	#ifdef _TEST_PGAOUT_	
	if(x == (_Pin_PGAOUT >> 4))
	{
		i &= ~(1 << (unsigned)(0xF & _Pin_PGAOUT));	
	}	
	#elif _MODE_IHDEBUG_
	if(B_Test_PGA)
	{
		if(x == (_Pin_PGAOUT >> 4))
		{
			i &= ~(1 << (unsigned)(0xF & _Pin_PGAOUT));	
		}
	}		
	#endif	
	//---------------------------------------------------------------------
	return (unsigned)~i; 	 
}
#endif
/*************************************************************************/
#if _MODE_COMM_NO_ || !_MODE_COMM_HAVE_
unsigned char SetTRISX_CommNo(unsigned char x) 
{    
	volatile unsigned char i = 0;
	//---------------------------------------------------------------------
	#ifdef _pBUZZ 	
	if(x == (_pBUZZ >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pBUZZ);	
	}
	#endif
 	
	#ifdef _pFAN	
	if(x == (_pFAN >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pFAN);	
	}
	#endif
 
	#ifdef _pIGBT_SUP	
	if(x == (_pIGBT_SUP >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pIGBT_SUP);	
	}
	#endif
	#ifdef _pSDA		
	if(x == (_pSDA >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pSDA);	
	}
	#endif
	#ifdef _pSCK	
	if(x == (_pSCK >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pSCK);	
	}	
	#endif
	#ifdef _pSTB		
	if(x == (_pSTB >> 4))
	{
		i |= 1 << (unsigned)(0xF & _pSTB);	
	}	
	#endif
	//---------------------------------------------------------------------	
	#ifdef _TEST_CMPOUT_
	#if _TEST_CMPOUT_ == _Pin_CMPOUT1
	if(x == (_Pin_CMPOUT1 >> 4))
	{
		i |= 1 << (unsigned)(0xF & _Pin_CMPOUT1);	
	}
	#elif _TEST_CMPOUT_ == _Pin_CMPOUT2
	if(x == (_Pin_CMPOUT2 >> 4))
	{
		i |= 1 << (unsigned)(0xF & _Pin_CMPOUT2);	
	}
	#endif	
	#elif _MODE_IHDEBUG_
	if(B_Test_CM_RA0)
	{
		if(x == (_Pin_CMPOUT1 >> 4))
		{
			i |= 1 << (unsigned)(0xF & _Pin_CMPOUT1);	
		}
	}
	else if(B_Test_CM_RB7)
	{
		if(x == (_Pin_CMPOUT2 >> 4))
		{
			i |= 1 << (unsigned)(0xF & _Pin_CMPOUT2);	
		}
	}
	#endif	
	
	#ifdef _TEST_PGAOUT_	
	if(x == (_Pin_PGAOUT >> 4))
	{
		i &= ~(1 << (unsigned)(0xF & _Pin_PGAOUT));	
	}
	#elif _MODE_IHDEBUG_
	if(B_Test_PGA)
	{
		if(x == (_Pin_PGAOUT >> 4))
		{
			i &= ~(1 << (unsigned)(0xF & _Pin_PGAOUT));	
		}
	}				
	#endif	
	//---------------------------------------------------------------------
	return (unsigned)~i; 	 		  
}
#endif

