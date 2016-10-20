#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DS18B20.H"
#include "main.h"
#include "RS485.h"
#include "modbus.h"
#include "key.h"
#include "mystring.h"
#include "STC12C5A60S2.H"
#include "EEPROM.H"
#include "AD.H"


unsigned int timer0_s = 0x00;
unsigned int timer0_20ms = 0x00;
//485数据接收缓冲
unsigned char modbus_str[MAX_COMCHAR_LEN]={0x00};
MODBUS_VALUE modbus_val;
//按键缓存字符串
unsigned char btn_str[12] = {0x00};
//是否是第一次获取温度值
bit isFistGetTemp = 1;

/*void Delay20ms()		//@22.1184MHz
{
	unsigned char i, j, k;

	i = 2;
	j = 175;
	k = 75;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}*/






void InitSystem()
{
	//初始化485
	RS485_init();
	//初始化AD
	InitAD();
}

//发送任意数字函数
void send_anynumber(unsigned char to_address,AnyNumber number)
{
	unsigned char modbus_str[11]={0x00};
	unsigned char len = 0x00;
	MODBUS_VALUE modbus_val;
	modbus_val.from_address = SELF_ADDRESS;
	modbus_val.to_address = to_address;
	modbus_val.reg_code = SEND_NUM_CMD;
	modbus_val.data_len = 0x04;
	modbus_val.data_ptr = &number.c[0];
	len = SetModBusStr(modbus_val,modbus_str);
	flagTxd = 1;
	UartWrite(modbus_str,len);
	flagTxd = 0;
}


void judge_btn()
{
	unsigned char btn_val;
	btn_val = keyscan();
	if(16 != btn_val)
	{
		P0 = ~P0;
		switch (btn_val)
		{
			case 0x00:
				array_push('1',btn_str,11);
				break;
			case 0x02:
				array_push('2',btn_str,11);
				break;
			case 0x03:
				array_push('3',btn_str,11);
				break;
			case 0x01:
				if(11 == strlen(btn_str))
				{
					unsigned char tmp[9] = {0x00};
					unsigned char to_add;
					AnyNumber to_num;
					memcpy(tmp,&btn_str[0],3);
					to_add = (unsigned int)atof(tmp);
					memcpy(tmp,&btn_str[3],8);
					to_num.num = (long int)atof(tmp);
					send_anynumber(to_add,to_num);
				}
				re_str(btn_str,12);
				break;

			case 0x04:
				array_push('4',btn_str,11);
				break;
			case 0x06:
				array_push('5',btn_str,11);
				break;
			case 0x07:
				array_push('6',btn_str,11);
				break;
			case 0x05:
				break;


			case 0x08:
				array_push('7',btn_str,11);
				break;
			case 0x0A:
				array_push('8',btn_str,11);
				break;
			case 0x0B:
				array_push('9',btn_str,11);
				break;
			case 0x09:
				break;


			case 0x0C:
				break;
			case 0x0E:
				array_push('0',btn_str,11);
				break;
			case 0x0F:
				break;
			case 0x0D:
				break;
		  default:
				break;
		}
	}
	
}
// 发送温度值函数
void send_temp_val(float temperature)
{
	MODBUS_VALUE modbus_val;
	unsigned char modbus_str[7]={0x00};
	unsigned char len = 0x02;
	TEMP_VALUE temp_val;

	temp_val.i = (int)(temperature * 100);
	
	
	//EEPROM操作
	Sector_Erase(AD_ADDRESS);
	Sector_Erase(AD_ADDRESS+1);
	//EEPROM操作
	Byte_Program(AD_ADDRESS,temp_val.c[0]);
	Byte_Program(AD_ADDRESS+1,temp_val.c[1]);
	
	
	modbus_val.from_address = SELF_ADDRESS;
	modbus_val.to_address = 0xFF;
	modbus_val.reg_code = SEND_TEMP_CMD;
	modbus_val.data_len = 2;
	modbus_val.data_ptr = &temp_val.c[0];
	len = SetModBusStr(modbus_val,modbus_str);
	flagTxd = 1;
	UartWrite(modbus_str,len);
	flagTxd = 0;
}

// 发送AD值函数
void send_ad_val(unsigned int ad_val,unsigned char reg_code)
{
	AD_value ad_value;
	MODBUS_VALUE modbus_val;
	unsigned char modbus_str[7]={0x00};
	unsigned char len = 0x02;
	ad_value.i = ad_val;
	modbus_val.from_address = SELF_ADDRESS;
	modbus_val.to_address = 0xFF;
	modbus_val.reg_code = reg_code;
	modbus_val.data_len = 2;
	modbus_val.data_ptr = &ad_value.c[0];
	len = SetModBusStr(modbus_val,modbus_str);
	flagTxd = 1;
	UartWrite(modbus_str,len);
	flagTxd = 0;
}

void main()
{
	unsigned char len = 0x00;
	unsigned char boolean;
	unsigned int timer = 3000;
	InitSystem();
	while(1)
	{

		
		//如果有接收
		if(flagRxd)
		{
			//防止发送中断
			//while(flagTxd);
			//检测20ms按键
			while(timer--)
			{
				judge_btn();
			}
			//Delay20ms();
			len = UartRead(modbus_str,MAX_COMCHAR_LEN);
			boolean = GetModBusVal(modbus_str,len,&modbus_val);
			if(boolean && (SELF_ADDRESS == modbus_val.to_address))
			{
				if(GET_TEMP_CMD == modbus_val.reg_code)
				{

					if(isFistGetTemp)
					{
						TEMP_VALUE temp_val;
						isFistGetTemp = 0;
						DS18B20_ReadTemperature();
						temp_val.c[0] = Byte_Read(AD_ADDRESS);
						temp_val.c[1] = Byte_Read(AD_ADDRESS+1);
						send_temp_val((float)(temp_val.i/100.0));
					}
					else
					{
						send_temp_val(DS18B20_ReadTemperature());
					}
					
				}
				else if(GET_AD1_CMD == modbus_val.reg_code)
				{
					//获取(P1_0)0号信道AD值
					send_ad_val(GetAD_value(0),SEND_AD1_CMD);
				}
				else if(GET_AD2_CMD == modbus_val.reg_code)
				{
					//获取(P1_0)0号信道AD值
					send_ad_val(GetAD_value(1),SEND_AD2_CMD);
				}
			}

			timer = 2000;
			
			//读完之后以防万一 flagRxd  = 0;
			flagRxd = 0;
		}
		judge_btn();
	}
}



