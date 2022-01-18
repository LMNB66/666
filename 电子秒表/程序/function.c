#include "function.h"

#define N 5//自定义可记录时间的个数

bit a;//防长按，避免连续执行按键对应的功能
unsigned char re=1;//记录次数，最大值为N
unsigned char pag=1;//页码数，共2页
unsigned char Look=1;//查阅第几页
unsigned char ssec=0, sec=0, min=0;//更新时间
unsigned char code rank[]={"NO."};//排名
unsigned char code record[]={"RECORD"};//记录头衔
unsigned char code time[]={"m  s  ms"};//时间头衔
unsigned char code cover[]={"STOPWATCH"};//封面
unsigned char re_ms[N]={0}, re_s[N]={0}, re_m[N]={0};//存储记录的时间
unsigned char code num[]={'0','1','2','3','4','5','6','7','8','9'};


/***************************************************************************
*名称:Init_face
*参数:无
*功能:进入时显示1.6s的封面，然后字母慢慢从后面消失
*返回值:无
***************************************************************************/
void Init_face()
{
	unsigned char i, j;
	Show_Str(1, 4, cover);//封面的英语
	for(i=0;i<8;i++)//大概1.6秒
	{
		Delay(200);	
	}
	for(i=0;i<9;i++)//慢慢退去，现象是字母从后面一个消失
	{
		for(j=0;j<9-i;j++)
		{
			Show_Data(1, 4+j, cover[j]);	
		}
		Delay(50);
		Write_Command(0x01);//清除显示寄存器的内容，不然不会有后面那个现象
		Write_Command(0x80);
	}
}


/***************************************************************************
*名称:major_face
*参数:无
*功能:显示时间头衔，毫秒对毫秒，秒对秒，分对分
*返回值:无
***************************************************************************/
void major_face()
{
	Show_Str(1, 5, time);
	Show_Data(2, 16, '1');//右下角显示1的页码
}


/***************************************************************************
*名称:Time_face
*参数:r, c, ssec, sec, min
*功能:显示行，列，毫秒，秒，分
*返回值:无
***************************************************************************/
void Time_face(unsigned char r, c, ssec, sec, min)
{
	Show_Data(r, c+7, num[ssec%10]);
	Show_Data(r, c+6, num[ssec/10]);
	Show_Data(r, c+5, 0x3a);
	Show_Data(r, c+4, num[sec%10]);
	Show_Data(r, c+3, num[sec/10]);
	Show_Data(r, c+2, 0x3a);
	Show_Data(r, c+1, num[min%10]);
	Show_Data(r, c, num[min/10]);
}


/***************************************************************************
*名称:Record_face
*参数:无
*功能:显示记录的面板，调用全局变量Look实现翻阅记录
*返回值:无
***************************************************************************/
void Record_face()
{
	unsigned char i, j;
	Show_Str(1, 6, record);
	Show_Str(2, 1, rank);
	Show_Data(2, 16, '2');//第2页
	i=Look;
	j=Look-1;
	if(re==1)i=0, j=0;//不记录时都显示0
	Show_Data(2, 4, i+'0');
	Time_face(2, 6, re_ms[j], re_s[j], re_m[j]);
}


/***************************************************************************
*名称:Pag_change
*参数:无
*功能:翻页功能，调用全局变量pag实现翻页
*返回值:无
***************************************************************************/
void Pag_change()
{
	Write_Command(0x01);
	Write_Command(0x80);
	switch(pag)
	{
		case 1:major_face();Time_face(2, 5, ssec, sec, min);break;
		case 2:Record_face();break;
	}
}


/***************************************************************************
*名称:Delete
*参数:无
*功能:清除记录的时间
*返回值:无
***************************************************************************/
void Delete()
{
	unsigned char i;
	for(i=0;i<re-1;i++)
	{
		re_ms[i]=0;
		re_s[i]=0;
		re_m[i]=0;
	}
}


/***************************************************************************
*名称:Key_scan
*参数:无
*功能:判断按键是否又被按下，按下后隔15毫秒进入中断，也就是消抖时间15毫秒
*返回值:无
***************************************************************************/
void Key_scan()
{
	KEY=0x0f;
	if(KEY==0x0f)a=0;//a是防长按，按一次就锁定，这样不至于长按连续执行按键功能
	if(KEY!=0x0f)TR0=1;

}


/***************************************************************************
*名称:Key_fun
*参数:无
*功能:按键的功能
*返回值:无
***************************************************************************/
void Key_fun()
{
	switch(RC)
	{
		case 13://页码加一
		{
			if(TR1==0)
			{
				pag++;
				if(pag>2)pag=2;Pag_change();
				Look=1;	
			}
			break;
		}	
		case 14://页码减一
		{
			if(TR1==0)
			{
				pag--;
				if(pag<1)pag=1;Pag_change();
				Look=1;	
			}
			break;
		}
		case 11://清零
		{
			if(TR1==0)//只有暂停时才可清零
			{
				ssec=0;
				sec=0;
				min=0;
				Time_face(2, 5, ssec, sec, min);
				Delete();
				re=1;
				if(pag==2)//如果是第一页，它显示会错乱
				{
					Write_Command(0x01);
					Write_Command(0x80);
					Record_face();//更新记录面板显示的内容，不然清零后没显示0					
				}
			}
			break;
		}
		case 9://暂停与开始
		{
			if(pag==1)TR1=~TR1;
			break;
		}
		case 10://记录时间数据
		{
			if(pag==1&&TR1==1)//要开始才可以记录
			{
				if(re<=N)
				{
					re_ms[re-1]=ssec;
					re_s[re-1]=sec;
					re_m[re-1]=min;
					re++;	
				}
			}break;
		}
		case 15://翻阅后面的时间记录
		{
			if(pag==2&&TR1==0)
			{
				Look++;
				if(Look>=re-1)Look=re-1;
				Record_face();//更新显示
			}break;
		}
		
		case 16://翻阅前面的时间记录
		{
			if(pag==2&&TR1==0)
			{
				Look--;
				if(Look<1)Look=1;
				Record_face();
			}break;
		}
	}
	RC=0;//清除按键数值，否则下次主函数循环又执行了按键的功能
}