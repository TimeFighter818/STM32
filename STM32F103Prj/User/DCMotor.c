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
void DCMotor_SetSpeed(int8_t nPWM1,int8_t nPWM2,int8_t nPWM3,int8_t nPWM4)
{
}