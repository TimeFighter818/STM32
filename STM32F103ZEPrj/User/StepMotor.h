#ifndef __STEPMOTOR_H__
#define __STEPMOTOR_H__
#include "stm32f10x.h"
//ʹ��TB6560���Ʋ������������������ƶˣ����塢����ʹ��
//��Ҫ˼·��ʹ��TIM2��Ϊ�������������壬CH1-CH4�ֱ��ӦPA0-PA3�����Դ���4·

#define STEPMOTOR1_DIR_PIN GPIOC,GPIO_Pin_0   //������1�ķ�������
#define STEPMOTOR1_EN_PIN GPIOC,GPIO_Pin_14			//������1��ʹ������
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

void StepMotor_Init(void); //��ʼ��
void StepMotor_SetFreq(uint32_t nFreq); //��������Ƶ��50-100000Hz
void StepMotor_Cmd(FunctionalState NewState); //ʹ�ܵ��
uint8_t StepMotor_SetSteps(uint8_t nCH, int32_t nSteps); //����ͨ��nCH����nSteps�����壬����������������1��ʾ���óɹ�������0��ʾʧ�ܡ�
void StepMotor_Run(void);  //��TIM2����ж��е��ã�ÿ���һ��Steps��һ��ֱ��0����ռ�ձ���Ϊ0���������������

#endif
