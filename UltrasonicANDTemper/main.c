
/*****		串口2返回的是前方超声波的数据，即超声波1			*****/
/*****		串口3返回的是下方超声波的数据，即超声波2			*****/

#include <STC15F2K60S2.h>
#include "delay.h"
#include "fasong.h"
#include  "uart0.h"
#include <math.h>
#include <adc.h>

typedef unsigned char uchar;
typedef unsigned int uint;

#define ESC_choose1 P24		//温度选择脚
#define ESC_choose2 P25

bit ON_flag = 0;
bit receive_flag1 = 0;			//串口2超声波数据接收标志
bit receive_flag2 = 0;			//串口3超声波数据接收标志
bit jiguan_receive_flag = 0;	//激光接收完成标志
bit flag = 0;
bit speed1_sgin;
bit speed2_sgin;
bit xiafang_finish = 0;		//下方距离数据接收处理完成标志
bit CSB_flag = 0;
unsigned char count = 0;
bit receive_ESC_flag = 0;

uchar xdata csb1_data[2] = {0,0};		//超声波1接收的数据
uchar xdata csb2_data[2] = {0,0};		//超声波2接收的数据
uchar xdata jiguang_data[10] = {0,0,0,0,0,0,0,0,0,0};	//激光接收的数据

uchar xdata receive[10] = {0,0,0,0,0,0,0,0,0,0};		//接收命令字符串
/*****		定时时间控制		*****/
uchar count_100ms;				//定时100ms，基本定时

uchar safe_count = 0;
uchar safe_array_flag;
uchar speed1;					//当前的速度
uchar speed2;

uchar mn = 0;					//激光接收数据位标志
uchar qianfang_3m = 0;
uchar qianfang_10num = 0;		//记录10个相同的前方超声波数据

/*****		超声波距离高低位数据		*****/
uchar TH_s = 0xff;						//前方超声波1距离
uchar TL_s = 0xff;
uchar TH_ss = 0xff;					//下面的数据（16进制显示，高低两位）
uchar TL_ss = 0xff;

uchar TH_csb1_data = 0;
uchar TL_csb1_data = 0;

uchar TH_csb2_data = 0;				//超声波数据
uchar TL_csb2_data = 0;
uchar TH_jiguang_data = 0;			//激光高八位数据
uchar TL_jiguang_data = 0;			//激光低八位数据
uchar TH_guangqiang = 0;			//激光光强高八位
uchar TL_guangqiang = 0;			//激光光强低八位

uchar speed1_safe = 0;			//超声波初始化的安全速度
uchar speed2_safe = 0;
uchar speed1_safe_flag = 0;
uchar speed2_safe_flag = 0;

uchar k,nn,nb;				
uchar baojing = 0x00;				//报警信号
uchar int0_num = 0;					//超时报数
uchar csb_off_num = 0;				//当连续三次收不到超声波数据，则报最大值
uchar ESC_num = 0;				//电调温度读取计数

uchar Temporary_temperature = 0;//暂存的电调温度
uchar actual_temperature1 = 0;	//电调1实际温度
uchar actual_temperature2 = 0;	//电调2实际温度
uchar actual_temperature3 = 0;	//电调3实际温度
uchar actual_temperature4 = 0;	//电调4实际温度

//-------------------------------------------------
//	电源电压测量
uint Jizhun_ADC = 0;			//基准电压adc
unsigned long xdata Power_ADC = 0;				//电源电压adc
uint Power_data = 0;			//电源电压
uchar xdata Power_data_TH = 0;
uchar xdata Power_data_TL = 0;
uint xdata Power_mean_array[10] = {0,0,0,0,0,0,0,0,0,0};
uchar Power_num = 0;
uint xdata numble = 0;
//-------------------------------------------------

uint s = 0;							//超声波1的数据（10进制显示）
uint ss = 0;						//下方的数据（10进制显示）
uint csb_ss = 0;					//超声波下方的数据（合并）
uint jiguang_ss = 0;					//激光距离数据（合并）
uint guangqiang = 0;				//激光光强（合并）

