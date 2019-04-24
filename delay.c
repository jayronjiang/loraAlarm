#include "stm8s.h"
#include "main.h"
#include "delay.h"

void delay_ms(unsigned int ms)
{
	unsigned int i,j;
	
	for(j=0;j<ms;j++)
	{
		for(i=0;i<650;i++)
		{
			delay_us(1);
		}
	}
}

void delay_us(unsigned int us)
{
	NOP();
}




































