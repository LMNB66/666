#include "Key.h"

unsigned char RC=0;

void Key()
{
	if(KEY!=0x0f&&a==0)
	{
		switch (KEY)
		{
			case (0x07):RC=1;break;
			case (0x0b):RC=2;break;
			case (0x0d):RC=3;break;
			case (0x0e):RC=4;break;
		}
		KEY=0xf0;
		switch (KEY)
		{
			case (0x70):RC=RC;break;
			case (0xb0):RC=RC+4;break;
			case (0xd0):RC=RC+8;break;
			case (0xe0):RC=RC+12;break;
		}
		a=1;
	}
}