uint csb1_safe;
uint csb2_safe;
uint xiafang_data = 0;
uint xiafang_data1 = 0;
uint qianfang_data = 0;
uint xdata safe1_array[2] = {0,0};	
uint xdata safe2_array[2] = {0,0};
uint xdata xiafang_array[2] = {0,0};		//下方数组
uint xdata qianfang_array[2] = {0,0};
uchar xdata ESC_data[25] = {0};//接收电调数据数组

uchar code ADC_Temperature[119][2] = {//温度表
{241,0}, {240,1}, {239,2}, {238,3}, {237,4}, {236,5}, {235,6}, {234,7}, {233,8}, {232,9},
{231,10},{230,11},{229,12},{228,13},{227,14},{226,15},{224,16},{223,17},{222,18},{220,19},
{219,20},{217,21},{216,22},{214,23},{213,24},{211,25},{209,26},{208,27},{206,28},{204,29},
{202,30},{201,31},{199,32},{197,33},{195,34},{193,35},{191,36},{189,37},{187,38},{185,39},
{183,40},{181,41},{179,42},{177,43},{174,44},{172,45},{170,46},{168,47},{166,48},{164,49},
{161,50},{159,51},{157,52},{154,53},{152,54},{150,55},{148,56},{146,57},{143,58},{141,59},
{139,60},{136,61},{134,62},{132,63},{130,64},{128,65},{125,66},{123,67},{121,68},{119,69},
{117,70},{115,71},{113,72},{111,73},{109,74},{106,75},{105,76},{103,77},{101,78},{99,79},
{97,80}, {95,81}, {93,82}, {91,83}, {90,84}, {88,85}, {85,86}, {84,87}, {82,88}, {81,89},
{79,90}, {77,91}, {76,92}, {74,93}, {73,94}, {72,95}, {69,96}, {68,97}, {66,98}, {65,99},
{64,100},{62,102},{61,103},{59,104},{58,105},{56,106},{54,108},{53,109},{51,111},{50,112},
{48,114},{46,116},{44,117},{43,119},{41,121},{39,124},{37,126},{35,128},{33,129},
};

void timer0_config();//定时器0初始化
bit receive_check(uchar str[],uchar num);//接收数据校验
void send_data(uchar T_DATA);//串口1发送数据
void pass_choose();//电调选择
uchar search_temperature(uchar ADC_temperature1,uchar ADC_temperature2,uchar len);

