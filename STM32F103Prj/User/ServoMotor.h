#ifndef __SERVOMOTOR_H__
#define __SERVOMOTOR_H__
#include "stm32f10x.h"
//主要思路：使用TIM4输出四路50Hz的PWM，占空比1/20 ~ 2/20

void ServoMotor_Init(void); //初始化TIM4四路PWM通道
//void ServoMotor_Cmd(uint8_t nCH, FunctionalState NewState); //开启或关闭nCH通道的PWM输出，通过设置占空比为0来关闭PWM输出
uint8_t ServoMotor_SetPulse(uint8_t nCH, uint16_t nPulse); //设置通道nCH的占空比，数值在1000~2000，分别对应1/20~2/20。如果设为0，则无脉冲输出，相当于关了舵机。如果不在这个范围内返回0，否则返回1
uint16_t ServoMotor_GetPulse(uint8_t nCH); //获取通道nCH当前设置的占空比值

#endif
