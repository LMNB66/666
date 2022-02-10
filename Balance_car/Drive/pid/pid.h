#ifndef _PID_H_
#define _PID_H_

#include "sys.h"

#define theta_offset	2

extern float Kp;
extern float Ki;
extern float Kd;
extern float Ktp;

extern float thetaP, thetaI, thetaD;

extern int powerL, powerR;

void PID_control(void);

#endif
