#ifndef __AD_H_
#define __AD_H_

/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks

extern void InitAD();
extern unsigned char GetADCResult(unsigned char ch);
// 0 - 255
extern unsigned int GetAD_value(unsigned char ch);


typedef union {
	unsigned char c[2];
	unsigned int i;
} AD_value ;

#endif