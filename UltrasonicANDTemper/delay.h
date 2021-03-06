
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


#ifndef	__DELAY_H
#define	__DELAY_H

//void  delay_ms(unsigned int ms);
//void delay_150us();
//void delay_us(unsigned int us);

#include <intrins.h>
void  delay_38KHZ();
void Delay2ms();
void Delay2us();		//@22.1184MHz
void Delay75us();		//@22.1184MHz
void Delay4us();		//@22.1184MHz
void Delay20us();		//@22.1184MHz
void Delay10us();		//@22.1184MHz
//void Delay100us();

#endif
