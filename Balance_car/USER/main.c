/*****************************************************
@bilibili: 	LMNB66
@data    :	2022/2/2
@version :	V6.0
@auth    :	LMNB66

代码只有直立环和转向环，卡尔曼滤波和PID参考了日本网站
制作平衡小车“M5StickC で倒立振子”中的内容，使用的模块
有oled7针，MPU6050，LMs2596DC-DC，L298n(给两路相同的
PWM，输出电压似乎不一样)，主控STM32F103C6T6，电机为TT
马达，没有编码器。
之所以使用卡尔曼滤波，是因为官方DMP库太大了，我这个芯
片FLASH只有32KB，加上oled都差不多30KB了。
然后调参就用串口来调参，不用每次都下载程序
目前只实现了直立，但是遥控还不行，遥控左右转还是可以
的，只是前进后退不行，会加速倒下或者一直在晃


mpu6050: SCL-->B6	||		oled: 	D0 -->A5
		 SDA-->B7	||		  		D1 -->A7
					||				RES-->B1
					||				DC -->B10
					||				CS -->B11
					||
L298n:   IN1-->A0	||	Bluetooth: 	TXD-->A10
		 IN2-->A1	||				RXD-->A9
         IN3-->A2	||
		 IN4-->A3	||
******************************************************/


#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "mpu6050.h"
#include "kalman.h"	
#include "pwm.h"
#include "tim.h"
#include "pid.h"


int main(void)
{
	delay_init();
	uart_init(9600);
	OLED_Init();
	OLED_Clear(0);				//清屏，黑色背景
	MPU_Init();
	ini_theta();				//卡尔曼滤波初始化
	TIM2_PWM_Init(999, 143);	//PWM0-999，频率500Hz
	TIM3_Init(24, 7199);		//2.5ms执行中断
	while(1)
	{
		//这个显示有BUG，如果数据长度变短了，多出后面的数据不会清除
		GUI_ShowFloat(0, 0, theta_deg2+theta_offset, 2, 8, 1);	//角度
		GUI_ShowFloat(42, 0, theta_dot2, 2, 8, 1);				//角速度
		
		GUI_ShowFloat(0, 20, powerL, 0, 8, 1);				//左电机PID导出结果
		GUI_ShowFloat(42, 20, powerR, 0, 8, 1);				//右电机PID导出结果
		
		GUI_ShowFloat(0, 40, Kp, 0, 8, 1);
		GUI_ShowFloat(30, 40, Ki, 1, 8, 1);
		GUI_ShowFloat(60, 40, Kd, 1, 8, 1);
		GUI_ShowFloat(90, 40, Ktp, 2, 8, 1);
	}
}
