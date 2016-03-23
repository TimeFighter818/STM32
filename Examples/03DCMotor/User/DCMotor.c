#include "DCMotor.h"
#include "TIM3PWM.h"

void DCMotor_Init(void)
{
	//�˿�������GPIO_Config�����н��У�����Ǳ�û�����ã�������������
	TIM3PWM_Init();
	MOTOR1_FWD;
	MOTOR2_FWD;
	MOTOR3_FWD;
	MOTOR4_FWD;
}

//�����ĸ�ͨ���ĵ���ٶȣ��ٶȷ�Χ-100~100
void DCMotor_SetSpeed(int8_t nSpeed1,int8_t nSpeed2,int8_t nSpeed3,int8_t nSpeed4)
{
	if(nSpeed1 < 0)
	{
		MOTOR1_REV;
		TIM3PWM_SetOC1Duty(-nSpeed1);
	}
	else
	{
		MOTOR1_FWD;
		TIM3PWM_SetOC1Duty(100-nSpeed1);
	}
	
	if(nSpeed2 < 0)
	{
		MOTOR2_REV;
		TIM3PWM_SetOC2Duty(-nSpeed2);
	}
	else
	{
		MOTOR2_FWD;
		TIM3PWM_SetOC2Duty(100-nSpeed2);
	}
	
	if(nSpeed3 < 0)
	{
		MOTOR3_REV;
		TIM3PWM_SetOC3Duty(-nSpeed3);
	}
	else
	{
		MOTOR3_FWD;
		TIM3PWM_SetOC3Duty(100-nSpeed3);
	}
	
	if(nSpeed4 < 0)
	{
		MOTOR4_REV;
		TIM3PWM_SetOC4Duty(-nSpeed4);
	}
	else
	{
		MOTOR4_FWD;
		TIM3PWM_SetOC4Duty(100-nSpeed4);
	}
}
