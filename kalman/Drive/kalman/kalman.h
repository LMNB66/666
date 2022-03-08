#ifndef _KALMAN_H_
#define _KALMAN_H_

#include "sys.h"

extern short accX, accY, accZ;

extern short gyroX, gyroY, gyroZ;

extern float theta_deg, theta_deg2;

extern float theta_dot, theta_dot2; 

void kalman_init(void);
void kalman_filter(void);

#endif
