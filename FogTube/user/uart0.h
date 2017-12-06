#ifndef	__UART0_H
#define	__UART0_H

#define uchar unsigned char
#define uint unsigned int

void UART_init(void);//串口初始化
void putChar(uchar a);//发送一个字节
void putstr(uchar str[],uchar length);//发送数组里的字符串
void putInt(uint a);//发送一个整型

#endif