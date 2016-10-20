#ifndef __RS485_H__
#define __RS485_H__
#include "STC12C5A60S2.H"
#define FOSC      22118400UL   /*使用22.1184M晶体*/
#define BAUDRATE   9600UL       // set 

/*Define UART parity mode*/
#define NONE_PARITY 0
#define ODD_PARITY 1
#define EVEN_PARITY 2
#define MARK_PARITY 3
#define SPACE_PARITY 4
#define PARITYBIT NONE_PARITY


#define MAX_COMCHAR_LEN 100

sbit RS485_DIR = P4 ^ 4; //RS485控制端口

extern bit uart_bit9;

extern bit flagRxd; 
extern bit flagTxd; 
extern unsigned char bufRxd[MAX_COMCHAR_LEN];

/**
 * 串口2初始化设置
 * return void
 */
extern void RS485_init(void);


/**
 * 串口1接收中断函数
 * return void 
 */
extern void UartRcv(void);

extern unsigned char UartRead(unsigned char *, unsigned char);
extern void UartWrite(unsigned char *, unsigned char);
#endif