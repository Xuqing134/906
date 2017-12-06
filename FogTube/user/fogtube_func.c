#include "fayan.h"

/************************************************
				???0???
************************************************/	
	void timer0_config()			
	{
		AUXR |= 0x80;		//?????1T??
		TMOD &= 0xF0;		//???????
		TL0 = 0x33;		//???????
		TH0 = 0x53;		
		TF0 = 0;		//??TF0(??????)
		TR0 = 1;		//?????
		
		EA = 1;			//????
		ET0 = 1;		//????0??(??2ms??)
		PX1 = 1;		//????1????1
		PT0 = 0;		//???0????0
		PS = 0;			//??1????0
		IE2 &= 0x76;	//?ES2(??2?????)???0??ES3(??3?????)???0???4
		IE2 |= 0x09;	//?ES2?1,???2????ES3?1,???3?????4
		ES = 1;//???1??		
	}	
/************************************************
				???????
************************************************/	
	void pipe_choose(uchar num)
	{
//		static uchar pipe_num;
//		pipe_num++;
//		if(pipe_num >= 5)
//			pipe_num = 1;
		
		switch(num)	//12????,????????????????
		{
			case 0:yanyou1 = 1;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;break;
			case 1:yanyou2 = 1;yanyou1 = 0;yanyou3 = 0;yanyou4 = 0;break;
			case 2:yanyou3 = 1;yanyou1 = 0;yanyou2 = 0;yanyou4 = 0;break;
			case 3:yanyou4 = 1;yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;break;
            default : break;
		}
	}
/************************************************
				???????
************************************************/	
	void state_config()
	{
		yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;	
		qiti = 0;dianhuo1 = 0;dianhuo2 = 0;shuibeng = 0;					//???,????	
		wendu_one = 0;//??,????
		dianhuo_one = 0;//??,????
		dianhuo_success_flag = 0;
	}
/************************************************
				????
************************************************/	
	void system_ON()
	{
		set_up_flag = 0;//?????
		system_ON_flag = 1;//????
		mc_data = 0;//????
		wendu_one = 1;//????
		dianhuo_one = 1;//????		
		
		dianhuo_flag = 0;//????		
		start_wendu_flag = 0;//??????????
//		gongzuo_flag = 0;//????????////////////////////gongzuo_flag = 1;
		dianhuo_success_flag = 0;//??????
		start_loop_flag = 0;//??????
		
		dianhuo_fail_num = 0;//??????
		mc_fail_num = 0;//??????
//		state_receive1 = 0x00;//??????///////////////////////////////
	}
	
/************************************************
				???????
************************************************/		
	void dianhuo_ON()
	{
		dianhuo1 = 1;		
		dianhuo2 = 1;	//??
		qiti = 1;
        shuibeng = 1;/////////////////////???
		count_2s = 0;			//???
		wendu_one = 1;//????,????5???
        
	}
/************************************************
				??????
	??:????????130%,??????
		  ????????????,????
************************************************/
/*	void upper_limit()
	{
		if((wendu_data1 >= dianhuo_wendu_receive*1.3)&&(wendu_up_flag))
		{						//??????????130%?,?????
			qiti = 0;//??
			wendu_limit_flag = 1;
		}
		else if((wendu_data1 <= dianhuo_wendu_receive)&&(wendu_limit_flag))
		{							//?????????,???
			dianhuo_flag = 1;//??,??
			fayan_10s_flag = 1;//??30s??
			wendu_limit_flag = 0;
			wendu_up_flag = 0;
			count_10s = 0;//??10s??
		}
		else if(fayan_10s_flag)
		{							//30?
			if(count_10s >= 300){
				fayan_10s_flag = 0;//??30s??
				wendu_up_flag = 1;
			}
		}
//        	if(count_10s >= 300){/////////////////////////////////
//				fayan_10s_flag = 0;//??30s??
//				wendu_up_flag = 1;
//			}
	}*/
    /////////////////////////////////
/************************************************
				??????
	??:????????120%,????????
		  
************************************************/
    /*void upper_limit()
	{
        if(wendu_data1 >= dianhuo_wendu_receive*1.3)
        {
            if(cyc_fayan <= (CYC_TOTAL - 4))
            {
                cyc_fayan += 4;
            }
            else
            {
                cyc_fayan = CYC_TOTAL;
            }
        }
        else if(wendu_data1 >= dianhuo_wendu_receive*1.2)
        {
            if(cyc_fayan <= (CYC_TOTAL - 2))
            {
                cyc_fayan += 2;
            }
            else
            {
                cyc_fayan = CYC_TOTAL;
            }
        }
        else if(wendu_data1 <= dianhuo_wendu_receive*0.8)
		{							//?????????,???????
			if(cyc_fayan >= 4)
            {
                cyc_fayan -= 4;
            }
		}
		else 
		{
            if(cyc_fayan >= 2)
            {
                cyc_fayan -= 2;
            }
		}
	}*/
    /////////////////////////////////
    
