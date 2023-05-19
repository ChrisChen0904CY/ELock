# ifndef __LCD12864_H__
# define __LCD12864_H__

#include "stc15f2k60s2.h"

//LCD12864操作位定义
//LCD12864操作位定义
#define   	LCD_PORT    P0
sbit 		RS			=P1^2;		 //寄存器选择 0:指令寄存器 1:数据寄存器
sbit 		RW			=P2^0;		 //读写控制 0：写  1：读
sbit 		CE			=P2^1;		 //读写数据使能   0：停止 1：启动

//定义端口操作
#define SET  			1		//置高
#define CLR 			0		//置低

#define RS_Set()		{RS=SET;}		//端口置高
#define RS_Clr()		{RS=CLR;}		//端口置低

#define RW_Set()		{RW=SET;}		//端口置高
#define RW_Clr()		{RW=CLR;}		//端口置低

#define CE_Set()		{CE=SET;}		//端口置高
#define CE_Clr()		{CE=CLR;}		//端口置低

//12864行开始地址
#define 	LINE1 		0x80		 //第一行地址
#define 	LINE2 		0x90		 //第二行地址
#define 	LINE3 		0x88		 //第三行地址
#define 	LINE4 		0x98		 //第四行地址

//函数声明
void LCD12864_WriteCMD(unsigned char cmd);
void LCD12864_WriteDAT(unsigned char dat);
void LCD12864_CheckBusy(void);
void LCD12864_Init(void) ;
void LCD12864_Display(unsigned char addr,unsigned char* pointer);
void LCD12864_Display1(unsigned char addr,unsigned char* pointer);
void  App_FormatDec (unsigned char *pstr, unsigned char value);
void LCD12864_Clear_Line(unsigned char line);

# endif