void main()
{		
	P0M1 = 0x00;	P0M0 = 0x00;	//设置为准双向口
	P1M1 = 0xff;	P1M0 = 0x00;	//设置为高阻输入,使用ADC
	P2M1 = 0x00;	P2M0 = 0xff;	//设置为推挽;
	P3M1 = 0x00;	P3M0 = 0x30;	//设置为准双向口
	P4M1 = 0x00;	P4M0 = 0x1e;	//设置为准双向口;P41~P44为推挽
	P5M1 = 0x00;	P5M0 = 0x00;	//设置为准双向口
	P6M1 = 0x00;	P6M0 = 0x00;	//设置为准双向口
	
//	WDT_CONTR = 0x04;			//32分频
//	WDT_CONTR |= 0x20;			//启动看门狗						
		
	timer0_config();				//定时器初始化
	UART_init();	//串口初始化
	ADC_Int();		//adc初始化
	while(1)
	{	
		if(count_100ms == 0)
		{		
			ESC_num++;	//电调计数，每1秒读一个电调数据，四个电调，循环四次。	
			if(ESC_num >= 10){
				ESC_num = 0;
				pass_choose();//电调通道选择
				S3CON |= 0x10;//允许串口3接收数据	
				Temporary_temperature = 0;//暂存温度清零
			}
			csb_off_num++;	//超声波计数失败报数。
			S2BUF = 0xe8;					//KS103的串口地址
			while(!(S2CON & 0x02));
			S2CON &= 0xfd;
			
			S2BUF = 0x02;					//KS103的寄存器2
			while(!(S2CON & 0x02));
			S2CON &= 0xfd;
			
			S2BUF = 0xbf;					//KS103的指令,不带温补
			while(!(S2CON & 0x02));
			S2CON &= 0xfd;	

			Delay2ms();	
			
			int0_num = 0;
			while(!CSB_flag)	//等待ks103驱动信号，6ms过后，则跳出等待
			{					
				if(int0_num >= 3)
					break;
			}				
			
			P2 = 0x00;			
			fasong();		//驱动超声波发射探头
			CSB_flag = 0;
			P2 = 0x00;
			P20 = 0;
			P23 = 1;
			P21 = 1;
			P22 = 0;
			IE2 |= 0x10;							//开启串口4中断，接收激光距离的数据
			WDT_CONTR = 0x35;	//喂狗	
			while(count_100ms <= 35)				//35*2=70ms，70ms内查询超声波返回	
			{
					//激光接收完成标志
/****************************************************************************
							下方超声波距离数据
****************************************************************************/									
				if(receive_flag1)
				{
					P37 = ~P37;
					csb_off_num = 0;
					receive_flag1 = 0;				
					TH_csb2_data = csb1_data[0];			//储存超声波数据（16进制）
					TL_csb2_data = csb1_data[1];	
					
					csb_ss = TH_csb2_data;
					csb_ss <<= 8;
					csb_ss |= TL_csb2_data;															
					
//					ss = csb2_data[0];				//储存超声波数据（10进制）
//					ss <<= 8;
//					ss |= csb2_data[1];
					
	/*****				激光和超声波2（下方）的距离融合			*****/
					if(csb_ss <= 6000)
					{
						xiafang_data = csb_ss;
						xiafang_data1 = csb_ss;
					}
					else if((jiguang_ss >= 2000)&&(guangqiang >= 15))
					{
						xiafang_data = jiguang_ss;
						xiafang_data1 = jiguang_ss;																								
					}	
					else
						xiafang_data = 0xffff;
					if((abs(xiafang_data1-xiafang_array[0]) >= 1000)&&(abs(xiafang_data1-xiafang_array[1]) >= 1000))
					{
						xiafang_data = 0xffff;		
						P61 = ~P61;
					}
						xiafang_array[1] = xiafang_array[0];
						xiafang_array[0] = xiafang_data1;
				
						TH_ss = xiafang_data>>8;
						TL_ss = xiafang_data%256;
				}			
		}	
			if(csb_off_num >= 3){
				TH_ss = 0xff;TL_ss = 0xff;
			}			
			jiguan_receive_flag = 0;	//激光接收完成标志清0
									
			//-------------------------------------------------------
//					电源电压测量
			Jizhun_ADC = Get_ADC10bitResult(1);//P1.1基准电压测量
			Power_ADC = Get_ADC10bitResult(3);//P1.3电源电压测量
			
			Power_data = ((Power_ADC<<8)-(Power_ADC<<3))/Jizhun_ADC;
			
			for(k=9;k>0;k--)
			{
				Power_mean_array[k] = Power_mean_array[k-1];
			}		
			Power_mean_array[0] = Power_data;
	
			Power_num++;
			if(Power_num > 11)
				Power_num = 11;
			
			if(Power_num == 11)
			{
				uchar a = 0;
				for(a=0;a<10;a++)
				{				
					numble += Power_mean_array[a];
				}				
				Power_data = numble/10;
				numble = 0;
			}			
			
			Power_data_TH = Power_data>>8;		//电压			
			Power_data_TL = Power_data%256;		//电压
			
//-------------------------------------------------------		
		}
	}
}

/************************************************
				定时器0初始化
************************************************/	
	void timer0_config()			
	{
		AUXR |= 0x80;		//定时器时钟1T模式
		TMOD &= 0xF0;		//设定定时器模式
		TL0 = 0x33;		//设定定时器初值
		TH0 = 0x53;		
		TF0 = 0;		//清除TF0(定时溢出标志)
		TR0 = 1;		//开启定时器
		
		EA = 1;			//开总中断
		ET0 = 1;		//开定时器0中断（产生2ms定时）
//		PX1 = 1;		//外部中断1优先级为1
		PT0 = 0;		//定时器0优先级为0
		IP2 = 0x01;		//串口2优先级为1;
		PS = 0;			//串口1优先级为0
		IE2 &= 0x76;	//对ES2（串口2中断允许位）进行清0和对ES3（串口3中断允许位）进行清0和串口4
		IE2 |= 0x09;	//对ES2置1，开串口2中断和对ES3置1，开串口3中断和串口4
		ES = 1;	
		EX0 = 1;		//开外部中断
		IT0 = 1;		//下降沿触发
	}	

	void Int0_Routine() interrupt 0
	{
		IE0 = 0;
		CSB_flag = 1;
		P36 = ~P36;
	}
