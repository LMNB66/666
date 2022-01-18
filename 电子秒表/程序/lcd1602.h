#ifndef __LCD1602_H__
#define	__LCD1602_H__

#include <REGX52.H>
#include <intrins.h>
#include "Delay.h"

#define DATA P0

sbit E=P2^7;
sbit RS=P2^6;
sbit RW=P2^5;
sbit BF=P0^7;

void lcd1602_Init();
unsigned char Busy();
void Write_Command(unsigned char Command);
void Show_Data(unsigned row, column, content);
void Show_Str(unsigned row, cloumn, unsigned char str[]);

#endif