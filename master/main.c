#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "main.h"
#include "RS485.h"
#include "modbus.h"
#include "12864.h"
#include "key.h"
#include "mystring.h"
#include "EEPROM.H"
#include "STC12C5A60S2.h"

typedef struct myTxError
{
	unsigned char C1ERR;
	unsigned char C2ERR;
	unsigned char C3ERR;
	unsigned char C4ERR;
} TxError;


unsigned int timer0_s = 0x00;
unsigned int timer0_20ms = 0x00;
//485���ݽ��ջ���
unsigned char modbus_str[MAX_COMCHAR_LEN]={0x00};
//485���ݽ��ս��ͽṹ��
MODBUS_VALUE modbus_val;
//lcd����չʾ�ַ���
unsigned char lcd_str[17]={0x00};
//��������ṹ��
TxError tx_err;
//���������ַ���
unsigned char btn_str[12] = {0x00};


void Delay5000ms()		//@22.1184MHz
{
	unsigned char i, j, k;

	i = 165;
	j = 59;
	k = 28;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void Delay20ms()		//@22.1184MHz
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
}




//��ʱ��0 16λģʽ 20ms����һ��
void Timer0Init(void)		//20����@22.1184MHz
{

	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x70;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0=1;					//����ʱ��T0�ж�
	EA=1;					//�������ж�
}

void InitSystem()
{
	unsigned int ad_val = 0; 
	//��ʼ��485
	RS485_init();
	
	
	
	//���������ʼ��
	tx_err.C1ERR = 0;
	tx_err.C2ERR = 0;
	tx_err.C3ERR = 0;
	tx_err.C4ERR = 0;
	
	//��ʼ��12864
	lcd_init();
	//������ʱ����ʾAD
	ad_val = Byte_Read(AD_VALUE1_ADD);
	ad_val <<= 8;
	ad_val |= Byte_Read(AD_VALUE1_ADD+1);
	sprintf(lcd_str,"%.2f",(float)(ad_val/100.0f));
	lcd_display(2,1,lcd_str);
	lcd_display(1,0,"                ");
	
	ad_val = Byte_Read(AD_VALUE2_ADD);
	ad_val <<= 8;
	ad_val |= Byte_Read(AD_VALUE2_ADD+1);
	sprintf(lcd_str,"%.2f",(float)(ad_val/100.0f));
	lcd_display(2,3,lcd_str);
	
	ad_val = Byte_Read(AD_VALUE3_ADD);
	ad_val <<= 8;
	ad_val |= Byte_Read(AD_VALUE3_ADD+1);
	sprintf(lcd_str,"%.2f",(float)(ad_val/100.0f));
	lcd_display(2,5,lcd_str);
	
	ad_val = Byte_Read(AD_VALUE4_ADD);
	ad_val <<= 8;
	ad_val |= Byte_Read(AD_VALUE4_ADD+1);
	sprintf(lcd_str,"%.2f",(float)(ad_val/100.0f));
	lcd_display(2,7,lcd_str);
	
	
	
	ad_val = Byte_Read(AD_VALUE5_ADD);
	ad_val <<= 8;
	ad_val |= Byte_Read(AD_VALUE5_ADD+1);
	sprintf(lcd_str,"%.2f",(float)(ad_val/100.0f));
	lcd_display(3,1,lcd_str);

	
	ad_val = Byte_Read(AD_VALUE6_ADD);
	ad_val <<= 8;
	ad_val |= Byte_Read(AD_VALUE6_ADD+1);
	sprintf(lcd_str,"%.2f",(float)(ad_val/100.0f));
	lcd_display(3,3,lcd_str);
	
	ad_val = Byte_Read(AD_VALUE7_ADD);
	ad_val <<= 8;
	ad_val |= Byte_Read(AD_VALUE7_ADD+1);
	sprintf(lcd_str,"%.2f",(float)(ad_val/100.0f));
	lcd_display(3,5,lcd_str);
	
	ad_val = Byte_Read(AD_VALUE8_ADD);
	ad_val <<= 8;
	ad_val |= Byte_Read(AD_VALUE8_ADD+1);
	sprintf(lcd_str,"%.2f",(float)(ad_val/100.0f));
	lcd_display(3,7,lcd_str);
	
	
	
	lcd_display(1,1,"                ");


	
	
	lcd_display(0,1,"System initing..");
	Delay5000ms();
	lcd_init();
	
	//��ʱ������
	Timer0Init();
}


//�����������ֺ���
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


// �����¶�ֵ����
void send_gettemp_cmd(unsigned char to_address)
{
	unsigned char modbus_str[6]={0x00};
	unsigned char len = 0x00;
	MODBUS_VALUE modbus_val;
	modbus_val.from_address = SELF_ADDRESS;
	modbus_val.to_address  = to_address;
	modbus_val.reg_code  = GET_TEMP_CMD;
	modbus_val.data_len  = 0x00;
	len = SetModBusStr(modbus_val,modbus_str);
	flagTxd = 1;
	UartWrite(modbus_str,len);
	flagTxd = 0;
}

