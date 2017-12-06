#include "fayan.h"

uchar bdata isStop;
sbit tube1_stop = isStop^0;  //tube1在工作周期停止发烟则置1
sbit tube2_stop = isStop^1;
sbit tube3_stop = isStop^2;
sbit tube4_stop = isStop^3;

//*****************定义接收数据变量********************
uchar average_mc = 0X19;//平均脉冲数，2C：200ml; 23:150ml
//uchar cycle_receive = 40;//接收的导通周期
uchar cyc_fayan = CYC_TOTAL/2;//周期中发烟初始化时间
uint dianhuo_wendu_receive = 1200;//接收的系统上电点火温度
uint flow_receive = 0;//接收的流量数据
uchar state_receive1 = 0;//接收的烟剂开关状态

//*****************定义系统相关变量********************
bit wendu_one = 0;
bit dianhuo_one = 0;
bit start_wendu_flag = 0;//进入预设燃烧时长标志
bit gongzuo_flag = 0;//是否允许进入发烟状态标志
bit dianhuo_flag = 0;//是否允许点火标志
bit dianhuo_success_flag = 0;//点火成功标志
bit start_loop_flag = 0;//恒温循环
bit system_ON_flag = 0;//系统开启或关闭，默认关闭
bit finish_flag = 0;
bit wait_OFF_flag = 0;
bit wendu_limit_flag = 0;
bit fayan_dianhuo_flag = 0;
bit fly_flag = 1;//是否允许起飞
bit set_up_flag = 0;//关闭初始化
bit down_wendu_flag = 0;//发烟过程中，若温度过低，则关闭水泵////////////////////
bit count_10s_flag = 1;//开机时恒温点火时，10s燃烧
bit fayan_10s_flag = 0;//关闭发烟时，恒温点火时，10s燃烧
bit wendu_up_flag = 1;
bit wait_OFF_one = 1;//
bit dianhuo_ok = 0;//只有当发送完点火标志后，才允许点火
//bit send_finish_flag = 0;//发送完成标志
bit gongzuo_check = 0;
bit detect_finished = 0; //检测阶段完成
bit detect_sign = 1;//检测阶段置1(1.5*4s)
bit execute_step = 0;  //0--0.8s单管，1--0.5s高温管通
bit execute_sign = 0;//执行阶段置1(8s)
bit execute_buf_sign = 0;//执行阶段的缓冲区
bit compensate_step = 0;//补偿阶段置1
bit preparesign = 0; //开始点火之前的检测标志
bit buf_step = 0;  //0.5s4管停止发烟
bit high_temper_sign = 0;  //有超高温管子置1
bit low_temper_sign = 0;

uchar state_type = 0x00;//反馈的状态类型
uchar state_dianhuo = 0x00;//反馈的点火状态
uchar dianhuo_fail_num = 0;//点火失败计数
uchar mc_fail_num = 0;//脉冲检测计数
uchar tubeCnt_detect = 0;  //记录detect_step中的烟管数
uchar tubeCnt_execute = 0;  //工作周期选中的tube  
uchar low_tube = 0;//对最低温和次低温管计数
//uchar seg_cnt = 0;  //记录执行阶段的分段
//uchar max_idx = 0;
//uchar min_idx = 0;
//uchar heat_coef = 20;
uchar high_tube = 0; //记录执行周期可以发烟管子的个数

uint wendu_data1 = 0;//管子1当前温度
uint wendu_check1 = 0;
uint wendu_data2 = 0;//管子2当前温度
uint wendu_data3 = 0;//管子3当前温度
uint wendu_data4 = 0;//管子4当前温度
uint wendu_chushi1 = 0;//保留的第一次测量温度

//******************定义流量相关变量*******************
bit ON_flag = 0;//开启烟剂标志
uint mc_data = 0;//脉冲数

//*****************定义时间相关变量********************
uchar count_10ms = 0;
uchar count_100ms = 0;//100ms循环定时
uchar detect_time = 0;//检测阶段定时
uchar execute_time = 0;//执行阶段定时
uchar execute_time0 = 0;//记录0.8s
uchar execute_time1 = 0;
uchar execute_buf_time = 0;
uchar buf_time = 0;
uchar compensate_time = 0;//补偿阶段计时
uchar cycle_time = 0;//当前周期时间计数
uchar count_2s = 0;//2s点火时间
uchar check_5s = 0;//5s点火成功检测
uchar clean_time_count = 0;//清洁导通时间计
uchar wendu_limit_time = 0;//温度数据过滤过长，则弃该次数据
uchar prepare_time = 0;
uchar count_10s = 0;//燃烧10s定时
uchar pul_time = 0; //计数脉冲长度
uint wait_OFF_count = 0;//关闭发烟后，在设定时间内没有发烟，则关闭系统

