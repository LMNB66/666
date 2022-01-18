#include "Delay.h"

void Delay(unsigned char x)
{
	unsigned char i, j;
	while(x)
	{
		_nop_();
		i=2;
		j=199;
		do
		{
			while(--j);
		}while(--i);
		--x;
	}
}
