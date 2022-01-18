#ifndef __FUNCTION_H__
#define	__FUNCTION_H__

#include <REGX52.H>
#include "Key.h"
#include "lcd1602.h"
#include "Delay.h"

#define N 5

extern bit a;
extern unsigned char re;
extern unsigned char pag;
extern unsigned char Look;
extern unsigned char ssec, sec, min;
extern unsigned char re_ms[N], re_s[N], re_m[N];

void Key_fun();
void Key_scan();
void Init_face();
void major_face();
void Pag_change();
void Record_face();
void Time_face(unsigned char r, c, ssec, sec, min);

#endif