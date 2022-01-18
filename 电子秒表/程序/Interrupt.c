#include "Interrupt.h"

void Timer0_Init()
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TL0 = 0x00;
	TH0 = 0xCA;
	TF0 = 0;
	TR0 = 0;
	ET0=1;
	EA=1;
	PT0=1;
}

void Timer1_Init()
{
	TMOD &= 0x0F;
	TMOD |= 0x10;
	TL1 = 0x00;
	TH1 = 0xDC;
	TF1 = 0;
	TR1 = 0;
	ET1=1;
	EA=1;
	PT1=0;
}