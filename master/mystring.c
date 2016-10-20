#include "mystring.h"


void array_push(char c,char *tmp,unsigned int len)
{
	
	unsigned int t_len=0;
	while(*tmp != '\0')
	{
		t_len++;
		tmp++;
	}

	if(t_len<len)
	{
		*tmp=c;
		tmp++;
		*tmp='\0';
	}	
	
}
void re_str(char *str,unsigned int len)
{
	unsigned int i;
	for(i=0;i<len;i++)
	{
		str[i]=0;	
	}	
}
/* void kill_zero()
{
	
} */