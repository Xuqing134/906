
/*****		����2���ص���ǰ�������������ݣ���������1			*****/
/*****		����3���ص����·������������ݣ���������2			*****/

#include <STC15F2K60S2.h>
#include "delay.h"
#include "fasong.h"
#include  "uart0.h"
#include <math.h>
#include <adc.h>

typedef unsigned char uchar;
typedef unsigned int uint;

#define ESC_choose1 P24		//�¶�ѡ���
#define ESC_choose2 P25

bit ON_flag = 0;
bit receive_flag1 = 0;			//����2���������ݽ��ձ�־
bit receive_flag2 = 0;			//����3���������ݽ��ձ�־
bit jiguan_receive_flag = 0;	//���������ɱ�־
bit flag = 0;
bit speed1_sgin;
bit speed2_sgin;
bit xiafang_finish = 0;		//�·��������ݽ��մ�����ɱ�־
bit CSB_flag = 0;
unsigned char count = 0;
bit receive_ESC_flag = 0;

uchar xdata csb1_data[2] = {0,0};		//������1���յ�����
uchar xdata csb2_data[2] = {0,0};		//������2���յ�����
uchar xdata jiguang_data[10] = {0,0,0,0,0,0,0,0,0,0};	//������յ�����

uchar xdata receive[10] = {0,0,0,0,0,0,0,0,0,0};		//���������ַ���
/*****		��ʱʱ�����		*****/
uchar count_100ms;				//��ʱ100ms��������ʱ

uchar safe_count = 0;
uchar safe_array_flag;
uchar speed1;					//��ǰ���ٶ�
uchar speed2;

uchar mn = 0;					//�����������λ��־
uchar qianfang_3m = 0;
uchar qianfang_10num = 0;		//��¼10����ͬ��ǰ������������

/*****		����������ߵ�λ����		*****/
uchar TH_s = 0xff;						//ǰ��������1����
uchar TL_s = 0xff;
uchar TH_ss = 0xff;					//��������ݣ�16������ʾ���ߵ���λ��
uchar TL_ss = 0xff;

uchar TH_csb1_data = 0;
uchar TL_csb1_data = 0;

uchar TH_csb2_data = 0;				//����������
uchar TL_csb2_data = 0;
uchar TH_jiguang_data = 0;			//����߰�λ����
uchar TL_jiguang_data = 0;			//����Ͱ�λ����
uchar TH_guangqiang = 0;			//�����ǿ�߰�λ
uchar TL_guangqiang = 0;			//�����ǿ�Ͱ�λ

uchar speed1_safe = 0;			//��������ʼ���İ�ȫ�ٶ�
uchar speed2_safe = 0;
uchar speed1_safe_flag = 0;
uchar speed2_safe_flag = 0;

uchar k,nn,nb;				
uchar baojing = 0x00;				//�����ź�
uchar int0_num = 0;					//��ʱ����
uchar csb_off_num = 0;				//�����������ղ������������ݣ������ֵ
uchar ESC_num = 0;				//����¶ȶ�ȡ����

uchar Temporary_temperature = 0;//�ݴ�ĵ���¶�
uchar actual_temperature1 = 0;	//���1ʵ���¶�
uchar actual_temperature2 = 0;	//���2ʵ���¶�
uchar actual_temperature3 = 0;	//���3ʵ���¶�
uchar actual_temperature4 = 0;	//���4ʵ���¶�

//-------------------------------------------------
//	��Դ��ѹ����
uint Jizhun_ADC = 0;			//��׼��ѹadc
unsigned long xdata Power_ADC = 0;				//��Դ��ѹadc
uint Power_data = 0;			//��Դ��ѹ
uchar xdata Power_data_TH = 0;
uchar xdata Power_data_TL = 0;
uint xdata Power_mean_array[10] = {0,0,0,0,0,0,0,0,0,0};
uchar Power_num = 0;
uint xdata numble = 0;
//-------------------------------------------------

uint s = 0;							//������1�����ݣ�10������ʾ��
uint ss = 0;						//�·������ݣ�10������ʾ��
uint csb_ss = 0;					//�������·������ݣ��ϲ���
uint jiguang_ss = 0;					//����������ݣ��ϲ���
uint guangqiang = 0;				//�����ǿ���ϲ���

uint csb1_safe;
uint csb2_safe;
uint xiafang_data = 0;
uint xiafang_data1 = 0;
uint qianfang_data = 0;
uint xdata safe1_array[2] = {0,0};	
uint xdata safe2_array[2] = {0,0};
uint xdata xiafang_array[2] = {0,0};		//�·�����
uint xdata qianfang_array[2] = {0,0};
uchar xdata ESC_data[25] = {0};//���յ����������

