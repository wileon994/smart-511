#include "remote.h"
#include "delay.h"
#include "lcd.h"
#include "led.h"
//#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//红外遥控解码驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved								  
//////////////////////////////////////////////////////////////////////////////////



//红外遥控初始化
//设置IO以及TIM3_CH3的输入捕获
void Remote_Init(void)    			  
{		
    GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
 
 	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能PORTB时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//TIM3 时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			// PB0 输入 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		// 上拉输入 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB, GPIO_Pin_0);	//初始化GPIOB0
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3); //GPIOB0复用为TIM3
						  
 	TIM_TimeBaseStructure.TIM_Period = 0xffff; 			//设定计数器自动重装值 最大131ms溢出  
	TIM_TimeBaseStructure.TIM_Prescaler = (176 - 1); 	//预分频器,2us加1.t = (arr+1)*(psc+1)/84000000	   	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;  // 选择输入端 IC3映射到TI3上
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
    TIM_ICInitStructure.TIM_ICFilter = 0x00;	//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
    TIM_ICInit(TIM3, &TIM_ICInitStructure);		//初始化定时器输入捕获通道
	
	TIM_ITConfig( TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);//允许更新中断 ,允许CC4IE捕获中断
    TIM_Cmd(TIM3, ENABLE ); 	//使能定时器3
	
	TIM_ClearFlag(TIM3, TIM_IT_CC3|TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
}



extern u32 IR_Key;
extern u8 flag;
u8 	IR_Sta = 0;	
u8 	IR_Up = 0;	
u32 IR_Code = 0;		// 红外接收到的数据  	  
u16 IR_ThisPulse;		// 下降沿时计数器的值
u16 IR_LastPulse;		// 下降沿时计数器的值
u16 IR_PulseSub;		// 下降沿时计数器的值
u16 LianfaCnt;			// 下降沿时计数器的值	   		    
u8  IR_PulseCnt = 0;	// 按键按下的次数	 

void TIM3_IRQHandler(void)
{ 		    	 
 
	if(TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		IR_Up = 0;								// 每次进入捕获中断就清除溢出次数计数值
		IR_ThisPulse = TIM_GetCapture3(TIM3);	// 获取通道3的捕获值
		
		if(IR_ThisPulse > IR_LastPulse)			// 这次捕获的值大于上次捕获的值
		{
			IR_PulseSub = IR_ThisPulse - IR_LastPulse;	// 得到时间差
		}
		else									// 小于时要加上0xffff
		{
			IR_PulseSub = 0xffff - IR_LastPulse + IR_ThisPulse;	// 得到时间差
		}
		IR_LastPulse = IR_ThisPulse;			// 将本次得到值作为下一次编码的前一个值
		IR_PulseCnt++;					// 解码位数加1
		if(IR_PulseCnt == 2)
		{
			if((IR_PulseSub > 6000) && (IR_PulseSub < 8000))	// 引导码范围，13.5ms 13.5ms/2us = 6750
			{
				IR_Sta = 0x01;			// 标志位为1， 表示引导码已经收到
			}
			else
				IR_PulseCnt = 0;		// 如果时间差没有在引导码范围内，中断次数清零，重新编码
		}
		if((IR_PulseCnt > 2) && (IR_Sta == 0x01))	// 中断次数大于2，并且引导码已经解完
		{
			IR_Code <<= 1;				// 存储红外编码的寄存器向左移一位， 以便于存储下一位放到最低位
			if((IR_PulseSub > 450) && (IR_PulseSub < 700))	// 编码‘0’范围， 1.125ms	1.125/2us = 562.5
			{
				IR_Code |= 0x00;		// 存储0
			}
			else if((IR_PulseSub > 800) && (IR_PulseSub) < 1300) // 编码‘1’范围， 2.25ms	2.25/2us = 1125
			{
				IR_Code |= 0x01;		// 存储1
			}
			else				// 如果不是0码也不是1码， 变量清零，重新解码
			{
				IR_Sta = 0;
				IR_Code = 0;
				IR_PulseCnt = 0;
			}
			
		}
		if(IR_PulseCnt == 34)		// 如果解完码， 第34次进入中断正好解完一帧码
		{
			IR_Key = IR_Code;		// 存放解出的码
			IR_Sta = 0x02;			// 进入连发码的状态
			flag = 1;				// 解完码标志位1，进行编码处理
		}
		if((IR_PulseCnt == 36) && (IR_Sta == 0x02)) // 2位引导码，32位数据码
		{
			IR_PulseCnt = 34;			// 按键不松手便认为他在34
			if((IR_PulseSub > 4500) && (IR_PulseSub < 6000))	// 进入连发状态， 11.5ms/2us = 5525
			{
				LianfaCnt++;
				IR_Key = IR_Code;
				flag = 1;
			}
		}		
	}
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	
		if(RDATA == 1)
		{
			IR_Up++;			// 按键已经抬起
			if(IR_Up >= 2)
			{
				IR_Code = 0;
				IR_Sta = 0;
				IR_PulseCnt = 0;
				LianfaCnt = 0;
			}
		}
	}
	
}
































