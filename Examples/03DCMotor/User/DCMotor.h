#ifndef __DCMOTOR_H__
#define __DCMOTOR_H__
#include "stm32f10x.h"
//连接方式：
//电机1、2连接到298驱动模块1，分别对应IN1/IN2和IN3/IN4
//电机3、4连接到298驱动模块2，分别对应IN1/IN2和IN3/IN4
#define MOTOR1_DIR_PIN GPIOC,GPIO_Pin_1   //定义电机1的方向引脚
#define MOTOR1_EN_PIN GPIOB,GPIO_Pin2			//定义电机1的使能引脚
#define MOTOR2_DIR_PIN GPIOC,GPIO_Pin_2
#define MOTOR2_EN_PIN GPIOB,GPIO_Pin2
#define MOTOR3_DIR_PIN GPIOC,GPIO_Pin_3
#define MOTOR3_EN_PIN GPIOB,GPIO_Pin2
#define MOTOR4_DIR_PIN GPIOC,GPIO_Pin_4
#define MOTOR4_EN_PIN GPIOB,GPIO_Pin2

#define MOTOR1_FWD GPIO_SetBits(MOTOR1_DIR_PIN) 
#define MOTOR1_REV GPIO_ResetBits(MOTOR1_DIR_PIN)

#define MOTOR2_FWD GPIO_SetBits(MOTOR2_DIR_PIN) 
#define MOTOR2_REV GPIO_ResetBits(MOTOR2_DIR_PIN)

#define MOTOR3_FWD GPIO_SetBits(MOTOR3_DIR_PIN) 
#define MOTOR3_REV GPIO_ResetBits(MOTOR3_DIR_PIN)

#define MOTOR4_FWD GPIO_SetBits(MOTOR4_DIR_PIN) 
#define MOTOR4_REV GPIO_ResetBits(MOTOR4_DIR_PIN)

void DCMotor_Init(void);
void DCMotor_SetSpeed(int8_t nSpeed1,int8_t nSpeed2,int8_t nSpeed3,int8_t nSpeed4);
#endif
