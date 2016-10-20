#ifndef __COM_H__
#define __COM_H__

/*
 * ���ý����ַ�
 */
extern unsigned char UartRcvChar;

/**
 * ���ڳ�ʼ������
 * return void
 */
extern void UartInit(void);

/**
 * ���ڷ��͵����ַ�
 * @param char Ҫ���͵��ַ���
 * return void
 */
extern void _com_send_char(char);


/**
 * ���ڷ����ַ��� \0������
 * @param char*
 * return void 
 */
extern void _com_send_string(char*);
/**
 * ���ڷ����ַ��� \0������
 * @param unsigned char* ���ݵ�ַ
 * @param unsigned char ���ݳ���
 * return void 
 */
void UartWrite(unsigned char *, unsigned char);
/**
 * ���ڽ����жϺ���
 * return void 
 */
extern void UartRcv(void);

#endif