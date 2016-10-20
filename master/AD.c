
/*  data and procedures from STC    */
/*------------------------------------------------------------------*/

#include "STC12C5A60S2.H"
#include "AD.H"
#include <intrins.h>
sfr ADC_LOW2    =   0xBE;           //ADC low 2-bit result register



unsigned int GetAD_value(unsigned char ch)
{
		AD_value myad_value;


		myad_value.c[0] = GetADCResult(ch);
		myad_value.c[1] = ADC_LOW2;
		myad_value.i >>= 6;
		myad_value.i = (unsigned int)((myad_value.i/1023.0) * 3.30 * 100);
		//return myad_value.c[1];
		return myad_value.i;
}

/*----------------------------
Get ADC result
----------------------------*/
unsigned char GetADCResult(unsigned char ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //Return ADC result
}

/*----------------------------
Initial ADC sfr
----------------------------*/
void InitAD()
{
	unsigned int timer = 10000;
    P1ASF |= 0x01;                   //Open 8 channels ADC function
    ADC_RES = 0;                    //Clear previous result
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    //此处需要延时
	while(timer--);
}
