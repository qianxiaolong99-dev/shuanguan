/**************************************************************************
**************************************************************************/
#ifndef	_DEFINE_TYPE_H_
#define	_DEFINE_TYPE_H_
 
/**************************************************************************
**************************************************************************/
#define uchar 	unsigned char
#define uint  	unsigned int
#define ulong 	unsigned long
//-------------------------------------------------------------------------
#define ExteralPin(Ram,Bit,Name)	volatile bit Name @((unsigned)&Ram*8)+Bit
//-------------------------------------------------------------------------
#define BOOL  				unsigned char		//布尔型

#define	TRUE				1					//真
#define	FALSE				0					//假
//-------------------------------------------------------------------------
#define	SetBit(x,y)			x |= (1<<y)			//将寄存器x的第y位置1
#define ClrBit(x,y)			x &= ~(1<<y)		//将寄存器x的第y位清0

#define TestOne(Ram,Bit)	(Ram & (1<<Bit))	//测试寄存器Ram的Bit位是否为1
#define TestZero(Ram,Bit)	!(Ram & (1<<Bit))	//测试寄存器Ram的Bit位是否为0

/**************************************************************************
**************************************************************************/
typedef union
{
    unsigned char  all;
    struct
    {
       unsigned b0:1;
       unsigned b1:1;
       unsigned b2:1;
       unsigned b3:1;
       unsigned b4:1;
       unsigned b5:1;
	   unsigned b6:1;
       unsigned b7:1;
    }one;
}bits;
//-------------------------------------------------------------------------
typedef struct
{
	unsigned char radix;		//低八位为小数
	unsigned char integer;		//高八位为整数
}TypeFloat;
//-------------------------------------------------------------------------
typedef union
{
	unsigned int all;
	TypeFloat flo;
}VolElecType;
//-------------------------------------------------------------------------
typedef union
{
	unsigned long all;
	unsigned char one[4];
}CommType;

/**************************************************************************
**************************************************************************/
#endif

