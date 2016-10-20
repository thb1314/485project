#ifndef __COM_H__
#define __COM_H__

/*
 * 设置接收字符
 */
extern unsigned char UartRcvChar;

/**
 * 串口初始化设置
 * return void
 */
extern void UartInit(void);

/**
 * 串口发送单个字符
 * @param char 要发送的字符串
 * return void
 */
extern void _com_send_char(char);


/**
 * 串口发送字符串 \0结束的
 * @param char*
 * return void 
 */
extern void _com_send_string(char*);
/**
 * 串口发送字符串 \0结束的
 * @param unsigned char* 数据地址
 * @param unsigned char 数据长度
 * return void 
 */
void UartWrite(unsigned char *, unsigned char);
/**
 * 串口接收中断函数
 * return void 
 */
extern void UartRcv(void);

#endif