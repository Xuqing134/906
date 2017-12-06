#include  "uart0.h"
#include <STC15F2K60S2.h>

/*****		串口1为115200，串口2，3初始化，波特率都为9600,串口4波特率为115200		*****/
void UART_init(void)
{
	SCON = 0x50;		
	AUXR |= 0x40;		
	AUXR &= 0xFE;		
	TMOD &= 0x0F;		
	TL1 = 0xD0;		
	TH1 = 0xFF;		
	ET1 = 0;		
	TR1 = 1;		
		
	S2CON = 0x50;		//串口2，8位数据，可变波特率
	AUXR |= 0x04;		//定时器2时钟为1T
	T2L = 0xC0;		
	T2H = 0xFD;		
	AUXR |= 0x10;		//启动定时器2
	
	S3CON = 0x10;		//串口3，8位数据，可变波特率
	S3CON |= 0x40;		//串口3选择定时器3为波特率发生器
	T4T3M |= 0x02;		//定时器3时钟为1T
	T3L = 0xC0;		
	T3H = 0xFD;		
	T4T3M |= 0x08;		//启动定时器3
	
	P_SW2 |= 0x01;		//设置S2_S为1，即选择P46为RxD2_2,P47为RxD2_2。
						//设置S3_S为0，即选择P00为RxD3,P01为TxD3。
	
	S4CON = 0x10;		//RxD4为P02  TxD4为P03
	S4CON |= 0x40;		
	T4T3M |= 0x20;		
	T4L = 0xD0;		
	T4H = 0xFF;		
	T4T3M |= 0x80;		
	
//	TI = 1;
}

//************发送一个字节*************
void putChar(uchar a)
{
	SBUF = a;
	while(!TI);
	TI = 0;
}

//***********发送一个整型*************
//     说明：高位在前，低位在后
void putInt(uint a)
{
	putChar(a>>8);
	putChar(a%256);
}
//*********发送数组里的字符串*********
void putstr(uchar str[],uchar length)
{
	uchar i;
	for(i=0;i<length;i++)
		putChar(str[i]);	
}



