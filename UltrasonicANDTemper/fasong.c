#include	"fasong.h"
#include	"delay.h"
#include <STC15F2K60S2.h>

unsigned char x,i;

void fasong()
{
   for(x=0;x<16;x++)	  //38
   { 
	  ce_1();
	  delay_38KHZ();
	  ce_2();
	  delay_38KHZ();
	  ce_3();
	
   }
}

void ce_1 ()
{
	P2 = 0x00;
	P2=0x00;
	P2=0x00;
	for(i=0;i<2;i++)
	{
	   	P21=1;
		P22=1;
	}
}

void ce_2 ()
{ 
	
		P2=0x00;
		P2=0x00;
		P2=0x00;
 	for(i=0;i<2;i++)
	{
	   	P20=1;
		P23=1;
	}
}
	
void ce_3 ()
{
	P2=0x00;
	P2=0x00;
	P2=0x00;
}