// ����AD����
void send_getad_cmd(unsigned char to_address,unsigned char reg_code)
{
	unsigned char modbus_str[6]={0x00};
	unsigned char len = 0x00;
	MODBUS_VALUE modbus_val;
	modbus_val.from_address = SELF_ADDRESS;
	modbus_val.to_address  = to_address;
	modbus_val.reg_code  = reg_code;
	modbus_val.data_len  = 0x00;
	len = SetModBusStr(modbus_val,modbus_str);
	flagTxd = 1;
	UartWrite(modbus_str,len);
	flagTxd = 0;
}



//�жϰ���
void judge_btn()
{
	unsigned char btn_val;
	btn_val = keyscan();
	if(16 != btn_val)
	{
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
					char tmp[9] = {0x00};
					unsigned char to_address;
					AnyNumber to_num;
					memcpy(tmp,&btn_str[0],3);
					to_address = (unsigned int)atof(tmp);
					memcpy(tmp,&btn_str[3],8);
					to_num.num = (long int)atof(tmp);
					sprintf(lcd_str,"to:%-3uv:%ld",(unsigned int)to_address,to_num.num);
					send_anynumber(to_address,to_num);
					lcd_display(2,1,lcd_str);
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

void main()
{
	unsigned char len = 0x00;
	unsigned char boolean;
	int temperature = 0;
	unsigned int ad_value = 0;
	InitSystem();
	
	while(1)
	{
		//����н���
		if(flagRxd)
		{
	
			//��ֹ�����ж�
			while(flagTxd);
			//���20ms����
			Delay20ms();
			len = UartRead(modbus_str,MAX_COMCHAR_LEN);
			boolean = GetModBusVal(modbus_str,len,&modbus_val);
			if(boolean)
			{
				if(SELF_ADDRESS == modbus_val.to_address)
				{
					if(SEND_TEMP_CMD == modbus_val.reg_code)
					{
						temperature = modbus_val.data_ptr[0];
						temperature <<= 8;
						temperature |= modbus_val.data_ptr[1];
						
						//У����ȷ �����¶�ֵ
						switch (modbus_val.from_address)
						{
							case 1:
								tx_err.C1ERR = 0;
								sprintf(lcd_str,"#1:%.2f",(float)(temperature/100.0f));
								lcd_display(0,1,lcd_str);
								break;
							case 2:
								tx_err.C2ERR = 0;
								sprintf(lcd_str,"#2:%.2f",(float)(temperature/100.0f));
								lcd_display(0,5,lcd_str);
								break;
							case 3:
								tx_err.C3ERR = 0;
								sprintf(lcd_str,"#3:%.2f",(float)(temperature/100.0f));
								lcd_display(1,1,lcd_str);
								break;
							case 4:
								tx_err.C4ERR = 0;
								sprintf(lcd_str,"#4:%.2f",(float)(temperature/100.0f));
								lcd_display(1,5,lcd_str);
								break;
							default:
								break;
						}
					}
					else if(SEND_AD1_CMD == modbus_val.reg_code)
					{
						ad_value = modbus_val.data_ptr[0];
						ad_value <<= 8;
						ad_value |= modbus_val.data_ptr[1];
						
						//У����ȷ ����AD
						switch (modbus_val.from_address)
						{
							case 1:
								tx_err.C1ERR = 0;
								sprintf(lcd_str,"%.2f",(float)(ad_value/100.0f));
								lcd_display(2,1,lcd_str);
							
								//AD
								Sector_Erase(AD_VALUE1_ADD);
								Sector_Erase(AD_VALUE1_ADD+1);
								Byte_Program(AD_VALUE1_ADD,modbus_val.data_ptr[0]);
								Byte_Program(AD_VALUE1_ADD+1,modbus_val.data_ptr[1]);
								break;
							case 2:
								tx_err.C2ERR = 0;
								sprintf(lcd_str,"%.2f",(float)(ad_value/100.0f));
							
							
								//AD
								Sector_Erase(AD_VALUE2_ADD);
								Sector_Erase(AD_VALUE2_ADD+1);
								Byte_Program(AD_VALUE2_ADD,modbus_val.data_ptr[0]);
								Byte_Program(AD_VALUE2_ADD+1,modbus_val.data_ptr[1]);
								lcd_display(2,3,lcd_str);
								break;
							case 3:
								tx_err.C3ERR = 0;
								sprintf(lcd_str,"%.2f",(float)(ad_value/100.0f));
							
							
								//AD
								Sector_Erase(AD_VALUE3_ADD);
								Sector_Erase(AD_VALUE3_ADD+1);
								Byte_Program(AD_VALUE3_ADD,modbus_val.data_ptr[0]);
								Byte_Program(AD_VALUE3_ADD+1,modbus_val.data_ptr[1]);
								lcd_display(2,5,lcd_str);
								break;
							case 4:
								tx_err.C4ERR = 0;
								sprintf(lcd_str,"%.2f",(float)(ad_value/100.0f));
							
								//AD
								Sector_Erase(AD_VALUE4_ADD);
								Sector_Erase(AD_VALUE4_ADD+1);
								Byte_Program(AD_VALUE4_ADD,modbus_val.data_ptr[0]);
								Byte_Program(AD_VALUE4_ADD+1,modbus_val.data_ptr[1]);
								lcd_display(2,7,lcd_str);
								break;
							default:
								break;
						}	
			
					}
					else if(SEND_AD2_CMD == modbus_val.reg_code)
					{
						ad_value = modbus_val.data_ptr[0];
						ad_value <<= 8;
						ad_value |= modbus_val.data_ptr[1];
						
						//У����ȷ ����AD
						switch (modbus_val.from_address)
						{
							case 1:
								tx_err.C1ERR = 0;
								sprintf(lcd_str,"%.2f",(float)(ad_value/100.0f));
								lcd_display(3,1,lcd_str);
							
								//AD
								Sector_Erase(AD_VALUE5_ADD);
								Sector_Erase(AD_VALUE5_ADD+1);
								Byte_Program(AD_VALUE5_ADD,modbus_val.data_ptr[0]);
								Byte_Program(AD_VALUE5_ADD+1,modbus_val.data_ptr[1]);
								break;
							case 2:
								tx_err.C2ERR = 0;
								sprintf(lcd_str,"%.2f",(float)(ad_value/100.0f));
							
							
								//AD
								Sector_Erase(AD_VALUE6_ADD);
								Sector_Erase(AD_VALUE6_ADD+1);
								Byte_Program(AD_VALUE6_ADD,modbus_val.data_ptr[0]);
								Byte_Program(AD_VALUE6_ADD+1,modbus_val.data_ptr[1]);
								lcd_display(3,3,lcd_str);
								break;
							case 3:
								tx_err.C3ERR = 0;
								sprintf(lcd_str,"%.2f",(float)(ad_value/100.0f));
							
							
								//AD
								Sector_Erase(AD_VALUE7_ADD);
								Sector_Erase(AD_VALUE7_ADD+1);
								Byte_Program(AD_VALUE7_ADD,modbus_val.data_ptr[0]);
								Byte_Program(AD_VALUE7_ADD+1,modbus_val.data_ptr[1]);
								lcd_display(3,5,lcd_str);
								break;
							case 4:
								tx_err.C4ERR = 0;
								sprintf(lcd_str,"%.2f",(float)(ad_value/100.0f));
							
								//AD
								Sector_Erase(AD_VALUE8_ADD);
								Sector_Erase(AD_VALUE8_ADD+1);
								Byte_Program(AD_VALUE8_ADD,modbus_val.data_ptr[0]);
								Byte_Program(AD_VALUE8_ADD+1,modbus_val.data_ptr[1]);
								lcd_display(3,7,lcd_str);
								break;
							default:
								break;
						}	
			
					}
				}

				if(SEND_NUM_CMD == modbus_val.reg_code)
				{
					AnyNumber to_num;
					memcpy(&to_num.c[0],modbus_val.data_ptr,4);
					sprintf(lcd_str,"%-3u %-3u %-8ld",(unsigned int)modbus_val.from_address,(unsigned int)modbus_val.to_address,to_num.num);
					lcd_display(3,1,lcd_str);
				}

			}

				//����֮���Է���һ flagRxd  = 0;
			flagRxd = 0;
		}

		if(tx_err.C1ERR >= 2)
		{
			tx_err.C1ERR = 2;
			sprintf(lcd_str,"#1:Error");
			lcd_display(0,1,lcd_str);
		}
		if(tx_err.C2ERR >= 2)
		{
			tx_err.C2ERR = 2;
			sprintf(lcd_str,"#2:Error");
			lcd_display(0,5,lcd_str);
		}
		if(tx_err.C3ERR >= 2)
		{
			tx_err.C3ERR = 0;
			sprintf(lcd_str,"#3:Error");
			lcd_display(1,1,lcd_str);
		}
		if(tx_err.C4ERR >= 2)
		{
			tx_err.C4ERR = 0;
			sprintf(lcd_str,"#4:Error");
			lcd_display(1,5,lcd_str);
		}
		judge_btn();
	}
}



void timer0() interrupt 1    
{    
	TL0 = 0x00;
	TH0 = 0x70; 

	timer0_20ms = (timer0_20ms > 60000) ? 1 : (timer0_20ms + 1);
	if(0 == (timer0_20ms % 25))
	{
		timer0_s = (timer0_s > 12) ? 1 : (timer0_s + 1);
		//16ms��ǰ4ms��ͨ��4��
		if(timer0_s < 5)
		{
			send_gettemp_cmd(timer0_s);
		}
		else if(timer0_s >= 5 && timer0_s <= 8)
		{
			send_getad_cmd(timer0_s-4,GET_AD1_CMD);
		}
		else if(timer0_s > 8 && timer0_s < 12)
		{
			send_getad_cmd(timer0_s-8,GET_AD2_CMD);
		}
		switch (timer0_s)
		{
			// ���ӻ����Ͷ�����
			case 1:
				tx_err.C1ERR++;
				break;
			case 2:
				tx_err.C2ERR++;
				break;
			case 3:
				tx_err.C3ERR++;
				break;
			case 4:
				tx_err.C4ERR++;
				break;
			default:
				break;
		}
	}
	
}