/************************************************
				定时器0中断
************************************************/		
	void timer0_Routine() interrupt 1
	{		
		TF0 = 0;
		int0_num++;
		count_100ms++;			//100ms循环计时
		if(count_100ms>=50)		//100ms定时到
			count_100ms=0;
		
	}
/************************************************
				串口1中断程序
		1、发送数据给上位机
		2、接收上位机下发的命令及设定值
************************************************/		
		void UART1_Routine() interrupt 4
		{				
			uchar check;
			uchar h;
			if(RI)
			{
				RI = 0;
				receive[count++] = SBUF;
				if(count <= 6)
				{	
					if((receive[0] == 0x5a)&&(receive[1] == 0xa5)&&(receive[2] == 0x10)&&(receive[3] == 0x06)&&(receive[4]==0x00)&&(receive[5]==0x15))
//					if((receive[0] == 0x0a)&&(receive[1] == 0x04)&&(receive[2] == 0x00)&&(receive[3] == 0x0e))
					{							
						count = 0;
						P60 = ~P60;
						check = 0x5a+0xa5+0x10+0x0e+0x11+TH_s+TL_s+TH_ss+TL_ss+speed1+speed2+Power_data_TH+Power_data_TL;		//校验位
						check = check+actual_temperature1+actual_temperature2+actual_temperature3+actual_temperature4;	
						send_data(0x5a);
						send_data(0xa5);
						send_data(0x10);
						send_data(0x0e);
						send_data(0x11);
						send_data(TL_s);
						send_data(TH_s);
						send_data(TL_ss);
						send_data(TH_ss);
						send_data(speed1);
						send_data(speed2);						
						send_data(Power_data_TL);
						send_data(Power_data_TH);
						
//						send_data(actual_temperature1);
//						send_data(actual_temperature2);
//						send_data(actual_temperature3);
//						send_data(actual_temperature4);
						
						send_data(check);

						nn = 0;
						for(h=0;h<8;h++)
						{
							receive[h] = 0;
						}

					}						
				}		
				
				if(count >= 10)
				{
					count = 0;
				}									
			}
			
		}
	
/************************************************
		串口4中断程序：接收激光数据
************************************************/		
	void UART4_Routine() interrupt 18
		{	
			if(S4CON & 0x01)
			{
				S4CON &= 0xfe;
				jiguang_data[mn++] = S4BUF;
				
				if(mn == 9)
				{
					mn = 0;
					jiguan_receive_flag = 1;
					IE2 &= 0x6f;						//关闭串口
					
					TL_jiguang_data = 0;
					TH_jiguang_data = 0;
					TL_guangqiang = 0;
					TH_guangqiang = 0;
					TL_jiguang_data = jiguang_data[2];
					TH_jiguang_data = jiguang_data[3];
					TL_guangqiang = jiguang_data[4];
					TH_guangqiang = jiguang_data[5];
					
					jiguang_ss = TH_jiguang_data;
					jiguang_ss <<= 8;
					jiguang_ss |= TL_jiguang_data;
					jiguang_ss = jiguang_ss * 10;
					
					guangqiang = TH_guangqiang;
					guangqiang <<= 8;
					guangqiang |= TL_guangqiang;				
				}
			}				
		}	
/************************************************
		串口2中断程序：接收前方超声波数据(下方)
************************************************/	
	void UART2_Routine() interrupt 8
		{
//			uchar n;
			if(S2CON & 0x01)
			{
				S2CON &= 0xfe;				//接收中断清0
				csb1_data[nb++] = S2BUF;		//将超声波数据保存到csb_data变量中	
				if(nb == 2) 
				{
					nb = 0;
					receive_flag1 = 1;
				}
			}		
		}		
