#ifndef __SERVOMOTOR_H__
#define __SERVOMOTOR_H__
#include "stm32f10x.h"
//��Ҫ˼·��ʹ��TIM4�����·50Hz��PWM��ռ�ձ�1/20 ~ 2/20

void ServoMotor_Init(void); //��ʼ��TIM4��·PWMͨ��
//void ServoMotor_Cmd(uint8_t nCH, FunctionalState NewState); //������ر�nCHͨ����PWM�����ͨ������ռ�ձ�Ϊ0���ر�PWM���
uint8_t ServoMotor_SetPulse(uint8_t nCH, uint16_t nPulse); //����ͨ��nCH��ռ�ձȣ���ֵ��1000~2000���ֱ��Ӧ1/20~2/20�������Ϊ0����������������൱�ڹ��˶����������������Χ�ڷ���0�����򷵻�1
uint16_t ServoMotor_GetPulse(uint8_t nCH); //��ȡͨ��nCH��ǰ���õ�ռ�ձ�ֵ

#endif
