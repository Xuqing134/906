#ifndef	__IIC_H
#define	__IIC_H

//************************************
#define uint unsigned int
#define uchar unsigned char
#define Nack_counter 10

//mlx90614 端口定义
#define SCL P16//时钟线
#define SDA P15//数据线

//************函数声明*****************************************
void start_bit(); //MLX90614 发起始位子程序
void stop_bit(); //MLX90614 发结束位子程序
unsigned char rx_byte(void); //MLX90614 接收字节子程序
void send_bit(void); //MLX90614 发送位子程序
void tx_byte(unsigned char dat_byte); //MLX90614 接收字节子程序
void receive_bit(void); //MLX90614 接收位子程序
void delay(uint N);
unsigned int memread(void); //读温度数据

#endif
