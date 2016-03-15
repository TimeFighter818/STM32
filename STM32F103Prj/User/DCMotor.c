#include "DCMotor.h"
#include "TIM3PWM.h"

void DCMotor_Init(void)
{
	//端口配置在GPIO_Config函数中进行，如果那边没有配置，请在这里配置
	TIM3PWM_Init();
	MOTOR1_FWD;
	MOTOR2_FWD;
	MOTOR3_FWD;
	MOTOR4_FWD;
}
void DCMotor_SetSpeed(int8_t nPWM1,int8_t nPWM2,int8_t nPWM3,int8_t nPWM4)
{
}