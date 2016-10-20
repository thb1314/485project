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
Description: LCD ��ʼ��
Author: THB
Version: 1.0
Date: 2016/4/1
History: 2016/4/1 
*****************************************************************************/
void lcd_init(void);

/*****************************************************************************
Copyright: 2016 THB
File name: 12864.H
Description: ��QB12864�������ݻ�������
Input Param: char �����ַ�
Input Param: bit 0��ʾ��������,1��ʾ
Author: THB
Version: 1.0
Date: 2016/4/1
History: 2016/4/1 
*****************************************************************************/
void lcd_transferdata(char, bit);

/*****************************************************************************
Copyright: 2016 THB
File name: 12864.H
Description: �����ַ���ʾλ��
Input Param: unsigned char ��ʾ��X���� 0-3
Input Param: unsigned char ��ʾ��Y���� 1-8
Author: THB
Version: 1.0
Date: 2016/4/1
History: 2016/4/1 
*****************************************************************************/
void lcd_pos(unsigned char, unsigned char);


/*****************************************************************************
Copyright: 2016 THB
File name: 12864.H
Description: LCDר����ʱ
Input Param: int ��ʱʱ��
Author: THB
Version: 1.0
Date: 2016/4/1
History: 2016/4/1 
*****************************************************************************/
void lcd_delay(unsigned int);

/*****************************************************************************
Copyright: 2016 THB
File name: 12864.H
Description: LCD��ʾ�ַ���ݺ���
Input Param: unsigned char ��ʾ��X���� 0-3
Input Param: unsigned char ��ʾ��Y���� 1-8
Input Param: Ҫ��ʾ���ַ�����ָ��
Author: THB
Version: 1.0
Date: 2016/4/1
History: 2016/4/1 
*****************************************************************************/
void lcd_display(unsigned char, unsigned char, unsigned char *);



#endif