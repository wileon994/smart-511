/*************************************
实验现象：在液晶上显示“南通大学”，“WWW.NTU.EDU.CN”
*************************************/

#include "sys.h"
#include "lcd.h"
#include "delay.h"
#include "font.h"
#include "remote.h"
#include "led.h"

u32 IR_Key = 0;
u8 flag = 0;
u8 dat = 0;
u8 *str = 0;
int main()
{	
	
	delay_init(168);
	LCD_IO_ENABLE();
	LCD_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	LED_Init();
	xianshi();
 	Remote_Init();				//红外接收初始化		 	

	while(1)
	{  
		if(flag)
		{	 
			flag = 0;
			dat = (u8)(IR_Key >> 8);
			LCD_ShowNum(86,130,dat,3);		//显示键值		  
			switch(dat)
			{
				case 0xA2:str = " POWER ";break;			   
				case 0xE2:str = " MENU ";break;	    
				case 0x22:str = " TEST ";break;	    
				case 0x02:str = " +";break;		 
				case 0xC2:str = " RETURN";break;		  
				case 0xE0:str = " LEFT";break;	   
				case 0xA8:str = " STOP";break;		  
				case 0x90:str = " RIGHT";break;		  
				case 0x68:str = " 0";break;		   
				case 0x98:str = " -";break;		    
				case 0xB0:str = " C";break;		  
				case 0x30:str = " 1";break;	   
				case 0x18:str = " 2";break;	    
				case 0x7A:str = " 3";break;		    
				case 0x10:str = " 4";break;		    
				case 0x38:str = " 5";break;		  
				case 0x5A:str = " 6";break;			   					
				case 0x42:str = " 7";break;	 
				case 0x4A:str = " 8";break;
				case 0x52:str = " 9";break;		 
			}
			LCD_Fill(86, 150, 116+8*8, 170+16, WHITE);	//清楚之前的显示
			LCD_ShowString(86,150,str);	//显示SYMBOL
		}
	}
}

