#include "STC12C5A60S2.H"
#include "DS18B20.H"


unsigned char DS18B20_DATA[9] =  {0x00};
unsigned char code CRC8Table[]={
0,  94, 188,  226,  97,  63,  221,  131,  194,  
	156,  126,  32,  163,  253,  31,  65,
157,  195,  33,  127,  252,  162,  64,  30,  
	95,  1,  227,  189,  62,  96,  130,  220,
35,  125,  159,  193,  66,  28,  254,  160, 
	225,  191,  93,  3,  128,  222,  60,  98,
190,  224,  2,  92,  223,  129,  99,  61,  
	124,  34,  192,  158,  29,  67,  161,  255,
70,  24,  250,  164,  39,  121,  155,  197,  
	132,  218,  56,  102,  229,  187,  89,  7,
219,  133, 103,  57,  186,  228,  6,  88,  
	25,  71,  165,  251,  120,  38,  196,  154,
101,  59, 217,  135,  4,  90,  184,  230,  
	167,  249,  27,  69,  198,  152,  122,  36,
248,  166, 68,  26,  153,  199,  37,  123,  58,  
	100,  134,  216,  91,  5,  231,  185,
140,  210, 48,  110,  237,  179,  81,  15,  
	78,  16,  242,  172,  47,  113,  147,  205,
17,  79,  173,  243,  112,  46,  204,  146, 
	211,  141,  111,  49,  178,  236,  14,  80,
175,  241, 19,  77,  206,  144,  114,  44,  
109,  51,  209,  143,  12,  82,  176,  238,
50,  108,  142,  208,  83,  13,  239,  177, 
240,  174,  76,  18,  145,  207,  45,  115,
202,  148, 118,  40,  171,  245,  23,  73,  
8,  86,  180,  234,  105,  55,  213, 139,
87,  9,  235,  181,  54,  104,  138,  212, 
149,  203,  41,  119,  244,  170,  72,  22,
233,  183,  85,  11,  136,  214,  52,  106,
43,  117,  151,  201,  74,  20,  246,  168,
116,  42,  200,  150,  21,  75,  169,  247,  
182,  232,  10,  84,  215,  137,  107,  53
};


/**  
	计算CRC校验  
*/  
unsigned char CRC8_Table(unsigned char *p, unsigned int counter)  
{  
    unsigned char crc8 = 0;  
    while(counter--){  
        crc8 = CRC8Table[crc8^*p];  
        p++;  
    }  
    return(crc8);
} 

//DS18B20操作基本函数
void DS18B20_delay40us(unsigned int x)		//@22.1184MHz
{
	unsigned char i, j;
	while(x--)
	{
		
		_nop_();
		_nop_();
		_nop_();
		i = 1;
		j = 216;
		do
		{
			while (--j);
		} while (--i);
	}

}

void DS18B20_delay1us()	//@22.1184MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 2;
	while (--i);
}



void DS18B20_Delay70us()		//@22.1184MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 2;
	j = 125;
	do
	{
		while (--j);
	} while (--i);
}

bit DS18B20_Init(void)
{
	
	bit flag = 0;
	DS_DQ = 1;
	//稍作延时
	DS18B20_delay1us();	
	//单片机将DQ拉低
	DS_DQ = 0;          
	//精确延时 大于 480us
	DS18B20_delay40us(14);       
	DS_DQ = 1;
	// 400us延时后 如果x=1则初始化成功 x=0则初始化失败 
	DS18B20_delay40us(10);
	flag = DS_DQ;
	return flag;
}



void DS18B20_WriteByte(unsigned char dat)
{

	unsigned char i = 0;
	for(i=0;i<8;i++)
	{
		DS_DQ = 1;
		DS18B20_delay1us();
		DS18B20_delay1us();
		DS_DQ = 0;
		if(dat&0x01)
		{
			DS18B20_delay1us();
			DS18B20_delay1us();
			DS18B20_delay1us();
			DS_DQ = 1;
		}
		else
		{
			DS_DQ = 0;
			
		}
		DS18B20_Delay70us();
		dat >>= 1;
	}

}


unsigned char DS18B20_ReadByte()
{
	unsigned char i = 0 , dat = 0;
	
	
	for (i = 0 ; i<8 ; i++)
	{
		DS_DQ = 1;
		DS18B20_delay1us();
		DS18B20_delay1us();
		DS_DQ = 0;
		DS18B20_delay1us();
		DS18B20_delay1us();
		dat >>= 1;
		DS_DQ = 1;
		DS18B20_delay1us();
		DS18B20_delay1us();
		DS18B20_delay1us();
		DS18B20_delay1us();
		if(DS_DQ)
			dat |= 0x80;
		DS18B20_Delay70us();
	}
	return dat;
}


float DS18B20_ReadTemperature(void) 
{
	bit flag = 0;
	float T=0; 
	unsigned int tmpvalue = 0;
	flag  = DS18B20_Init(); 
	if(flag)
	{
		DS18B20_WriteByte(DS18B20_SkipROM); // 跳过读序号列号的操作 
		DS18B20_WriteByte(DS18B20_ConvertTemp); // 启动温度转换 
		flag  = DS18B20_Init();
		if(!flag) return 0;
		DS18B20_WriteByte(DS18B20_SkipROM); //跳过读序号列号的操作 
		DS18B20_WriteByte(DS18B20_ReadSCR); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度 
		
		
		
		for(tmpvalue=0; tmpvalue<9; tmpvalue++)
		{
			DS18B20_DATA[tmpvalue] = DS18B20_ReadByte();
		}
		
		if(CRC8_Table(DS18B20_DATA,8) == DS18B20_DATA[8])
		{
			tmpvalue = DS18B20_DATA[1];
			tmpvalue <<= 8;
			tmpvalue |= DS18B20_DATA[0];

			//使用DS18B20的默认分辨率12位, 精确度为0.0625度, 即读回数据的最低位代表0.0625度
			T = tmpvalue * 0.0625;
			//将它放大100倍,实数变成了整数， 使显示时可显示小数点后两位, 并对小数点后第三进行4舍5入
			//如t=11.0625, 进行计数后, 得到value = 1106, 即11.06 度
			//如t=-11.0625, 进行计数后, 得到value = -1106, 即-11.06 度
			T = (T * 100 + (tmpvalue > 0 ? 0.5 : -0.5))/100; //大于0加0.5, 小于0减0.5
			return T;
		}
		
		return 1;
 
	}
	return 0;
}



