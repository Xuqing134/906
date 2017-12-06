#include <STC15F2K60S2.h>
#include <adc.h>
#define VCC 5.02

	void delay(void)   
{
    unsigned char a,b;
    for(b=89;b>0;b--)
        for(a=247;a>0;a--);
}

	/*****  ADC��ʼ��  *****/

	void ADC_Int()
{
	
	CLK_DIV |= 0x20;				//ADC_RES��Ÿ�2λ�����ADC_RESL��ŵ�8λ���
	ADC_CONTR = 0x80;				//����Դ
	delay();						//��ʱ
	P1ASF |= 0xff;					//�����е�P1�ڶ���Ϊģ�⹦�ܡ�
	ADC_RES = 0x00;	
	ADC_RESL = 0x00;	
	
}

	/*****  ����10λADת���Ľ��  *****/

	unsigned int Get_ADC10bitResult(unsigned char channe1)	//channe1��ͨ��ѡ������Ϊ0~7
{
	unsigned char status = 0;
	unsigned int ADC_DA;
	unsigned char Tmp;
	ADC_CONTR = 0xe0 | channe1;				//���ת���ٶ�,������AD��ͨ��ѡ��
	ADC_CONTR |= 0x08;						//����ADת��
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
	
	P1ASF |= 0x02;							//ͨ��ѡ��				
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

