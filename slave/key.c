#include "key.h"



unsigned char code key_code[]={0xee,0xde,0xbe,0x7e,0xed,0xdd,0xbd,0x7d,0xeb,0xdb,0xbb,0x7b,0xe7,0xd7,0xb7,0x77};
unsigned char keyscan()
{
	
	unsigned char scan1,scan2,keycode;
	KEY_IO = 0xf0;
	scan1 = KEY_IO;
	if((scan1&0xf0)!=0xf0) //�ж��Ƿ��а�������
	{
		unsigned char i, j;
		i = 108;
		j = 145;
		do
		{
			while (--j);
		} while (--i);
		
		scan1=KEY_IO;
		if((scan1&0xf0)!=0xf0)
		{
			KEY_IO=0x0f; //�߷�ת���Ĺؼ�
			scan2=KEY_IO;
			keycode=scan1|scan2; //��ϳɼ�ֵ��
			//���ó���ֵ
			for(j=0;j<16;j++)
			{
				if(keycode==key_code[j])
				{
					//beep();
					while(KEY_IO==scan2);
					return j;
				}
			}
		}
		else
			KEY_IO=0xff;//P1�ڿ���д��״̬
	}
	return 16;
}