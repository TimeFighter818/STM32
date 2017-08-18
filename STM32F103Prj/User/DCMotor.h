#ifndef __DCMOTOR_H__
#define __DCMOTOR_H__
#include "stm32f10x.h"

#define DCMotor_TIM3_COUNTER_CLK_FREQ 24000000  //Timer3计数器时钟频率
#define DCMotor_TIM3_PWM_FREQ 12000 //Timer3 PWM的频率

//连接方式：本示例程序共连接2个298驱动模块，可驱动四个直流电机
//电机1、2连接到298驱动模块1，分别对应IN1/IN2/ENA和IN3/IN4/ENB
//电机3、4连接到298驱动模块2，分别对应IN1/IN2/ENA和IN3/IN4/ENB
//TIM3 四路PWM输出PB0、PB1、PA6、PA7分别接模块1的ENA、ENB和模块2的ENA、ENB
//PB10、PB11为模块1的IN3、IN4
//PC4、PC5为模块1的IN1、IN2
//PA4、PA5为模块2的INI1、IN2
//PC13、PC15为模块2的IN3、IN4

#define MOTOR1_DIR_PIN1 GPIOA,GPIO_Pin_4   //定义电机1的IN1引脚
#define MOTOR1_DIR_PIN2 GPIOA,GPIO_Pin_5  	//定义电机1的IN2引脚
#define MOTOR1_EN_PIN GPIOA,GPIO_Pin_6

#define MOTOR2_DIR_PIN1 GPIOC,GPIO_Pin_13
#define MOTOR2_DIR_PIN2 GPIOC,GPIO_Pin_15
#define MOTOR2_EN_PIN GPIOA,GPIO_Pin_7

#define MOTOR3_DIR_PIN1 GPIOC,GPIO_Pin_4
#define MOTOR3_DIR_PIN2 GPIOC,GPIO_Pin_5
#define MOTOR3_EN_PIN GPIOB,GPIO_Pin_0

#define MOTOR4_DIR_PIN1 GPIOB,GPIO_Pin_10
#define MOTOR4_DIR_PIN2 GPIOB,GPIO_Pin_11
#define MOTOR4_EN_PIN GPIOB,GPIO_Pin_1

//如果上述引脚定义改了，请注意到main.c中的GPIO_Config函数中做相应端口的初始化

#define MOTOR1_FWD GPIO_SetBits(MOTOR1_DIR_PIN1);GPIO_ResetBits(MOTOR1_DIR_PIN2) 
#define MOTOR1_REV GPIO_ResetBits(MOTOR1_DIR_PIN1);GPIO_SetBits(MOTOR1_DIR_PIN2) 
#define MOTOR1_STP GPIO_SetBits(MOTOR1_DIR_PIN1);GPIO_SetBits(MOTOR1_DIR_PIN2) 

#define MOTOR2_FWD GPIO_SetBits(MOTOR2_DIR_PIN1);GPIO_ResetBits(MOTOR2_DIR_PIN2)  
#define MOTOR2_REV GPIO_ResetBits(MOTOR2_DIR_PIN1);GPIO_SetBits(MOTOR2_DIR_PIN2)
#define MOTOR2_STP GPIO_SetBits(MOTOR2_DIR_PIN1);GPIO_SetBits(MOTOR2_DIR_PIN2) 

#define MOTOR3_FWD GPIO_SetBits(MOTOR3_DIR_PIN1);GPIO_ResetBits(MOTOR3_DIR_PIN2)   
#define MOTOR3_REV GPIO_ResetBits(MOTOR3_DIR_PIN1);GPIO_SetBits(MOTOR3_DIR_PIN2)
#define MOTOR3_STP GPIO_SetBits(MOTOR3_DIR_PIN1);GPIO_SetBits(MOTOR3_DIR_PIN2) 

#define MOTOR4_FWD GPIO_SetBits(MOTOR4_DIR_PIN1);GPIO_ResetBits(MOTOR4_DIR_PIN2)   
#define MOTOR4_REV GPIO_ResetBits(MOTOR4_DIR_PIN1);GPIO_SetBits(MOTOR4_DIR_PIN2)
#define MOTOR4_STP GPIO_SetBits(MOTOR4_DIR_PIN1);GPIO_SetBits(MOTOR4_DIR_PIN2) 

void DCMotor_Init(void);
void DCMotor_Cmd(FunctionalState NewState);
void DCMotor_SetSpeed(int8_t nSpeed1,int8_t nSpeed2,int8_t nSpeed3,int8_t nSpeed4);
void DCMotor_TIM3PWM_SetOC1Duty(uint8_t nDuty);
void DCMotor_TIM3PWM_SetOC2Duty(uint8_t nDuty);
void DCMotor_TIM3PWM_SetOC3Duty(uint8_t nDuty);
void DCMotor_TIM3PWM_SetOC4Duty(uint8_t nDuty);
#endif
