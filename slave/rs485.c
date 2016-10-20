#include "RS485.h"
#include "STC12C5A60S2.H"
#include <intrins.h>
#define FOSC      22118400UL   /*使用22.1184M晶体*/
#define BAUDRATE   9600UL       // set 
#define S2RI 0x01		// S2CON.0
#define S2TI 0x02		// S2CON.1
#define S2RB8 0x04		// S2CON.2
#define S2TB8 0x08		// S2CON.3


bit flagRxd = 0; 
bit flagTxd = 0; 
bit	uart_bit9 = 0;
unsigned char cntRxd = 0;
unsigned char bufRxd[MAX_COMCHAR_LEN] = {0x00}; //串口接收缓冲区

void RS485_Delay50us()
{
	unsigned char i, j;
	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}


void RS485_init()  //串口配置函数，baud为波特率
{
	RS485_DIR = 0; //RS485设置为接收方向
	P4SW |= 0x10;
	
	#if (PARITYBIT == NONE_PARITY)
	SCON = 0x50;	//	8-bit varaible UART
	#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
	SCON = 0xDA;	//	9-bit varaible UART,parity bit initial to 1
	#elif (PARITYBIT == SPACE_PARITY)
	SCON = 0xD5;	//	9-bit varaible UART,parity bit initial to 0
	#endif
	TMOD = 0x20;	// set Timer1 as 8-bit auto reload
	TH1 = TL1 = (unsigned char)(256-(FOSC/12/32/BAUDRATE));	//set auto-load value
	TR1 = 1;	// Enable timer1
	ES = 1;	//Enable UART interrupt
	EA = 1;	//Open master interrupt switch

}
unsigned char UartRead(unsigned char *buf, unsigned char len) //串口数据读取函数，数据接收指针buf，读取数据长度len，返回值为实际读取到的数据长度
{
	unsigned char i = 0;
    
    if (len > cntRxd)  //指定读取长度大于实际接收到的数据长度时，
    {					//读取长度设置为实际接收到的数据长度
        len = cntRxd;	
    }
		for(i = 0;i<len;i++)
		{
			buf[i] = bufRxd[i];
		}
    cntRxd = 0;  //接收计数器清零
		flagRxd = 0;
    return len;  //返回实际读取长度
}



void UartRcv() interrupt 4
{
	if(RI)
	{

		RI = 0;

		if (cntRxd < MAX_COMCHAR_LEN) 
		//接收缓冲区尚未用完时，
		{    
			//保存接收字节，并递增计数器
				bufRxd[cntRxd++] = SBUF;
		}
		flagRxd = 1;
		uart_bit9 = RB8;
	}
}


void UartWrite(unsigned char *buf, unsigned char len)
{
	RS485_DIR = 1;  //RS485设置为发送
	while (len)   //循环发送所有字节
	{
		ACC = *(buf++);
		if(P)
		{
		#if (PARITYBIT == ODD_PARITY)
			TB8 = 0;
		#elif (PARITYBIT == EVEN_PARITY)
			TB8 = 1;
		#endif
		}
		else
		{
		#if (PARITYBIT == ODD_PARITY)
			TB8 = 1;
		#elif (PARITYBIT == EVEN_PARITY)
			TB8 = 0;
		#endif
		}
		SBUF = ACC;
		while(!TI);
		TI = 0;
		len--;
	}
	RS485_Delay50us();  //等待最后的停止位完成，延时时间由波特率决定
	RS485_DIR = 0;  //RS485设置为接收
}