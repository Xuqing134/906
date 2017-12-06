#include "iic.h"
#include <STC15F2K60S2.h>
#include "intrins.h"

//************数据定义****************
bdata uchar flag;//可位寻址数据
sbit bit_out=flag^7;
sbit bit_in=flag^0;
uchar DataH,DataL,Pecreg;
//************函数声明*****************************************
void start_bit(); //MLX90614 发起始位子程序
void stop_bit(); //MLX90614 发结束位子程序
uchar rx_byte(void); //MLX90614 接收字节子程序
void send_bit(void); //MLX90614 发送位子程序
void tx_byte(uchar dat_byte); //MLX90614 接收字节子程序
void receive_bit(void); //MLX90614 接收位子程序
uint memread(void); //读温度数据
void UartInit(void);//串口初始化

void start_bit(void)
{
	uchar a;
	SDA=1;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();_nop_();}
	SCL=1;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();_nop_();}
	SDA=0;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();_nop_();}
	SCL=0;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();_nop_();}
}
//------------------------------
void stop_bit(void)
{	
	uchar a;
	SCL=0;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();_nop_();}
	SDA=0;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();_nop_();}
	SCL=1;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();_nop_();}
	SDA=1;
}
//---------发送一个字节---------
void tx_byte(uchar dat_byte)
{
	char i,n,dat;
	n=Nack_counter;
	TX_again:
	dat=dat_byte;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		bit_out=1;
		else
		bit_out=0;
		send_bit();
		dat=dat<<1;
	}
	receive_bit();
	if(bit_in==1)
	{
		stop_bit();
		if(n!=0)
		{n--;goto Repeat;}
		else
		goto exit;
	}
	else
		goto exit;
	Repeat:
	start_bit();
	goto TX_again;
	exit: ;
}
//-----------发送一个位---------
void send_bit(void)
{
	uchar a;
	if(bit_out==0)
		SDA=0;
	else
		SDA=1;
	_nop_();
	SCL=1;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();}
	SCL=0;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();}
}
//----------接收一个字节--------
uchar rx_byte(void)
{
	uchar i,dat;
	dat=0;
	for(i=0;i<8;i++)
	{
	dat=dat<<1;
	receive_bit();
	if(bit_in==1)
	dat=dat+1;
	}
	send_bit();
	return dat;
}
//----------接收一个位----------
void receive_bit(void)
{
	uchar a;
	SDA=1;bit_in=1;
	SCL=1;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();}
	bit_in=SDA;
	for(a=0;a<10;a++){
	_nop_();}
	SCL=0;
	for(a=0;a<10;a++){
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();}
}

//------------延时--------------
void delay(uint N)
{
	uint i;
	for(i=0;i<N;i++)
	_nop_();
}

//------------------------------
uint memread(void)		//读数据
{
	start_bit();
	tx_byte(0x00); //发送从地址00
	tx_byte(0x07); //发送命令
	//------------
	start_bit();
	tx_byte(0x01);	//读从地址指令
	bit_out=0;
	DataL=rx_byte();
	bit_out=0;
	DataH=rx_byte();
	bit_out=1;
	Pecreg=rx_byte();
	stop_bit();
	return(DataH*256+DataL);
}


