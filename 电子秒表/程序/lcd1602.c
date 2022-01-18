#include "lcd1602.h"

unsigned char Busy()
{
	bit result;
	RS=0;
	RW=1;
	E=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	result=BF;
	E=0;
	return result;
}

void Write_Command(unsigned char Command)
{
	while(Busy()==1);
	E=0;
	RS=0;
	RW=0;
	_nop_();
	_nop_();
	DATA=Command;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E=0;

}

void Write_Data(unsigned char Data)
{
	while(Busy()==1);
	E=0;
	RS=1;
	RW=0;
	DATA=Data;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E=1;
	Delay(1);
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E=0;
}

void lcd1602_Init()
{
	Delay(15);
	Write_Command(0x38);
	Delay(5);
	Write_Command(0x01);
	Delay(5);
	Write_Command(0x06);
	Delay(5);
	Write_Command(0x0c);
	Delay(5);
	Write_Command(0x80);
	Delay(5);
}

void Show_Data(unsigned row, column, content)
{
	switch (row)
	{
		case 1:column=column-1;break;
		case 2:column=column-1+0x40;break;
	}
	Write_Command(column|0x80);
	Write_Data(content);
}

void Show_Str(unsigned row, column, unsigned char str[])
{
	unsigned char i;
	for(i=0;str[i]!='\0';i++)
	{
		Show_Data(row, column+i, str[i]);
	}
}

