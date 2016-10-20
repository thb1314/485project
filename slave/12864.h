/*--------------------------------------------------------------------------
12806.H

author:thb
email:52thb1314@gmail.com
Header file for QB12864B driver
--------------------------------------------------------------------------*/

#ifndef __12806_H__
#define __12806_H__
#include "STC12C5A60S2.H"

sbit LCD_RS = P2 ^ 0;
sbit LCD_RW = P2 ^ 1;
sbit LCD_EN = P2 ^ 2;
sbit LCD_PSB = P2 ^ 3;
sbit LCD_RET = P2 ^ 5;




/*****************************************************************************
Copyright: 2016 THB
File name: 12864.H
Description: LCD 初始化
Author: THB
Version: 1.0
Date: 2016/4/1
History: 2016/4/1 
*****************************************************************************/
void lcd_init(void);

/*****************************************************************************
Copyright: 2016 THB
File name: 12864.H
Description: 向QB12864发送数据或者命令
Input Param: char 命令字符
Input Param: bit 0表示发送命令,1表示
Author: THB
Version: 1.0
Date: 2016/4/1
History: 2016/4/1 
*****************************************************************************/
void lcd_transferdata(char, bit);

/*****************************************************************************
Copyright: 2016 THB
File name: 12864.H
Description: 设置字符显示位置
Input Param: unsigned char 显示的X坐标 0-3
Input Param: unsigned char 显示的Y坐标 1-8
Author: THB
Version: 1.0
Date: 2016/4/1
History: 2016/4/1 
*****************************************************************************/
void lcd_pos(unsigned char, unsigned char);


/*****************************************************************************
Copyright: 2016 THB
File name: 12864.H
Description: LCD专用延时
Input Param: int 延时时间
Author: THB
Version: 1.0
Date: 2016/4/1
History: 2016/4/1 
*****************************************************************************/
void lcd_delay(unsigned int);

/*****************************************************************************
Copyright: 2016 THB
File name: 12864.H
Description: LCD显示字符快捷函数
Input Param: unsigned char 显示的X坐标 0-3
Input Param: unsigned char 显示的Y坐标 1-8
Input Param: 要显示的字符串的指针
Author: THB
Version: 1.0
Date: 2016/4/1
History: 2016/4/1 
*****************************************************************************/
void lcd_display(unsigned char, unsigned char, unsigned char *);



#endif