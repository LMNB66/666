#ifndef _pwm_H
#define _pwm_H

#include "sys.h"

void TIM2_PWM_Init(u16 arr, u16 psc);
void PWM_LoadL(uint16_t OC1, uint16_t OC2);
void PWM_LoadR(uint16_t OC3, uint16_t OC4);

#endif
