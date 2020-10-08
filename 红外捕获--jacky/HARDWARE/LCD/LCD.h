#ifndef __LCD_H
#define __LCD_H
#include "sys.h"

#define LCD_REST PBout(1)
#define LCD_CS PDout(7)
#define LCD_RD PDout(4)
#define LCD_WR PDout(5)
#define LCD_RS PDout(13)
#define LCD_DataPortH(num) PEout(11)=(num&0x01);PEout(12)=(num>>1)&0x01;PEout(13)=(num>>2)&0x01;PEout(14)=(num>>3)&0x01;PEout(15)=(num>>4)&0x01;PDout(8)=(num>>5)&0x01;PDout(9)=(num>>6)&0x01;PDout(10)=(num>>7)&0x01;
#define LCD_DataPortL(num) PDout(14)=(num&0x01);PDout(15)=(num>>1)&0x01;PDout(0)=(num>>2)&0x01;PDout(1)=(num>>3)&0x01;PEout(7)=(num>>4)&0x01;PEout(8)=(num>>5)&0x01;PEout(9)=(num>>6)&0x01;PEout(10)=(num>>7)&0x01;

#define LCD_W 240
#define LCD_H 320

//颜色宏定义
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


void LCD_IO_ENABLE(void);
void LCD_init(void);
void LCD_Writ_Bus(char VH,char VL);
void LCD_WR_DATA8(char VH,char VL);
void LCD_WR_DATA(int da);
void LCD_WR_REG(int da); 
void LCD_WR_REG_DATA(int reg,int da);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_Init(void);
void LCD_Clear(u16 Color);
void LCD_DrawPoint(u16 x,u16 y);
void LCD_DrawPoint_big(u16 x,u16 y);
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);
void LCD_ShowString(u16 x,u16 y,const u8 *p);
void showhanzi(unsigned int x,unsigned int y,unsigned char index)	;
void xianshi(void);

#endif


