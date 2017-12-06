#ifndef	__FAYAN_H
#define	__FAYAN_H

#include  "uart0.h"
#include <STC15F2K60S2.h>
#include "iic.h"
#include "intrins.h"
#include "math.h"

sbit look  = P5^5;

#define MAX(a,b) ((a > b) ? a : b)
#define MIN(a,b) ((a < b) ? a : b)

//****************宏定义*******************
#define uchar unsigned char
#define uint unsigned int
#define yanyou1  P20	
#define yanyou2  P21
#define yanyou3  P22
#define yanyou4  P23		//四个管子导通电磁阀
#define qiti     P24		//气体电磁阀
#define dianhuo1 P25		
#define dianhuo2 P26		//两个点火器
#define shuibeng P27		//水泵

#define dianhuo_time 	20		//点火时长
#define check_time		50		//气体检测周期
#define clean_time   	30		//清洁导通时长
#define CLEANPULSE      20      //清洁导通脉冲数
#define wait_OFF_time   150		//关闭发烟后，在设定时间内没有发烟，则关闭系统

#define CYC_TOTAL 40      //
#define DETECT_TIME 10  //(1-0.4)/1S数脉冲长度
#define EXECUTE_TIME 150
#define EXECUTE_TIME0 8  //0.8s
#define EXECUTE_TIME1 5
#define EXECUTE_BUF_TIME 2
//#define EXECUTE_SEG 1 //15s执行阶段分段
#define PREPARE_TIME 50//5s
#define BUF_TIME 0 //0.5s作缓冲，4管停止发烟
#define COMPENSATE_TIME0 50 //5s补偿阶段
#define COMPENSATE_TIME1 100
#define TUBENUM 4  //烟管总数
//#define PUL_STAND_MAX 21 //0.8S内检测到的基准脉冲最大值
//#define PUL_STAND_MIN 19
#define PUL_STAND 10
#define PUL_STANDTIME 70 //0.8S内一个脉冲的发烟基准之一(>=0.7s)
#define PUL_COMFORT_STANDNUM 5  //
#define PUL_LOW_STANDNUM 6 //低温基准
#define PUL_HIGH_STANDNUM 1 //高温基准
#define PUL_NORMAL 10 //补偿阶段正常脉冲数
#define LOW_CONTIN 2//连续两次低温，则轮空

#define PULSE_NUM 0  //temper_seq[][]  一维取值
#define PULSE_ID 1

#define ISSTOP 1  //烟管停止发烟标志
//#define ISRUN 0
#define NORMAL 0  //管子正常状态
#define TOO_LOW 1 //管子低温状态
#define TOO_HIGH 2//管子高温状态

/***************全局变量声明******************/
extern bit wendu_one;
extern bit dianhuo_one;
extern bit dianhuo_success_flag;
extern bit set_up_flag;
extern bit system_ON_flag; 
extern bit dianhuo_flag;
extern bit start_wendu_flag;
extern bit start_loop_flag;
extern uchar count_2s;
extern uchar mc_fail_num;
extern uchar dianhuo_fail_num ; 
extern uint mc_data;
//extern uchar heat_coef;

//extern uchar tube_mc[];
extern uchar tube_time[];
//extern float coeficient[];
//extern uchar heating_time[];
extern uchar bdata isStop;
extern bit tube1_stop;
extern bit tube2_stop;
extern bit tube3_stop;
extern bit tube4_stop;

//*****************函数声明********************
void timer0_config();//定时器初始化
void pipe_choose(uchar num);//发烟管导通选择
void state_config();//状态初始化
void system_ON();//系统关闭
void dianhuo_ON();//点火
void upper_limit();//温度上限限制
bit receive_check(uchar str[],uchar num);//接收数据和校验
//void state_array_handle();//反馈数组处理
void tubeAltern(uchar num,uchar mode,bit state);
void tubeStopSet(uchar num,bit val);
bit tubeStopRead(uchar num);
void pulseDetection(uchar cnt,uchar time);
uint oiltubeOpen(float pulse_num);
void detectStep(uchar tube,bit state,bit step);
uchar calcMax(uchar dat[],uchar len);
uchar calcMin(uchar dat[],uchar len);
float calcAver(uint dat[],uchar num,uchar max_i,uchar min_i);
void dataSort(uchar dat[],uchar record_idx[]);
uchar recordLowTubeID(uchar dat[],uchar record_dat[],uchar dat_len,uchar rec_len);
//void compensateTime(float ave);

#endif
