/*
 * delay.c
 *
 *  Created on: 2018年8月5日
 *      Author: Marshal
 */
#include "driver/delay.h"
#include "osapi.h"
/*
 * 函数名称：void DealyUs(unsigned int data)
 * 函数功能：微秒延时函数
 * 函数形参：unsigned int data：延时时间
 * */
void Delay1ms(unsigned int y)
{
	os_delay_us(y * 1000);
}






