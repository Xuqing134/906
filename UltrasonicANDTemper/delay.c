
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


/*************	����˵��	**************

���ļ�ΪSTC15xxxϵ�е���ʱ����,�û��������Բ��޸��������.


******************************************/

#include	"delay.h"

//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================

/*
void  delay_ms(unsigned int ms)
{
	unsigned int i,j,k; 
	for(i=ms;i>0;i--)
		for(k=71;k>0;k--)
			for(j=31;j>0;j--);
}


void delay_150us()		//@30.000MHz
{
	unsigned char i, j;

	i = 5;
	j = 93;
	do
	{
		while (--j);
	} while (--i);
}


void delay_us(unsigned int us)
{
	unsigned char i;
	unsigned int j;
	for(j=0;j<us;j++)
	{
		i = 1;
		while(--i);
	}

}

*/
/*
void Delay100us()		//@22.1184MHz
{
	unsigned char i, j;

	i = 3;
	j = 35;
	do
	{
		while (--j);
	} while (--i);
}
*/
void Delay2ms()		//@22.1184MHz
{
	unsigned char i, j;

	i = 15;
	j = 114;
	do
	{
		while (--j);
	} while (--i);
}


	void  delay_38KHZ()
{
     unsigned char i;
	
	      i = 54;
		  while(--i);   //14T per loop
		  _nop_();
	
		
}
/*
void Delay2us()		//@22.1184MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 8;
	while (--i);
}

void Delay75us()		//@22.1184MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

void Delay4us()		//@22.1184MHz
{
	unsigned char i, j;

	i = 1;
	j = 206;
	do
	{
		while (--j);
	} while (--i);
}

void Delay20us()		//@22.1184MHz
{
	unsigned char i, j;

	_nop_();
	i = 4;
	j = 10;
	do
	{
		while (--j);
	} while (--i);
}

void Delay10us()		//@22.1184MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 2;
	j = 164;
	do
	{
		while (--j);
	} while (--i);
}

*/







