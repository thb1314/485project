#ifndef __MAIN_H__
#define __MAIN_H__

#define SELF_ADDRESS 0x04


#define GET_TEMP_CMD 0x01
#define SEND_TEMP_CMD 0x02
#define SEND_NUM_CMD 0x03
#define GET_AD1_CMD 0x04
#define SEND_AD1_CMD 0x05
#define GET_AD2_CMD 0x06
#define SEND_AD2_CMD 0x07



#define AD_ADDRESS 0x00  //STC5Axx 系列 EEPROM 测试起始地址



typedef union myTEMP_VALUE{
	int i;
	char c[2];
} TEMP_VALUE;


typedef union myAnyNumber
{
	long int num;
	unsigned char c[4];
} AnyNumber;

//初始化配置
void InitSystem();


#endif