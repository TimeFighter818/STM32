#ifndef __TIM3PWM_H
#define __TIM3PWM_H
#include "stm32f10x.h"
#define TIM3_COUNTER_CLK_FREQ 24000000  //Timer3������ʱ��Ƶ��
#define TIM3_PWM_FREQ 12000 //Timer3 PWM��Ƶ��
void TIM3PWM_Init(void);
void TIM3PWM_SetOC1Duty(uint8_t nDuty);
void TIM3PWM_SetOC2Duty(uint8_t nDuty);
void TIM3PWM_SetOC3Duty(uint8_t nDuty);
void TIM3PWM_SetOC4Duty(uint8_t nDuty);
#endif