/************************************************
		串口3中断程序：接收下方超声波数据
************************************************/		
	void UART3_Routine() interrupt 17
		{
			static uchar ESC_count;
			if(S3CON & 0x01)
			{
				S3CON &= 0xfe;				//接收中断清0,禁止接收中断
				ESC_data[ESC_count++] = S3BUF;		//将超声波数据保存到csb_data变量中	
				if(ESC_data[ESC_count-1] == 0x9b)
				{
					ESC_count = 1;
					ESC_data[0] = 0x9b;
				}
				if(ESC_count == 24)
				{
					S3CON &= 0xef;
					if(receive_check(ESC_data,24))
					{
						ESC_count = 0;
						receive_ESC_flag = 1;
						Temporary_temperature = (search_temperature(ESC_data[18],ESC_data[19],119));						
					}
				}
			}
		}
		
	
/************************************************
				接收数据检测
	说明：对接收到的数据进行校验
************************************************/
		
	bit receive_check(uchar str[],uchar num)
	{
		uchar n;
		uint check = 0;
		static uint Handle_data = 0;
		if((str[0]==0x9b)&&(str[1]==0x16)&&(str[2]==0x01))
		{
			for(n=0;n<num-2;n++)
				check+=str[n];	
			Handle_data = (uint)(str[num-1]<<8)+(str[num-2]);//把高低位合成整形
		
			if(Handle_data == check)
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
/************************************************
				发送数据
		说明：发送串口1的数据
************************************************/	
	void send_data(uchar T_DATA)
	{
		SBUF = T_DATA;
		while(!TI);
		TI = 0;
	}
/************************************************
				电调选择
		说明：顺序选择四个电调
************************************************/	
	void pass_choose()
	{
		static uchar num;
		num++;
		if(num >= 5)
			num = 1;
		switch(num)	
		{
			case 1:actual_temperature4=Temporary_temperature; ESC_choose1=0; ESC_choose2=0;break;
			case 2:actual_temperature1=Temporary_temperature; ESC_choose1=0; ESC_choose2=1;break;
			case 3:actual_temperature2=Temporary_temperature; ESC_choose1=1; ESC_choose2=0;break;
			case 4:actual_temperature3=Temporary_temperature; ESC_choose1=1; ESC_choose2=1;break;
		}	
	}
	
/************************************************
				查表函数(二分查找)
		说明：查找温度数据,ADC值越小，温度越高
************************************************/		
	uchar search_temperature(uchar ADC_temperature1,uchar ADC_temperature2,uchar len)
	{
		uchar mix_ADC_temperature;
		uchar low,high,mid;
		if(ADC_temperature1 <= ADC_temperature2)
			mix_ADC_temperature = ADC_temperature1;
		else
			mix_ADC_temperature = ADC_temperature2;	//将两个温度ADC值比较，取小值（即取温度大值）
		
		while(1)
		{
			low = 0;high = len-1;
			if(mix_ADC_temperature < 33)
				return 0xff;
			else if(mix_ADC_temperature > 241)
				return 0xfe;
			else
			{			
				while(low <= high)
				{
					mid = (low+high)/2;
					if(ADC_Temperature[mid][0] == mix_ADC_temperature)						
						return ADC_Temperature[mid][1];
					else if(ADC_Temperature[mid][0] > mix_ADC_temperature)
						low = mid+1;
					else if(ADC_Temperature[mid][0] < mix_ADC_temperature)
						high = mid-1;
				}
				mix_ADC_temperature--;
			}
		}
	}
	

	

/***************************************************************************
						程序所用到的I/O口
				共用串口2的TXD给两个超声波发送数据
	P00		
	P01					//设置S3_S为0，即选择P00为RxD3,P01为TxD3。下方
	P02
	P03					//RxD4为P02  TxD4为P03
	P04					//驱动超声波发射头的信号
	
	P1					//整个P1口设为adc工作模式
	P10					//对红外进行电压采样
	
	P20					
	P21					
	P22					
	P23					//P20~P23驱动发射头
	
	P24					电调温度选择
	P25					电调温度选择
	
	P30					//RXD
	P31					//TXD
	dianhuo P34			//点火
	qifa P35			//气阀
	
	diancifa1 P41		//电磁阀1
	diancifa2 P42		//电磁阀2
	diancifa3 P43		//电磁阀3
	diancifa4 P44		//电磁阀4
	P46		
	P47					//设置S2_S为1，即选择P46为RxD2_2,P47为TxD2_2。前方

	P60					//向上位机发送数据指示灯
	P61					//下方数据异常
	P62
	

***************************************************************************/
		
		
