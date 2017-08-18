#ifndef __DCMOTOR_H__
#define __DCMOTOR_H__
#include "stm32f10x.h"

#define DCMotor_TIM3_COUNTER_CLK_FREQ 24000000  //Timer3������ʱ��Ƶ��
#define DCMotor_TIM3_PWM_FREQ 12000 //Timer3 PWM��Ƶ��

//���ӷ�ʽ����ʾ����������2��298����ģ�飬�������ĸ�ֱ�����
//���1��2���ӵ�298����ģ��1���ֱ��ӦIN1/IN2/ENA��IN3/IN4/ENB
//���3��4���ӵ�298����ģ��2���ֱ��ӦIN1/IN2/ENA��IN3/IN4/ENB
//TIM3 ��·PWM���PB0��PB1��PA6��PA7�ֱ��ģ��1��ENA��ENB��ģ��2��ENA��ENB
//PB10��PB11Ϊģ��1��IN3��IN4
//PC4��PC5Ϊģ��1��IN1��IN2
//PA4��PA5Ϊģ��2��INI1��IN2
//PC13��PC15Ϊģ��2��IN3��IN4

#define MOTOR1_DIR_PIN1 GPIOA,GPIO_Pin_4   //������1��IN1����
#define MOTOR1_DIR_PIN2 GPIOA,GPIO_Pin_5  	//������1��IN2����
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

//����������Ŷ�����ˣ���ע�⵽main.c�е�GPIO_Config����������Ӧ�˿ڵĳ�ʼ��

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