//	void dianhuo_check()
//	{
//		if((wendu_data1 > wendu_chushi1)&&(wendu_data1 - wendu_chushi1 >= 20))
//			return 1
//			
//	}
	
/************************************************
			??????
	??:????????????
************************************************/
	bit receive_check(uchar str[],uchar num)
	{
		uchar n,check = 0;
		if((str[0]==0x5a)&&(str[1]==0xa5)&&(str[2]==0x10))//????
		{
			for(n=0;n<num-1;n++)
				check+=str[n];	
			if((str[3]==num)&&(str[num-1]==check))//?????
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}

/*	
	void state_array_handle()
	{
		static uchar i = 0;
		static uchar k = 0;
		if(state_type != state_array[i])
		{
			i++;
			state_array[i] = state_type;
		}			
		if(send_finish_flag)
		{
			send_finish_flag = 0;
			for(k=0;k<11;k++)
				state_array[k] = state_array[k+1];
			state_array[11] = 0;
			i--;
		}	
	}
*/	
/**********************************
    管子通断选择
***********************************/
void tubeAltern(uchar num,uchar mode,bit state){
    if(mode == 0){
        switch(num){
            case 0: yanyou1 = 1;yanyou2 = 0;yanyou3 = 0;yanyou4 = 0;break;
            case 1: yanyou1 = 0;yanyou2 = 1;yanyou3 = 0;yanyou4 = 0;break;
            case 2: yanyou1 = 0;yanyou2 = 0;yanyou3 = 1;yanyou4 = 0;break;
            case 3: yanyou1 = 0;yanyou2 = 0;yanyou3 = 0;yanyou4 = 1;break;
            default : break;
        }
    }
    else if(mode == 1){
        if(state){
            switch(num){
                case 0: yanyou1 = 1;break;
                case 1: yanyou2 = 1;break;
                case 2: yanyou3 = 1;break;
                case 3: yanyou4 = 1;break;
                default : break;
            }
        }
        else{
            switch(num){
                case 0: yanyou1 = 0;break;
                case 1: yanyou2 = 0;break;
                case 2: yanyou3 = 0;break;
                case 3: yanyou4 = 0;break;
                default : break;
            }
        }
    }
}
/**********************************
    写入tube_stop位
***********************************/
void tubeStopSet(uchar num,bit val){
    
    switch(num){
        case 0: tube1_stop = val;break;
        case 1: tube2_stop = val;break;
        case 2: tube3_stop = val;break;
        case 3: tube4_stop = val;break;
        default : break;
    }
    
}
/**********************************
    读取tube_stop位
***********************************/
bit tubeStopRead(uchar num){
    bit val;
    
    switch(num){
        case 0: val = tube1_stop;break;
        case 1: val = tube2_stop;break;
        case 2: val = tube3_stop;break;
        case 3: val = tube4_stop;break;
        default : break;        
    }
    return val;
}

/**********************************
    1.5S内检验每个管子的脉冲数
***********************************/
void pulseDetection(uchar cnt,uchar time){
    bit finish_sign = 0;
    
    /*if(time < 5){//0.5s
        yanyou1 = 1;yanyou2 = 1;yanyou3 = 1;yanyou4 = 1;
        mc_data = 0;
    }
    else */
    if(time < DETECT_TIME){
        tubeAltern(cnt,0,0);
        /*if(time < 2){
            mc_data = 0;
        }*/
    }
    else {
//        time = 0;
        finish_sign = 1;
//        cnt ++;
    }
    
//    return finish_sign;
}

/**********************************
    脉冲数映射加热时间 /100
***********************************/
/*uint oiltubeOpen(float pulse_num){
    uint heat_time = 0;  //执行阶段的加热时间
//    heat_time = EXECUTE_TIME/40 * pulse_num;
    heat_time = (uint)(EXECUTE_TIME * (double)pulse_num/heat_coef);
    if(heat_time >= EXECUTE_TIME){
        heat_time = EXECUTE_TIME;
    }
    else {
        
    }
//    heat_time = 80;////////////
    
    return heat_time;
}/*

/**********************************
    step : 0--0.8s , 1--0.5s
***********************************/
void detectStep(uchar tube,bit state,bit step){
    uchar i = 0;
    
    if(!step){  //0.8s
        tubeAltern(tube,1,~state);
    }
    else{
        for(i = 0;i < TUBENUM;i ++){
            tubeAltern(i,1,(~tubeStopRead(i)));
        }
    }
}
/**********************************
    计算最大值的下标
***********************************/
uchar calcMax(uchar dat[],uchar len){
    uchar i = 0;
    uchar max_i = 0;
    uint temp = 0;
    
    temp = dat[0];
    for(i = 0;i < (len-1);i ++){
        if(temp < dat[i+1]){
            max_i = i + 1;
            temp = dat[i+1];
        }
        else {
//            max_i = i;
//            temp = dat[i];
        }
    }
    
    return max_i;
}
/**********************************
    计算最小值的下标
***********************************/
uchar calcMin(uchar dat[],uchar len){
    uchar i = 0;
    uchar min_i = 0;
    uint temp = 0;
    
    temp = dat[0];
    for(i = 0;i < (len-1);i ++){
        if(temp > dat[i+1]){
            min_i = i + 1;
            temp = dat[i+1];
        }
        else {
//            min_i = i;
//            temp = dat[i];
        }
    }
    
    return min_i;
}
/**********************************
    计算平均值
***********************************/
float calcAver(uint dat[],uchar num,uchar max_i,uchar min_i){
    uint sum = 0;
    float ave = 0;
    uchar i = 0;
     
    for(i = 0;i < num;i ++){
        if((i != max_i) || (i != min_i)){
            sum += dat[i];
        }
    }
    ave = sum / (num-2);//去掉最高和最低
    
    return ave;
}
/**********************************
    对数组排序(由低到高)--记录下标 
***********************************/
void dataSort(uchar dat[],uchar record_idx[]){
    uchar i = 0;
    uchar j = 0;
    uchar temp = 0;
    uchar temp_id = 0;
    uchar min_id = 0;
    
    for(i = 0;i < TUBENUM;i ++){
        record_idx[i] = i;
    }
    for(i = 0;i < TUBENUM;i ++){
        if(i < (TUBENUM - 1)){
//            record_idx[TUBENUM-1-i] = calcMax(dat,TUBENUM-i);
            min_id = calcMin(dat,TUBENUM-i);
            temp = dat[TUBENUM-1-i];//记录下被取代位置的数组元素的值
//            dat[TUBENUM-1-i] = dat[record_idx[TUBENUM-1-i]];
            dat[TUBENUM-1-i] = dat[min_id];
            dat[min_id] = temp;
            temp_id = record_idx[TUBENUM-1-i];
            record_idx[TUBENUM-1-i] = record_idx[min_id];
            record_idx[min_id] = temp_id;
        }
    }
}
/**********************************
    记录最低温和次低温管子_id
***********************************/
uchar recordLowTubeID(uchar dat[],uchar record_dat[],uchar dat_len,uchar rec_len){
    bit finish = 0;  //记录完成置1
    uchar i = 0;
    uchar j = 0;
    uchar fog_num = 0; //记录可以发烟管子的个数
    
    while(i <= dat_len){
        if(i < dat_len){
            if(!tubeStopRead(dat[i])){  //为可以发烟检测状态
                record_dat[0] = dat[i];
                j = i + 1;
                while(j <= dat_len){
                    if(j < dat_len){
                        if(!tubeStopRead(dat[j])){
                            record_dat[1] = dat[j];
                            fog_num = dat_len - j;
                            finish = 1;
                            break;
                        }
                    }
                    else if(j == dat_len){
                        fog_num = 1;
                        finish = 1;
                        break;
                    }
                    j ++;
                }
            }
        }
        else if(i == dat_len){
            fog_num = dat_len - i;
            break;
        }
        if(finish){
            break;
        }
        i ++;
    } 
    
//    return low_id;
    return fog_num;
}
/**********************************
    计算加热补偿时间
***********************************/
/*void compensateTime(float ave){
    uchar i = 0;
//    float ave = 0;
    float coef = 0;
    float com_pul = 0;//调节之后的脉冲数
    float oil_time = 0;//调节后的加热时间
    
//    ave = calcAver(tube_mc,TUBENUM,max_idx,min_idx);
    for(i = 0;i < TUBENUM;i ++){ //计算补偿后的脉冲数--pulse*(pulse/ave)
        coef = (tube_mc[i] + 1)/ ave;
        com_pul = (tube_mc[i] + 1) * coef * coef;
        oil_time = oiltubeOpen(com_pul);
        heating_time[i] = (uchar)((oil_time * coeficient[i]) / EXECUTE_SEG/10);
        if(heating_time[i] >= EXECUTE_TIME / EXECUTE_SEG){
            heating_time[i] = EXECUTE_TIME / EXECUTE_SEG;
        }        
    }
}*/
