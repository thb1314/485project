#ifndef _DS18B20_H_
#define _DS18B20_H_
#include <intrins.h>
#include "STC12C5A60S2.H"
#define DS_HIGH 1
#define DS_LOW 0

//定义DS18B20的接口
sbit DS_DQ = P4^1;

//定义DS18B20ROM指令
#define    DS18B20_ReadROM        0x33    //读ROM
#define    DS18B20_MatchROM       0x55    //匹配ROM
#define    DS18B20_SkipROM        0xCC    //跳过ROM
#define    DS18B20_SearchROM      0xF0    //搜索ROM
#define    DS18B20_AlarmROM       0xEC    //报警搜索

//定义DS18B20存储器操作命令
#define    DS18B20_WriteSCR       0x4E    //写暂存存储器
#define    DS18B20_ReadSCR        0xBE    //读暂存存储器
#define    DS18B20_CopySCR        0x48    //复制暂存存储器
#define    DS18B20_ConvertTemp    0x44    //温度变换
#define    DS18B20_RecallEP       0xB8    //重新调出
#define    DS18B20_ReadPower      0xB4    //读电源




extern unsigned char DS18B20_DATA[9];

void DS18B20_delay40us(unsigned int);

void DS18B20_delay20us(unsigned int);

bit DS18B20_Init(void);

void DS18B20_WriteByte(unsigned char);
float DS18B20_ReadTemperature(void);

unsigned char DS18B20_ReadByte();
#endif