uchar code ADC_Temperature[119][2] = {//�¶ȱ�
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

void timer0_config();//��ʱ��0��ʼ��
bit receive_check(uchar str[],uchar num);//��������У��
void send_data(uchar T_DATA);//����1��������
void pass_choose();//���ѡ��
uchar search_temperature(uchar ADC_temperature1,uchar ADC_temperature2,uchar len);

void main()
{		
	P0M1 = 0x00;	P0M0 = 0x00;	//����Ϊ׼˫���
	P1M1 = 0xff;	P1M0 = 0x00;	//����Ϊ��������,ʹ��ADC
	P2M1 = 0x00;	P2M0 = 0xff;	//����Ϊ����;
	P3M1 = 0x00;	P3M0 = 0x30;	//����Ϊ׼˫���
	P4M1 = 0x00;	P4M0 = 0x1e;	//����Ϊ׼˫���;P41~P44Ϊ����
	P5M1 = 0x00;	P5M0 = 0x00;	//����Ϊ׼˫���
	P6M1 = 0x00;	P6M0 = 0x00;	//����Ϊ׼˫���
	
//	WDT_CONTR = 0x04;			//32��Ƶ
//	WDT_CONTR |= 0x20;			//�������Ź�						
		
	timer0_config();				//��ʱ����ʼ��
	UART_init();	//���ڳ�ʼ��
	ADC_Int();		//adc��ʼ��
	while(1)
	{	
		if(count_100ms == 0)
		{		
			ESC_num++;	//���������ÿ1���һ��������ݣ��ĸ������ѭ���ĴΡ�	
			if(ESC_num >= 10){
				ESC_num = 0;
				pass_choose();//���ͨ��ѡ��
				S3CON |= 0x10;//������3��������	
				Temporary_temperature = 0;//�ݴ��¶�����
			}
			csb_off_num++;	//����������ʧ�ܱ�����
			S2BUF = 0xe8;					//KS103�Ĵ��ڵ�ַ
			while(!(S2CON & 0x02));
			S2CON &= 0xfd;
			
			S2BUF = 0x02;					//KS103�ļĴ���2
			while(!(S2CON & 0x02));
			S2CON &= 0xfd;
			
			S2BUF = 0xbf;					//KS103��ָ��,�����²�
			while(!(S2CON & 0x02));
			S2CON &= 0xfd;	

			Delay2ms();	
			
			int0_num = 0;
			while(!CSB_flag)	//�ȴ�ks103�����źţ�6ms�����������ȴ�
			{					
				if(int0_num >= 3)
					break;
			}				
			
			P2 = 0x00;			
			fasong();		//��������������̽ͷ
			CSB_flag = 0;
			P2 = 0x00;
			P20 = 0;
			P23 = 1;
			P21 = 1;
			P22 = 0;
			IE2 |= 0x10;							//��������4�жϣ����ռ�����������
			WDT_CONTR = 0x35;	//ι��	
			while(count_100ms <= 35)				//35*2=70ms��70ms�ڲ�ѯ����������	
			{
					//���������ɱ�־
/****************************************************************************
							�·���������������
****************************************************************************/									
				if(receive_flag1)
				{
					P37 = ~P37;
					csb_off_num = 0;
					receive_flag1 = 0;				
					TH_csb2_data = csb1_data[0];			//���泬�������ݣ�16���ƣ�
					TL_csb2_data = csb1_data[1];	
					
					csb_ss = TH_csb2_data;
					csb_ss <<= 8;
					csb_ss |= TL_csb2_data;															
					
//					ss = csb2_data[0];				//���泬�������ݣ�10���ƣ�
//					ss <<= 8;
//					ss |= csb2_data[1];
					
	/*****				����ͳ�����2���·����ľ����ں�			*****/
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
			jiguan_receive_flag = 0;	//���������ɱ�־��0
									
			//-------------------------------------------------------
//					��Դ��ѹ����
			Jizhun_ADC = Get_ADC10bitResult(1);//P1.1��׼��ѹ����
			Power_ADC = Get_ADC10bitResult(3);//P1.3��Դ��ѹ����
			
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
			
			Power_data_TH = Power_data>>8;		//��ѹ			
			Power_data_TL = Power_data%256;		//��ѹ
			
//-------------------------------------------------------		
		}
	}
}

/************************************************
				��ʱ��0��ʼ��
************************************************/	
	void timer0_config()			
	{
		AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
		TMOD &= 0xF0;		//�趨��ʱ��ģʽ
		TL0 = 0x33;		//�趨��ʱ����ֵ
		TH0 = 0x53;		
		TF0 = 0;		//���TF0(��ʱ�����־)
		TR0 = 1;		//������ʱ��
		
		EA = 1;			//�����ж�
		ET0 = 1;		//����ʱ��0�жϣ�����2ms��ʱ��
//		PX1 = 1;		//�ⲿ�ж�1���ȼ�Ϊ1
		PT0 = 0;		//��ʱ��0���ȼ�Ϊ0
		IP2 = 0x01;		//����2���ȼ�Ϊ1;
		PS = 0;			//����1���ȼ�Ϊ0
		IE2 &= 0x76;	//��ES2������2�ж�����λ��������0�Ͷ�ES3������3�ж�����λ��������0�ʹ���4
		IE2 |= 0x09;	//��ES2��1��������2�жϺͶ�ES3��1��������3�жϺʹ���4
		ES = 1;	
		EX0 = 1;		//���ⲿ�ж�
		IT0 = 1;		//�½��ش���
	}	

	void Int0_Routine() interrupt 0
	{
		IE0 = 0;
		CSB_flag = 1;
		P36 = ~P36;
	}
/************************************************
				��ʱ��0�ж�
************************************************/		
	void timer0_Routine() interrupt 1
	{		
		TF0 = 0;
		int0_num++;
		count_100ms++;			//100msѭ����ʱ
		if(count_100ms>=50)		//100ms��ʱ��
			count_100ms=0;
		
	}
/************************************************
				����1�жϳ���
		1���������ݸ���λ��
		2��������λ���·�������趨ֵ
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
						check = 0x5a+0xa5+0x10+0x0e+0x11+TH_s+TL_s+TH_ss+TL_ss+speed1+speed2+Power_data_TH+Power_data_TL;		//У��λ
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
		����4�жϳ��򣺽��ռ�������
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
					IE2 &= 0x6f;						//�رմ���
					
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
		����2�жϳ��򣺽���ǰ������������(�·�)
************************************************/	
	void UART2_Routine() interrupt 8
		{
//			uchar n;
			if(S2CON & 0x01)
			{
				S2CON &= 0xfe;				//�����ж���0
				csb1_data[nb++] = S2BUF;		//�����������ݱ��浽csb_data������	
				if(nb == 2) 
				{
					nb = 0;
					receive_flag1 = 1;
				}
			}		
		}		
/************************************************
		����3�жϳ��򣺽����·�����������
************************************************/		
	void UART3_Routine() interrupt 17
		{
			static uchar ESC_count;
			if(S3CON & 0x01)
			{
				S3CON &= 0xfe;				//�����ж���0,��ֹ�����ж�
				ESC_data[ESC_count++] = S3BUF;		//�����������ݱ��浽csb_data������	
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
				�������ݼ��
	˵�����Խ��յ������ݽ���У��
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
			Handle_data = (uint)(str[num-1]<<8)+(str[num-2]);//�Ѹߵ�λ�ϳ�����
		
			if(Handle_data == check)
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
/************************************************
				��������
		˵�������ʹ���1������
************************************************/	
	void send_data(uchar T_DATA)
	{
		SBUF = T_DATA;
		while(!TI);
		TI = 0;
	}
/************************************************
				���ѡ��
		˵����˳��ѡ���ĸ����
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
				�����(���ֲ���)
		˵���������¶�����,ADCֵԽС���¶�Խ��
************************************************/		
	uchar search_temperature(uchar ADC_temperature1,uchar ADC_temperature2,uchar len)
	{
		uchar mix_ADC_temperature;
		uchar low,high,mid;
		if(ADC_temperature1 <= ADC_temperature2)
			mix_ADC_temperature = ADC_temperature1;
		else
			mix_ADC_temperature = ADC_temperature2;	//�������¶�ADCֵ�Ƚϣ�ȡСֵ����ȡ�¶ȴ�ֵ��
		
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
						�������õ���I/O��
				���ô���2��TXD��������������������
	P00		
	P01					//����S3_SΪ0����ѡ��P00ΪRxD3,P01ΪTxD3���·�
	P02
	P03					//RxD4ΪP02  TxD4ΪP03
	P04					//��������������ͷ���ź�
	
	P1					//����P1����Ϊadc����ģʽ
	P10					//�Ժ�����е�ѹ����
	
	P20					
	P21					
	P22					
	P23					//P20~P23��������ͷ
	
	P24					����¶�ѡ��
	P25					����¶�ѡ��
	
	P30					//RXD
	P31					//TXD
	dianhuo P34			//���
	qifa P35			//����
	
	diancifa1 P41		//��ŷ�1
	diancifa2 P42		//��ŷ�2
	diancifa3 P43		//��ŷ�3
	diancifa4 P44		//��ŷ�4
	P46		
	P47					//����S2_SΪ1����ѡ��P46ΪRxD2_2,P47ΪTxD2_2��ǰ��

	P60					//����λ����������ָʾ��
	P61					//�·������쳣
	P62
	

***************************************************************************/
		
		