//*****************定义数组****************************
uchar lowtemper_id[2] = {0};//记录符合发烟条件的最低温和次低温管子_id [0]--the most
uchar tube_time[TUBENUM] = {0};//记录烟管在执行阶段分段后的发烟时间
uchar tube_state[TUBENUM] = {0};  //记录管子状态
//float coeficient[TUBENUM] = {10,10,10,10};//发烟时间的补偿系数
//uchar coe_delta[TUBENUM] = {1,1,1,1}; //补偿系数步增值
//uchar heating_time[TUBENUM] = {0};//记录分段后的加热时间
//uchar reload_heat[TUBENUM] = {0};
uchar pulse_time[TUBENUM] = {0};//记录一个脉冲的时间
uint xdata wendu_array[2] = {0,0};
uint xdata wendu_check_array[2] = {0,0};
uchar xdata receive_array[10] = {0};//接收数据数组
uchar xdata send_array[15] = {0x5a,0xa5,0x10,0x0F,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//发送的系统状态
//uchar xdata state_array[12] = {0};
    uchar high_cnt[TUBENUM] = {2,2,2,2};
    uchar high_cnt1[TUBENUM] = {0};

uchar open_cnt = 0;
//uchar open_time[TUBENUM] = {0};///////////////临时记录执行周期各管子发烟的时间  --  x*0.5s

void main()
{
    uchar i;
    bit temper_test = 0;//
    bit first_execute = 0; //第一次执行阶段标志
//    bit ignitesign = 0;  //5s内可以点火，置1
    bit fuming_sign = 0;   //发烟状态是置1
    uchar fayan_cnt = 0;//////////
    uchar high_temper_cnt = 0; //连续两次补偿周期都为高温
    uchar low_temper_cnt = 0;
    uchar low_temper_cnt1 = 0;
    uchar pipe_num = 0; //清洗模式下管子的选择
    float average = 0;
    float coef_delta = 0;
    uchar low_cnt[TUBENUM] = {0};//记录管子连续低温的次数
    uchar tube_mc[TUBENUM] = {0};//记录烟管的脉冲数(0.8s内)
    uchar temper_seq[TUBENUM] = {0,1,2,3}; //记录管子温度次序（由高到低）--0:pulse 1:id
    
	P0M1 = 0x00;	P0M0 = 0x00;	//设置为准双向口
	P1M1 = 0x00;	P1M0 = 0x00;	//设置为准双向口
	P2M1 = 0x00;	P2M0 = 0xff;	//设置为推挽;P26,P27为高阻输入
	P3M1 = 0x00;	P3M0 = 0x00;	//设置为准双向口
	P4M1 = 0x00;	P4M0 = 0x00;	//设置为准双向口
	P5M1 = 0x00;	P5M0 = 0x00;	//设置为准双向口
	P6M1 = 0x00;	P6M0 = 0x00;	//设置为准双向口
	
	WDT_CONTR = 0x05;			//64分频，约1.1s
	WDT_CONTR |= 0x20;			//启动看门狗
	
    look = 0;
	state_config();//系统初始化
	timer0_config();//开定时器，设置中断	
	UART_init();//开串口
    IT0 = 1;//下降沿触发
    EX0 = 1;//开外部中断/////////////////
	SCL=1;SDA=1;_nop_();
	_nop_();_nop_();_nop_();
	SCL=0;
	delay(10000);	
	SCL=1;
	while(1)
	{
		WDT_CONTR = 0x35;	//喂狗
		REN = 1;
		wendu_limit_time = 0;
		while(1)//温度数据滤波外理
		{
			wendu_check1 = (memread()*0.02-273.15)*10;
			wendu_check_array[1] = wendu_check_array[0];
			wendu_check_array[0] = wendu_check1;
			if(wendu_check1 < 10000){
				if(abs(wendu_check_array[0] - wendu_check_array[1]) <= 100)
				{
					wendu_data1 = wendu_check1;
					break;
				}
			}
			else if(wendu_limit_time >= 2)//在一定时间内，若没有新的数据，刚跳出循环，保留前一个有效数据
				break;
		}
//		dianhuo1 = 1;qiti=1;
		if(system_ON_flag)
		{
			if(dianhuo_one)			//上电点火				
			{
				state_type = 0x01;//系统开启
				dianhuo_flag = 1;
				dianhuo_one = 0;		//清除上电点火标记
//				start_wendu_flag = 1;	//允许进入预设燃烧温度
				count_10s = 0;//重置10s计数
			}
			if(dianhuo_flag)//是否开启点火
			{	
				dianhuo_flag = 0;
				state_dianhuo = 0x0b;//反馈点火，只有返回OK时才能点火
			}
//			state_array_handle();//反馈数组处理
			if(dianhuo_ok)
			{
                preparesign = 1;
                shuibeng = 1;
                yanyou1 = 1;yanyou2 = 1;yanyou3 = 1;yanyou4 = 1;
                if(mc_data >= 40){
                    prepare_time = 0;
                    preparesign = 0;
                    if(prepare_time <= PREPARE_TIME){
//                        ignitesign = 1; 
                        mc_data = 0;
                        
                        dianhuo_ok = 0;				
                        dianhuo_ON();//开始点火
                        dianhuo_success_flag = 1;//是否点火成功
                        check_5s = 0;//点火后，开启5s点火检测                        
                    }
                    /*else{
                        prepare_time = 0;
                    }*/
                    yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;
                }
                else if(prepare_time >= PREPARE_TIME){  //5s内没有对应脉冲数，不点火
                    state_type = 0x03;
                    if((prepare_time > (PREPARE_TIME + 2))){
                        IAP_CONTR = 0x20;//系统关闭	
                    }
                }
			}			
			
			if(wendu_one)			//取温度初值,并计算点火预设时间
			{
				wendu_chushi1 = wendu_data1;
				wendu_one = 0;	
			}
			if(count_2s >= dianhuo_time)			//2秒后关闭点火（想要点火2s，仅需将count_2s=0）
			{
				dianhuo1 = 0;
				dianhuo2 = 0;
				count_2s = 20;
			}	
			if(check_5s >= check_time)//5秒循环温度检测
			{
				wendu_array[1] = wendu_array[0];//更新数组里的值
				wendu_array[0] = wendu_data1;
				check_5s = 0;
				if(qiti)//当气阀开启时，每5秒要进行火焰检测
				{
					if((wendu_data1 >= dianhuo_wendu_receive*0.7)&&(fly_flag))//允许起飞温度
					{								//当温度低于预设温度的70%时，则提示可以起飞
						state_type = 0x02;
						fly_flag = 0;
					}
					if(dianhuo_success_flag)//点火成功检测
					{ 
						////////////           ///////////////
                        if(wendu_data1 <= wendu_chushi1)
						{
                            state_type = 0x05;//发送3次点火失败信号
							dianhuo_fail_num++;
							dianhuo_flag = 1;//否则再次点火	
						}
						else if((wendu_data1 > wendu_chushi1)&&(wendu_data1 - wendu_chushi1 <= 20))
						{
                            state_type = 0x05;//发送3次点火失败信号
							dianhuo_fail_num++;
							dianhuo_flag = 1;//否则再次点火	
						}
						else
						{ 
							dianhuo_success_flag = 0;//点火成功，关闭15秒点火
							dianhuo_fail_num = 0;//点火成功，清零失败计数
                            gongzuo_flag = 1;//////////////////////////////
						}
                        /////////////////////////
							/////////////////////dianhuo_success_flag = 0;//点火成功，关闭15秒点火
							//////////////////dianhuo_fail_num = 0;//点火成功，清零失败计数
                        
						if(dianhuo_fail_num > 2)//点火三次不成功，则关系发烟系统
						{
//							state_type = 0x05;//发送3次点火失败信号
							IAP_CONTR = 0x20;//系统关闭	
						}					
					}
					//////////////////               /////////////////
                    else if(gongzuo_check)//工作状态温度检测
					{									//当温度下降到预设值的80%时，则关闭发烟系统
//						if(wendu_data1 < dianhuo_wendu_receive*0.7)////////////////
                        if(wendu_data1 < 400)
						{
							state_type = 0x04;//燃烧用完
							IAP_CONTR = 0x20;//系统关闭	
						}
					}
					else if((wendu_array[1] > wendu_array[0])&&(wendu_array[0]-wendu_array[1] >= 100))
					{						//当连续三次最新一次的温度比前一次测量值低10度时，则关闭发烟系统
						dianhuo_fail_num++;
                        state_type = 0x04;
						if(dianhuo_fail_num > 2)
						{
							state_type = 0x04;//燃烧用完
							IAP_CONTR = 0x20;//系统关闭
						}
					}
					else dianhuo_fail_num = 0;
                    ////////////////////////////////
				}
				else dianhuo_fail_num = 0;
			}
			
			if((wait_OFF_count >= wait_OFF_time)&&(wait_OFF_flag))
				IAP_CONTR = 0x20;		//系统关闭
		}
		else
			state_type = 0x07;//提示系统关闭
		if(gongzuo_flag)      /////////////////////允许发烟
		{
			if(state_receive1 == 0x33)//开启发烟
			{
                gongzuo_check = 1;
				wait_OFF_one = 1;
				set_up_flag = 1;//初始化,在发烟过程中可以改变烟油的量
				start_loop_flag = 0;
				start_wendu_flag = 0;//关闭恒温循环点火
				wait_OFF_flag = 0;//开启发烟后，关闭在设定时间内没有发烟，则关闭系统
				if((qiti == 0)&&(fayan_dianhuo_flag))
				{
					fayan_dianhuo_flag = 0;
					dianhuo_flag = 1;
				} 
				if(wendu_data1 >= (dianhuo_wendu_receive + 10)){
					finish_flag = 1;
                    temper_test = 1;//////////
                    fuming_sign = 1;
                    if(fuming_sign){
                        fuming_sign = 0;
                        state_type = 0x08;//反馈的正在发烟
                    }
				}
                if((finish_flag)&&(down_wendu_flag)){
//                    state_type = 0x08;//反馈的正在发烟
                    shuibeng = 1;
                    
                    if(buf_step){ //缓冲阶段
                        if(buf_time < BUF_TIME){
                            detect_sign = 0;
                            execute_sign = 0;
                            yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;
                        }
                        else{//进入检测阶段
                            buf_time = 0;
                            buf_step = 0;
                            detect_sign = 1;
                        }
                    }
                    if(detect_sign){//检测阶段
                        execute_sign = 0;
                        buf_step = 0;
                        if(detect_time >= DETECT_TIME){
//                           detect_time = 0; 
                            detect_finished = 1;
                        }
                        else if(detect_time < 4){//前0.4s不数脉冲
                            mc_data = 0;
                        }
                        if(!detect_finished){
                            if(low_cnt[i] > LOW_CONTIN){  //连续两个周期轮空，不执行检测周期
                                tubeAltern(tubeCnt_detect,0,0);
                            }
                            else{
                                tubeAltern(tubeCnt_detect,0,1);  //打开No.tubeCnt_detect--按管子固定次序
                            }
                        }
                        else{ //到0.8 s
                            tubeCnt_detect ++;
                            if(tubeCnt_detect <= TUBENUM){
                                if(low_cnt[i] <= LOW_CONTIN){
                                    tube_mc[tubeCnt_detect-1] = mc_data;
                                }
                                else{
                                    tube_mc[tubeCnt_detect-1] = PUL_LOW_STANDNUM;
                                }
                                mc_data = 0;
                                if(tubeCnt_detect == TUBENUM){
                                    yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;
                                    isStop = 0; //清空轮空标志
                                    for(i = 0;i < TUBENUM;i ++){
                                        if(tube_mc[i] >= PUL_LOW_STANDNUM){
                                            high_cnt1[i] = 0;
                                            low_cnt[i] ++;
                                        }
                                        else {
                                            low_cnt[i] = 0;
                                            if(tube_mc[i] <= PUL_HIGH_STANDNUM){
                                                high_cnt1[i] ++;
                                            }
                                            else {
                                                high_cnt1[i] = 0;
                                            }
                                        }
                                        if(low_cnt[i] >= LOW_CONTIN){//连续两次以上都为低温管，则在工作周期轮空
                                            tubeStopSet(i,ISSTOP);
                                            low_temper_sign = 1;
                                            low_temper_cnt ++;
                                            if(low_cnt[i] > (LOW_CONTIN + 1)){
                                                low_cnt[i] = LOW_CONTIN;
                                            }
                                        }
                                        else {
                                            low_temper_sign = 0;
                                            low_temper_cnt = 0;
                                            if(high_cnt1[i] >= 2){ //有超高温管/堵塞管
                                                tubeStopSet(i,!ISSTOP);
                                                high_temper_sign = 1;
                                                state_type = 0x0c;
                                                tube_state[i] = TOO_HIGH;
                                                if(high_cnt1[i] >= 4){
//                                                if(high_temper_cnt > 2){
                                                    IAP_CONTR = 0x20;
//                                                }
                                                }
                                            }
                                            else{
                                                state_type = 0x08;
                                                tube_state[i] = NORMAL;
                                            }
                                        }
                                    }
                                    dataSort(tube_mc,temper_seq);//对管子温度进行排序
                                    high_tube = recordLowTubeID(temper_seq,lowtemper_id,TUBENUM,2);
                                    tubeCnt_detect = 0;
                                    detect_sign = 0;
                                    buf_step = 0;
                                    mc_data = 0;
                                    execute_step = 0;//先进入0.8s
                                    execute_sign = 1;
                                }
                            }
                            else{
                                tubeCnt_detect = 0;
                            }
                            detect_time = 0;
                            detect_finished = 0;
                        }
                    }
                    if(execute_sign){//执行阶段
                        if(execute_time < EXECUTE_TIME){//15s
                            detect_sign = 0;
                            buf_step = 0;
                            if(execute_time0 >= EXECUTE_TIME0){ //准备进入0.5s
                                //判断最低温管子是否要继续发烟
                                execute_time0 = 0;
                                if((pulse_time[lowtemper_id[low_tube]] == 0) || (pulse_time[lowtemper_id[low_tube]] < pul_time)){
                                    pulse_time[lowtemper_id[low_tube]] = pul_time;
                                }
                                pul_time = 0;
                                /***检测到最低温管子不符合高温发烟条件***/
                                if((pulse_time[lowtemper_id[low_tube]] < PUL_STANDTIME) && (mc_data >= PUL_COMFORT_STANDNUM)){
                                    tubeStopSet(lowtemper_id[low_tube],1);
                                    while(tubeCnt_execute < (TUBENUM-1)){
                                        tubeCnt_execute ++;
                                        if(!tubeStopRead(temper_seq[tubeCnt_execute+1])){
                                            lowtemper_id[low_tube] = temper_seq[tubeCnt_execute+1];
                                            break;
                                        }
                                    }
                                }
                                execute_step = 1;
                            }
                            else{  //在0.8s
                                
                            }
                            if(execute_time1 >= EXECUTE_TIME1){ //准备进入0.8s
                                
                                mc_data = 0;
                                low_tube ++;
                                if(low_tube >= 2){  //最低温管和次低温管轮流
                                    low_tube = 0;
                                }
                                for(i = 0;i < TUBENUM;i ++){
                                    pulse_time[i] = 0;
                                }
                                for(i = 0;i < TUBENUM;i ++){////////////////临时
                                    if(!tubeStopRead(i)){
//                                        open_time[i] ++;
                                        open_cnt ++;
                                    }
                                }
                                pul_time = 0;
                                execute_time1 = 0;
                                execute_step = 0;
                            }
                            else{  //在0.5s
                                
                            }
                            if(!execute_buf_sign){  //不在缓冲区
//                                detectStep(tubeCnt_detect,tubeStopRead(tubeCnt_detect),execute_step);
                                detectStep(lowtemper_id[low_tube],tubeStopRead(lowtemper_id[low_tube]),execute_step);
                            }
                            else{
                                yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;
                            }
                        }
                        else{//已完成执行阶段
                            execute_time = 0;
                            execute_time0 = 0;
                            execute_time1 = 0;
                            tubeCnt_execute = 0;
                            low_tube = 0;
                            execute_sign = 0;
                            execute_step = 0;
                            detect_sign = 0;
                            mc_data = 0;
                            buf_step = 0;
//                            isStop = 0;
//                            isStop = 0x0f;
                            yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;
                            
                            for(i = 0;i < TUBENUM;i ++){
                                /*if(open_time[i] >= 10){  //执行周期中10个以上0.5s都在发烟，判为高温管
                                    tubeStopSet(i,!ISSTOP);
                                    high_temper_sign = 1;
                                }
                                else if(open_time[i] <= 2){//执行周期中2个以下0.5s发烟，判为低温管
                                    tubeStopSet(i,ISSTOP);
                                    low_temper_sign = 1;
                                }
                                open_time[i] = 0;*/
                                if(!tubeStopRead(i)){
                                    high_temper_sign = 1;
                                }
                            }
                            if(open_cnt <= 3){
                                low_temper_cnt1 ++;
                                state_type = 0x0a;
                                if(low_temper_cnt1 == 2){
                                    IAP_CONTR = 0x20;
                                }
                            }
                            else {
                                state_type = 0x08;
                                low_temper_cnt1 = 0;
                            }
                            open_cnt = 0;
                            if((!high_temper_sign) && (!low_temper_sign)){
                                if(!high_temper_sign){
                                    high_temper_cnt = 0;
                                }
                                if(!low_temper_sign){
                                    low_temper_cnt = 0;
                                }
                                compensate_step = 0;
                                detect_sign = 1;
                            }
                            else{
                                compensate_step = 1;
                            }
                        }
                    }
                    if(compensate_step){  //补偿阶段
                        if(high_temper_sign){
                            if((compensate_time >= COMPENSATE_TIME1) || (mc_data >= PUL_NORMAL)){
                                if(mc_data < PUL_NORMAL){
                                    /*high_temper_cnt ++;
                                    state_type = 0x0c;
                                    if(high_temper_cnt > 2){
                                        IAP_CONTR = 0x20;
                                    }*/
                                    compensate_time = 0;
                                    compensate_step = 0;
                                    detect_sign = 1;
                                    yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;  
                                    mc_data = 0;
                                }
                                else{   //以有足够脉冲数
                                    if(low_temper_sign){   //高低温同时进入
                                        if(compensate_time >= COMPENSATE_TIME0){  //5s
                                            compensate_time = 0;
                                            compensate_step = 0;
                                            detect_sign = 1;
                                            yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;  
                                            mc_data = 0;  
                                        }
                                        else{              
                                            
                                        }
                                    }
                                    else{ //只有高温进入
                                        high_temper_cnt = 0;
                                        compensate_time = 0;
                                        compensate_step = 0;
                                        detect_sign = 1;
                                        yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;  
                                        mc_data = 0;
                                    }
                                    isStop = 0x0f;
                                } 
                                high_temper_sign = 0;
                            }
                            else{
                                /*for(i = 0;i < TUBENUM;i ++){
                                   if(high_cnt1[i] >= 1){
                                        detectStep(i,!ISSTOP,0);
                                   }
                                   else{
                                       low_cnt[i] = 0;
                                       detectStep(i,ISSTOP,0);
                                   }
                                }*/
                                detectStep(0,0,1);
                            }
                        }
                        else{ //只有低温进入,5s加热
                            if(compensate_time >= COMPENSATE_TIME0){
                                /*low_temper_cnt ++;
                                state_type = 0x04;
                                if(low_temper_cnt >= 2){
                                    IAP_CONTR = 0x20;
                                }*/
                                compensate_time = 0;
                                compensate_step = 0;
                                detect_sign = 1;
                                yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;  
                            }
                            else{
//                                detectStep(0,0,1);
                                isStop = 0x0f;
                            }
                        }
                    }
                }
                
				////////////                    ///////////////
//                if(wendu_data1 <= dianhuo_wendu_receive*0.88)//2017.8.19 20:31改。发烟过程中，温度低于预设值的83%，则关阀。
                if((wendu_data1 < dianhuo_wendu_receive*0.75) && (finish_flag))////////////////
                {
                    state_type = 0x04;//燃烧用完
                    if(wendu_data1 < dianhuo_wendu_receive*0.7){
                        IAP_CONTR = 0x20;//系统关闭	
                    }
                }
                if(wendu_data1 <= dianhuo_wendu_receive*0.85)
				{											  //直到温度达到预设的温度程序自动发烟。
                    if(down_wendu_flag)//////////////////
                    {
					state_type = 0x0a;//温度过低，关闭阀门。
					down_wendu_flag = 0;
//					shuibeng = 0;//关泵
					yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;//关阀
                    }
				}
				else if(wendu_data1 >= dianhuo_wendu_receive)//2017.8.19 20:31改
                {
					down_wendu_flag = 1;
                }
                ////////////////////////////////////////////
			}
			else if(state_receive1 == 0x34)//关闭发烟
			{
				gongzuo_check = 1;
				state_type = 0x09;//反馈的关闭发烟
				set_up_flag = 0;//关闭初始化
//				upper_limit();//温度上限限制
				
				mc_fail_num = 0;
				shuibeng = 0;//关泵
				yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;//关阀
				if(wait_OFF_one)
				{
					wait_OFF_one = 0;//防止再次进入
					wait_OFF_flag = 1;//开启在设定时间内没有发烟，则关闭系统
					wait_OFF_count = 0;//清零关烟过长计数
				}
			}			
		}
		else gongzuo_check = 0;
//		if(state_receive1 == 0x31)//系统开启/////////////////
        if(state_receive1 == 0x33)
		{
            pipe_num = 0;
            if(!system_ON_flag)////////////////
            {///////
                system_ON();
                wait_OFF_one = 1;
            }///////////
		}
		else if((state_receive1 == 0x35)&&(!system_ON_flag))//全开，清洗用
		{
            shuibeng = 1;
			wait_OFF_one = 1;
			//当系统关闭时，才可以进入清洗模式
			set_up_flag = 0;//关闭初始化
//			if(clean_time_count >= clean_time) //定时
            if(mc_data < CLEANPULSE)
			{
//				pipe_choose(pipe_num);//发烟管导通选择
                tubeAltern(pipe_num,0,0);
			}
            else{
				mc_data = 0;//清零清洗计时
                pipe_num ++;
                if(pipe_num >= TUBENUM){
                    pipe_num = 0;
                }
            }
		}
        
	}
}

/************************************************
				外部中断0中断
************************************************/	
	void INT0_Routine() interrupt 0
	{
//		IE0 = 0;
        mc_data ++;
        if(pulse_time[lowtemper_id[low_tube]] < pul_time){//取最大值
            pulse_time[lowtemper_id[low_tube]] = pul_time;
        }
        pul_time = 0;
	}

/************************************************
				定时器0中断
************************************************/		
	void timer0_Routine() interrupt 1
	{
        uchar i = 0;
		TF0 = 0;
		wendu_limit_time++;//温度数据过滤过长，则弃该次数据
        count_10ms ++;
		count_100ms++;			//100ms循环计时
        if((execute_sign) && (!detect_finished)){
            if(pul_time < 255){
                pul_time ++;
            }
        }
		if(count_100ms>=50)		//100ms定时到
		{
			count_100ms = 0;
            if(preparesign){
                prepare_time ++;
            }
            if(finish_flag){
                cycle_time ++;
                if(buf_step){
                    buf_time ++;
                }
                if(detect_sign){
                    detect_time ++;
                }
                if(execute_sign){
                    execute_time ++;
                    if(!execute_step){
                        execute_time0 ++;
                    }
                    else{
                        execute_time1 ++;
                    }
                }
                if(compensate_step){
                    compensate_time ++;
                }
                if(cycle_time > CYC_TOTAL)
                {
                    cycle_time = 0;
                }
            }
			count_2s++;//点火2s计数
			check_5s++;//5s点火成功检测 
			clean_time_count++;//清洁导通时长计数		
			wait_OFF_count++;//1分钟后没有发烟指令，则关闭整个系统
			count_10s++;
		}	
	}
		
/************************************************
				串口1中断程序
		1、发送数据给上位机
		2、接收上位机下发的命令及设定值
************************************************/		
	void UART1_Routine() interrupt 4
	{				
		uchar i = 0;
        uchar count,h;
		uint num;
		if(RI)
		{
			RI = 0;
			receive_array[count++] = SBUF;	
            if(count >= 2){
                if((receive_array[count-2]==0x5a) && (receive_array[count-1]==0xa5))
                {
                    count = 2;
                    receive_array[0] = 0x5a;
                    receive_array[1] = 0xa5;
                }
            }
			if((count == 6)&&(receive_check(receive_array,count)))//发烟反馈
			{
				count = 0;
				REN = 0;
				if(state_dianhuo == 0x0b){
					send_array[5] = 0x0b;//当有点火命令时，则先发送点火命令
					dianhuo_ok = 1;	
				}
				else{
					send_array[5] = state_type;//将系统状态储存到数组中来
//					send_array[5] = state_array[1];//将系统状态储存到数组中来
//					send_finish_flag = 1;//发送完成标志
				}
                if(state_type == 0x0c){
//                    send_array[6] = 0;
//                    send_array[7] = tube_state[0];
//                    send_array[8] = 0;
//                    send_array[9] = tube_state[1];
//                    send_array[10] = 0;
//                    send_array[11] = tube_state[2];
//                    send_array[12] = 0;
//                    send_array[13] = tube_state[3];
                    /*if(tube_state[0] == TOO_HIGH){
                        wendu_data1 = 0xffff;
                    }
                    else{
                        wendu_data1 = 0;
                    }*/
                    if(tube_state[1] == TOO_HIGH){
                        wendu_data2 = 0xffff;
                    }
                    else{
                        wendu_data2 = 0;
                    }
                    if(tube_state[2] == TOO_HIGH){
                        wendu_data3 = 0xffff;
                    }
                    else{
                        wendu_data3 = 0;
                    }
                    if(tube_state[3] == TOO_HIGH){
                        wendu_data4 = 0xffff;
                    }
                    else{
                        wendu_data4 = 0;
                    }
                }
                send_array[6] = wendu_data1%256;
                send_array[7] = wendu_data1>>8;
                send_array[8] = wendu_data2%256;
                send_array[9] = wendu_data2>>8;
                send_array[10] = wendu_data3%256;
                send_array[11] = wendu_data3>>8;
//                  send_array[10] = receive_array[1];
//                  send_array[11] = mc_data;
//                send_array[12] = high_cnt1[2];
//                send_array[13] = high_cnt1[3];
                send_array[12] = wendu_data4%256;
                send_array[13] = wendu_data4>>8;
                ///////////////////////
				/*send_array[8] = tube_mc[0];
				send_array[9] = tube_mc[1];
                send_array[10] = tube_mc[2];
                send_array[11] = tube_mc[3];
                send_array[12] = ((uchar)tube1_stop << 6) | ((uchar)tube2_stop << 4) | ((uchar)tube3_stop << 2) | tube4_stop;
                send_array[13] = pulse_time[0];
                send_array[14] = pulse_time[1];
                send_array[15] = pulse_time[2];
                send_array[16] = open_cnt;
                send_array[17] = compensate_time;
                send_array[18] = low_temper_cnt1;
                send_array[19] = high_temper_cnt;
                send_array[20] = low_temper_sign | (((uchar)high_temper_sign) << 4);
				send_array[21] = mc_data;
                send_array[22] = ((uchar)execute_step << 4) | detect_sign;*/
				for(h=0;h<14;h++)
					send_array[14] += send_array[h];//和校验
				putstr(send_array,15);//发送状态类型以及各个管子温度
				send_array[14] = 0;//清除上一次和校验数据
                
                /*for(h=0;h<23;h++)
					send_array[23] += send_array[h];//和校验
				putstr(send_array,24);//发送状态类型以及各个管子温度
				send_array[23] = 0;//清除上一次和校验数据*/
				for(h=0;h<=10;h++)
					receive_array[h] = 0x00;	
				state_dianhuo = 0x00;//清零点火标志
			}
			if((count == 8)&&(receive_check(receive_array,count)))//接收的数据
			{
				putChar(0xff);//返回一个接收成功信号
				REN = 0;
				state_receive1 = receive_array[4];//接收的烟剂开关状态
				if(state_receive1 == 0x32)//系统关闭
					IAP_CONTR = 0x20;		//软复位，系统重新从用户代码区开始运行程序
				
//				else if(state_receive1 == 0x33)
//				{
//					start_loop_flag = 0;
//					start_wendu_flag = 0;
//				}
				else if(state_receive1 == 0x36)
				{	
//					cycle_receive = receive_array[1];//接收的导通周期				
//					flow_receive = receive_array[5];//接收的流量数据
					dianhuo_wendu_receive = receive_array[6]*10;//接收的开机温度
                    /*for(i = 0;i < TUBENUM;i++){
                        coeficient[i] = receive_array[7+i];
                    }*/
					if(flow_receive == 150)
						average_mc = 0x19;//0x23;
					else if(flow_receive == 200)
					{
						num = flow_receive*10/30;//计算平均流量
						average_mc = (int)(0.7*num-3);		
					}
//					if(set_up_flag)
                    if(gongzuo_flag)
						state_receive1 = 0x33;
				}
				count = 0;//接收数组清零
				for(h=0;h<=10;h++)
					receive_array[h] = 0x00;				
			}
			if(count >= 9)
				count = 0;
		}		
	}

	
	