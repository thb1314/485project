#ifndef __MODBUS_H__
#define __MODBUS_H__

#include "crc16.h"

typedef struct 
{
	// 本机地址
	unsigned char from_address;
	// 发送地址
	unsigned char to_address; 
	// 命令寄存器
	unsigned char reg_code;
	// 数据长度
	unsigned char data_len;
	// 数据指针
	unsigned char *data_ptr;
	// 数据crc校验
	CRC16_VALUE crc16_value;
} MODBUS_VALUE;

/*
  根据 modbus struct 返回modbus字符串
	input: MOUBUS_VALUE
	output: unsigned char*
	return: 数据长度
 */
unsigned int SetModBusStr(MODBUS_VALUE,unsigned char*);

/*
  根据 modbus struct 返回modbus字符串
	input: unsigned char* 数据地址
	input: unsigned int 数据长度
	output: MOUBUS_VALUE
return: 1 数据ok 0 不正确
 */
unsigned char GetModBusVal(unsigned char*,unsigned int,MODBUS_VALUE*);

#endif