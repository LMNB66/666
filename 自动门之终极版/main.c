#include <REGX52.H>

sbit dj_Signal=P2^0;
sbit beep=P1^0;
unsigned char time, dj_Angle, dj_Busy, a, b;
unsigned int dj_RST;

void delay(unsigned int m)
{
	unsigned char i, j;
	while(m--)
	{
		i = 18;
		j = 235;
		do
		{
			while (--j);
		} while (--i);
	}
}

void Timer0Init()
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TL0 = (65536-100)%256;	
	TH0 = (65536-100)/256;	
	TF0 = 0;
	TR0 = 1;
	ET0=1;
	EA=1;
	PT0=1;
}

void CK_Init()
{
	SCON=0x50;
	PCON|=0x80;
	TMOD&=0x0f;
	TMOD|=0x20;
	TH1=0xfa;
	TL1=0xfa;
	ET1=0;
	TR1=1;
	ES=1;
	EA=1;
	PS=0;
}

void CK_send(unsigned char i)
{
	SBUF=i;
	while(!TI);
	TI=0;
}

void main()
{
	a=0;
	b=0;
	beep=1;
	dj_Signal=0;
	time=0;
	dj_Angle=21;
	dj_Busy=1;
	dj_RST=0;
	Timer0Init();
	CK_Init();
	while(1)
	{
		if(SBUF>=0xe7&&dj_Busy==1)
		{
			beep=0;
			delay(6);
			beep=1;
			delay(75);
			a=0;
			dj_Signal=0;
			time=0;
			dj_Angle=SBUF-0xe6;
			dj_Busy=0;
			TR0=1;		
		}
		if(dj_Angle!=21&&a==1)
		{
			delay(40);
			beep=0;
			delay(6);
			beep=1;
			dj_RST=0;
		}
		if(dj_Angle==21&&b==1)
		{
			delay(100);
			b=0;
			TR0=0;
		}	
	}
}

void dj_PWM () interrupt 1
{
	TL0 = (65536-100)%256;	
	TH0 = (65536-100)/256;
	time++;
	dj_RST++;
	if(time<=dj_Angle)  //4,9,13,17,21
	{
		dj_Signal=1;
	}
	else
	{
		dj_Signal=0;
	}
	if(time==168)
	{
		time=0;
		if(dj_Angle!=21)
		{
			a++;
			if(a>=2)
			{
				a=2;
			}
		}
		else
		{
			b++;
			if(b>=2)
			{
				b=2;
			}
		}
	}
	if(dj_RST==30000)
	{
		dj_RST=0;
		dj_Angle=21;
	}
}

void __CK() interrupt 4
{
	if(RI==1)
	{
		CK_send(SBUF);
		RI=0;
		dj_Busy=1;
	}
}