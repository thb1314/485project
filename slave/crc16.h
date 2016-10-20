#ifndef _CRC16_H_
#define _CRC16_H_

typedef union myCRC16_VALUE{
	unsigned int i;
	unsigned char c[2];
} CRC16_VALUE;


extern CRC16_VALUE CRC16(unsigned char *,  unsigned int);

#endif