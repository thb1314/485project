#ifndef __EEPROM_H__
#define __EEPROM_H__



//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0x80 //系统工作时钟<30MHz 时，对IAP_CONTR 寄存器设置此值
#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x82 //系统工作时钟<20MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x84 //系统工作时钟<6MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x85 //系统工作时钟<3MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x86 //系统工作时钟<2MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x87 //系统工作时钟<1MHz 时，对IAP_CONTR 寄存器设置此值


#define DATA_FLASH_START_ADDRESS 0x00  //STC5Axx 系列 EEPROM 测试起始地址



unsigned char Byte_Read(unsigned int add);              //读一字节，调用前需打开IAP 功能
void Byte_Program(unsigned int add, unsigned char ch);  //字节编程，调用前需打开IAP 功能
void Sector_Erase(unsigned int add);            //擦除扇区
void IAP_Disable();                       //关闭IAP 功能


#endif