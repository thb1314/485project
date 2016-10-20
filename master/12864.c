#include "12864.h"




void lcd_init(void)
{
	lcd_delay(40);
	//lcd并口显示
	LCD_PSB = 1;
	lcd_delay(1);
	LCD_RET = 0;
	lcd_delay(1);
	LCD_RET = 1;
	lcd_delay(10);
	lcd_transferdata(0x30, 0);
	lcd_delay(100);
	lcd_transferdata(0x30, 0);
	lcd_delay(37);
	lcd_transferdata(0x08, 0);
	lcd_delay(100);
	lcd_transferdata(0x10, 0);
	lcd_delay(100);
	lcd_transferdata(0x0C, 0);
	lcd_delay(100);
	lcd_transferdata(0x01, 0);
	lcd_delay(10);
	lcd_transferdata(0x06, 0);
	lcd_delay(100);
}



void lcd_display(unsigned char x, unsigned char y, unsigned char *adder)
{
	lcd_pos(x,y);
	while (*adder != '\0')
	{
		lcd_transferdata(*adder, 1);
		adder++;
	}
}

void lcd_transferdata(char data1, bit DI)
{
	LCD_RW = 0;
	LCD_RS = DI;
	lcd_delay(1);
	P0 = data1;
	LCD_EN = 1;
	lcd_delay(1);
	LCD_EN = 0;
}


void lcd_delay(unsigned int m)
{
	unsigned char i = 20;
	while (m--)
		while (i--);
}

void lcd_pos(unsigned char x, unsigned char y)
{
	switch (x)
	{
		case 0:
			x = 0x80;
			break;
		case 1:
			x = 0x90;
			break;
		case 2:
			x = 0x88;
			break;
		case 3:
			x = 0x98;
			break;
		default:
			x = 0x80;
			break;
	}
	if (y > 0 && y < 9)
	{
		x += y - 0x01;
	}
	//设置显示地址
	lcd_transferdata(x, 0);
	lcd_delay(100);
}