#ifndef __STEPMOTOR_H__
#define __STEPMOTOR_H__
#include "stm32f10x.h"
//使用TB6560控制步进电机，输出三个控制端：脉冲、方向、使能
//主要思路：使用TIM2作为步进电机输出脉冲，CH1-CH4分别对应PA0-PA3，可以带动4路

#define STEPMOTOR1_DIR_PIN GPIOC,GPIO_Pin_0   //定义电机1的方向引脚
#define STEPMOTOR1_EN_PIN GPIOC,GPIO_Pin_14			//定义电机1的使能引脚
#define STEPMOTOR2_DIR_PIN GPIOC,GPIO_Pin_1
#define STEPMOTOR2_EN_PIN GPIOC,GPIO_Pin_14
#define STEPMOTOR3_DIR_PIN GPIOC,GPIO_Pin_2
#define STEPMOTOR3_EN_PIN GPIOC,GPIO_Pin_14
#define STEPMOTOR4_DIR_PIN GPIOC,GPIO_Pin_3
#define STEPMOTOR4_EN_PIN GPIOC,GPIO_Pin_14

#define STEPMOTOR1_FWD GPIO_SetBits(STEPMOTOR1_DIR_PIN) 
#define STEPMOTOR1_REV GPIO_ResetBits(STEPMOTOR1_DIR_PIN)

#define STEPMOTOR2_FWD GPIO_SetBits(STEPMOTOR2_DIR_PIN) 
#define STEPMOTOR2_REV GPIO_ResetBits(STEPMOTOR2_DIR_PIN)

#define STEPMOTOR3_FWD GPIO_SetBits(STEPMOTOR3_DIR_PIN) 
#define STEPMOTOR3_REV GPIO_ResetBits(STEPMOTOR3_DIR_PIN)

#define STEPMOTOR4_FWD GPIO_SetBits(STEPMOTOR4_DIR_PIN) 
#define STEPMOTOR4_REV GPIO_ResetBits(STEPMOTOR4_DIR_PIN)

void StepMotor_Init(void); //初始化
void StepMotor_SetFreq(uint32_t nFreq); //设置脉冲频率50-100000Hz
void StepMotor_Cmd(FunctionalState NewState); //使能电机
uint8_t StepMotor_SetSteps(uint8_t nCH, int32_t nSteps); //设置通道nCH产生nSteps个脉冲，可以有正负，返回1表示设置成功，返回0表示失败。
void StepMotor_Run(void);  //在TIM2溢出中断中调用，每溢出一次Steps减一，直到0，把占空比设为0，等于无脉冲输出

#endif
