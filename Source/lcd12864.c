#include "lcd12864.h"
//包含该头文件可使用_nop_()函数
#include <intrins.h>

/***********************************************
函数名称：LCD12864_Delay1ms
功    能：STC 1T单片机1ms延时程序
入口参数：ms:延时的毫秒数
返 回 值：无	
备    注：示波器实测1.05ms 时钟11.0592MHz
************************************************/
void LCD12864_Delay1ms(unsigned int ms)
{
  unsigned int i;
  while( (ms--) != 0)
  {
    for(i = 0; i < 845; i++); 
  }             
}
/*************************************
* 函 数 名: LCD12864_WriteCMD
* 函数功能: 向lcd12864写指令
* 入口参数: cmd：指令
* 返    回: 无
**************************************/
void LCD12864_WriteCMD(unsigned char cmd)
{
  LCD12864_CheckBusy();
  RS_Clr();
  RW_Clr();
  CE_Set();
  LCD_PORT=cmd;
	_nop_();
	_nop_();
  CE_Clr();
}
/***********************************************
函数名称：LCD12864_WriteDAT
功    能：向lcd12864写数据
入口参数：dat：数据
返 回 值：无	
备    注：无
************************************************/
void LCD12864_WriteDAT(unsigned char dat)
{
  LCD12864_CheckBusy();
  RS_Set();
  RW_Clr();
  CE_Set();
  LCD_PORT=dat;
	_nop_();
	_nop_();
  CE_Clr();
}
/***********************************************
函数名称：LCD12864_CheckBusy
功    能：检测LCD12864忙信号
入口参数：无
返 回 值：无	
备    注：无
************************************************/
void LCD12864_CheckBusy(void)
{
  unsigned char temp=0;
	LCD_PORT=0xff;	 //做输入先置高,51系列单片机读之前需要置高
  do
	{        
    RS_Clr();
    RW_Set();
    CE_Set();
		_nop_();
		_nop_();
    temp=LCD_PORT;
    CE_Clr();
  }
	while((temp&0x80)!=0);//高位忙标志BF，BF=1表示：忙
}
 /***********************************************
函数名称：LCD12864_Init
功    能：LCD12864初始化函数
入口参数：无
返 回 值：无	
备    注：并行模式
************************************************/
void LCD12864_Init(void)  
{
	//初始化P0口为准双向口
	P0M1 =0x00;  
	P0M0 =0x00; 
	
	//初始化P41,P42口为准双向口
	P4M1 &=~( (1<<1) | (1<<2) );  
	P4M0 &=~( (1<<1) | (1<<2) ); 
	
	//初始化P37口为准双向口
	P3M1 &=~(1<<7);  
	P3M0 &=~(1<<7);  
	
	LCD12864_Delay1ms(100);      		//上电延时100ms
	LCD12864_WriteCMD(0x01);//清屏
	LCD12864_Delay1ms(10);
	LCD12864_Display(LINE1+1,"多功能电子锁" );		//第一行显示字符
	LCD12864_Display(LINE2+1,"自动化叁班" );		//第二行显示字符
	LCD12864_Display(LINE3+1,"陈琛");		//第三行显示字符
	LCD12864_Display(LINE4+1,"邬迪");			//第四行显示字符	
}
/***********************************************
函数名称：LCD12864_Clear_Line
功    能：lcd12864清除行函数
入口参数：line：行地址
返 回 值：无	
备    注：无
************************************************/
void LCD12864_Clear_Line(unsigned char line)
{
	unsigned char i;
	
	if( (line&LINE4)==LINE4 )	  //判断行地址，决定该清除哪行
	{
		line=LINE4;	
	}
	else if( (line&LINE3)==LINE3 )
	{
		line=LINE3;	
	}
	else if( (line&LINE2)==LINE2 )
	{
		line=LINE2;	
	}
	else if( (line&LINE1)==LINE1 )
	{
		line=LINE1;	
	}

  LCD12864_WriteCMD(0x30);    //功能设定：八位传输，基本指令集
  LCD12864_Delay1ms(1);
  LCD12864_WriteCMD(0x0c);	  //显示状态：整体显示
  LCD12864_Delay1ms(1);
  LCD12864_WriteCMD(0x06);	  //进入点设定
  LCD12864_Delay1ms(1);
  LCD12864_WriteCMD(line);	  //设定DDRAM地址
	for(i=0;i<16;i++)
	{
		LCD12864_WriteDAT(' ');	
	}	
}
/***********************************************
函数名称：LCD12864_Display
功    能：写多字节字符
入口参数：addr：起始地址，pointer指针地址
返 回 值：无	
备    注：详细的地址和命令见数据手册
************************************************/
void LCD12864_Display(unsigned char addr,unsigned char* pointer)
{
	LCD12864_Clear_Line(addr);	  //清除该行
	LCD12864_Delay1ms(1);

  LCD12864_WriteCMD(0x30);      //功能设定：八位传输，基本指令集
  LCD12864_Delay1ms(1);
  LCD12864_WriteCMD(0x0c);	  //显示状态：整体显示
  LCD12864_Delay1ms(1);
  LCD12864_WriteCMD(0x06);	  //进入点设定
  LCD12864_Delay1ms(1);
  LCD12864_WriteCMD(addr);	  //设定DDRAM地址
  while(*pointer!='\0')	      //未到字符串末尾
  {
  	LCD12864_WriteDAT(*pointer);
	  pointer++;
  }
}
void LCD12864_Display1(unsigned char addr,unsigned char* pointer)
{
	LCD12864_Clear_Line(addr);	  //清除该行

  LCD12864_WriteCMD(0x30);      //功能设定：八位传输，基本指令集
  LCD12864_WriteCMD(0x0c);	  //显示状态：整体显示
  LCD12864_WriteCMD(0x06);	  //进入点设定
  LCD12864_WriteCMD(addr);	  //设定DDRAM地址
  while(*pointer!='\0')	      //未到字符串末尾
  {
  	LCD12864_WriteDAT(*pointer);
	  pointer++;
  }
}
/***********************************************
函数名称：App_FormatDec
功    能：整型数据转字符串函数
入口参数：value:整型数据
返 回 值：pstr：指向字符串的指针。	
备    注：无
************************************************/
void  App_FormatDec (unsigned char *pstr, unsigned char value)
{
  unsigned char   i;
  unsigned int   mult; 
  unsigned int   nbr;

  mult  = 1;
	//因为这里要处理的value输入格式为三位数，例如123
	//故初始让mult=100
  for (i = 0; i < 2; i++) 
	{
    mult *= 10;
  }

	i=0;
  while (mult > 0) 
	{
		i++; 
		//第一次整除，获取最高位数据，例如123/100=1
		//其他位类推
    nbr = value / mult;
		//如果得到的数据不是0
    if (nbr != 0) 
	  {
			//整型数据+'0'，将自动转换成字符数据，例如1+'0'将变成字符'1'
      *pstr = nbr + '0';
    } 
	  else
		{   
			//如果数据是0，则直接转换成字符'0'           
      *pstr = '0';               
    }
		//移动指针，进行其他位数据处理
    pstr++;
    value %= mult;
    mult  /= 10;
  }
}
