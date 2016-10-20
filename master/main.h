#ifndef __MAIN_H__
#define __MAIN_H__

#define SELF_ADDRESS 0xFF


#define GET_TEMP_CMD 0x01
#define SEND_TEMP_CMD 0x02
#define SEND_NUM_CMD 0x03
#define GET_AD1_CMD 0x04
#define SEND_AD1_CMD 0x05
#define GET_AD2_CMD 0x06
#define SEND_AD2_CMD 0x07

#define AD_VALUE1_ADD 0x0000
#define AD_VALUE2_ADD 0x0002
#define AD_VALUE3_ADD 0x0004
#define AD_VALUE4_ADD 0x0006
#define AD_VALUE5_ADD 0x0200
#define AD_VALUE6_ADD 0x0202
#define AD_VALUE7_ADD 0x0204
#define AD_VALUE8_ADD 0x0206



typedef union myAnyNumber
{
	long int num;
	unsigned char c[4];
} AnyNumber;

//初始化配置
void InitSystem();


#endif