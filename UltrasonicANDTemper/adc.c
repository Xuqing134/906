#include <STC15F2K60S2.h>
#include <adc.h>
#define VCC 5.02

	void delay(void)   
{
    unsigned char a,b;
    for(b=89;b>0;b--)
        for(a=247;a>0;a--);
}

	/*****  ADC初始化  *****/

	void ADC_Int()
{
	
	CLK_DIV |= 0x20;				//ADC_RES存放高2位结果，ADC_RESL存放低8位结果
	ADC_CONTR = 0x80;				//开电源
	delay();						//延时
	P1ASF |= 0xff;					//将所有的P1口都设为模拟功能。
	ADC_RES = 0x00;	
	ADC_RESL = 0x00;	
	
}

	/*****  返回10位AD转换的结果  *****/

	unsigned int Get_ADC10bitResult(unsigned char channe1)	//channe1：通道选择，数据为0~7
{
	unsigned char status = 0;
	unsigned int ADC_DA;
	unsigned char Tmp;
	ADC_CONTR = 0xe0 | channe1;				//最高转换速度,不开启AD，通道选择
	ADC_CONTR |= 0x08;						//开启AD转换
	while(status == 0)
	{
		status = ADC_CONTR&0x10;
	}
	
	ADC_DA = ADC_RES;
	ADC_DA <<= 8;
	Tmp = ADC_RESL;
	ADC_DA |= Tmp;
	ADC_CONTR &= 0xe7;	
	ADC_RES = 0x00;	
	ADC_RESL = 0x00;
	return ADC_DA;
	
}




/*
	void ADC_Int()
{
	
	P1ASF |= 0x02;							//通道选择				
	ADC_CONTR = 0xe6;						
	delay();	
	ADC_RES = 0x00;	
	ADC_RESL = 0x00;	
	
}


	unsigned int Get_ADC10bitResult()
{
	unsigned char status = 0;
	unsigned int ADC_DA;
	unsigned char Tmp;
	ADC_CONTR |= 0x08;						
	while(status == 0)
	{
		status = ADC_CONTR&0x10;
	}	
	ADC_DA = ADC_RES;
	ADC_DA <<= 2;
	Tmp = ADC_RESL;
	Tmp &= 0x03;
	ADC_DA |= Tmp;
	ADC_CONTR &= 0xe7;	
	return ADC_DA;
	
}

*/

