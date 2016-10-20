#include "modbus.h"
#include "crc16.h"
unsigned int SetModBusStr(MODBUS_VALUE modbus_value,unsigned char* modbus_str)
{
	unsigned char i = 0;
	modbus_str[0] = modbus_value.from_address;	
	modbus_str[1] = modbus_value.to_address;	
	modbus_str[2] = modbus_value.reg_code;
	modbus_str[3] = modbus_value.data_len;
	modbus_value.data_len += 4;
	i = 4;
	for (i = 4; i < modbus_value.data_len; i++)
	{
		modbus_str[i] =  modbus_value.data_ptr[i-4];
	}
	modbus_value.crc16_value = CRC16(modbus_str,i);
	modbus_str[i++] = modbus_value.crc16_value.c[1];
	modbus_str[i++] = modbus_value.crc16_value.c[0];
	return i;
}


unsigned char GetModBusVal(unsigned char* modbus_str,unsigned int len,MODBUS_VALUE *modbus_value)
{
	unsigned char i = 0;
	modbus_value->from_address = modbus_str[0];
	modbus_value->to_address = modbus_str[1];
	modbus_value->reg_code = modbus_str[2];
	modbus_value->data_len = modbus_str[3];
	modbus_value->data_ptr = &modbus_str[4];
	modbus_value->crc16_value = CRC16(modbus_str,len-2);
	if((modbus_value->crc16_value.c[1] == modbus_str[len-2]) && (modbus_value->crc16_value.c[0] == modbus_str[len-1]))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}