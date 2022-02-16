#include "pid.h"
#include "usart.h"
#include "kalman.h"
#include "pwm.h"
#include "math.h"

//直立环参数
float 	Kp = -100,
		Ki = -0.5,
		Kd = -2.5;
//转向环参数
float 	Ktp = 0.03;

//机械中值偏离角度
float   theta_offset = 0.2;

//偏差，偏差积分，偏差微分
float thetaP=0, thetaI=0, thetaD=0;

//电机输出PWM
int powerL, powerR;

//转向，前后
float mX=0, mY=0;

//====================================================
//转向环
//====================================================
float Turn(void)
{
	if(mX==0)	return -Ktp* gyroZ;//左右旋转的角速度
	else 		return mX;
}

//====================================================
//直立环PID处理
//====================================================
void PID_control(void)
{
	static float thetaI_last=0, thetaI_Low;

	//机械中值为0，所以不用减去0，theta_deg2+theta_offset使机械中值为0
	thetaP =theta_deg2+theta_offset-mY;
	
	//积分，低通滤波使数据平滑
	thetaI_Low = 0.3*thetaP+ 0.7*thetaI_last;
	thetaI_last = thetaI_Low;
	thetaI += thetaI_Low;
	
	//角度微分就是角速度
	thetaD = theta_dot2;
	
	//小于1°就不积分了，我这会慢慢变大然后就又动起来了
	if(fabs(theta_deg2+theta_offset) < 1)
	{
		thetaI = 0;
		thetaI_last=0;
	}
	
	//pidout = 偏差*Kp + 偏差积分*Ki + 偏差微分*Kd
	powerL = thetaP * Kp + thetaI * Ki + thetaD * Kd-Turn();
	powerR = thetaP * Kp + thetaI * Ki + thetaD * Kd+Turn();
	
	//输出PWM限幅
	powerL = fmax(-1000, fmin(1000, powerL));
	powerR = fmax(-1000, fmin(1000, powerR));
	
	//角度大于30°时停止控制，已经救不回来了
	if(fabs(theta_deg2+theta_offset) > 30 || (powerL==0 && powerR==0))
	{
		PWM_LoadL(0, 0);
		PWM_LoadR(0, 0);
		
		thetaI = 0;
		thetaI_last=0;
	}
	else
	{
		if(powerL>0) 			PWM_LoadL(powerL, 0);		//前进
		else if(powerL<0) 		PWM_LoadL(0, -1*powerL);	//后退
		
		if(powerR>0) 			PWM_LoadR(powerR, 0);		//前进
		else if(powerR<0) 		PWM_LoadR(0, -1*powerR);	//后退
	}
}
