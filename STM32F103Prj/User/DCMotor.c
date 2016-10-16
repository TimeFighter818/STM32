#include "DCMotor.h"

uint16_t DCMotor_PrescalerValue = 0;
uint32_t DCMotor_ARRValue = 0;	
	
void DCMotor_Init(void)
{
	//端口配置在GPIO_Config函数中进行，如果那边没有配置，请在这里配置

	//初始化TIM3为四个PWM通道
		  /* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
    clock at 24 MHz the Prescaler is computed as following:
     - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
    and Connectivity line devices and to 24 MHz for Low-Density Value line and
    Medium-Density Value line devices

    The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
                                                  = 24 MHz / 666 = 36 KHz
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
  ----------------------------------------------------------------------- */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//先停止电机
	DCMotor_Cmd(DISABLE);
	
	
  /* Compute the prescaler value */
	//PrescalerValue的计算方法：系统时钟频率/TIM3计数时钟频率 - 1
  DCMotor_PrescalerValue = (uint16_t) (SystemCoreClock / DCMotor_TIM3_COUNTER_CLK_FREQ) - 1;
  /* Time base configuration */
	//使TIM3输出频率（即PWM频率）的计算方法：TIM3计数时钟频率/（ARR+1），这里的ARR就是TIM_Period的值，设成1999，如果TIM3计数时钟频率为24M，则PWM频率为12KHz
	DCMotor_ARRValue = (uint16_t) (DCMotor_TIM3_COUNTER_CLK_FREQ/DCMotor_TIM3_PWM_FREQ) - 1;
  TIM_TimeBaseStructure.TIM_Period = DCMotor_ARRValue;
  TIM_TimeBaseStructure.TIM_Prescaler = DCMotor_PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
	
	//TIM3初始化结束
	
	//使能电机
	MOTOR1_FWD;
	MOTOR2_FWD;
	MOTOR3_FWD;
	MOTOR4_FWD;
	
}

//使能电机，1-使能，0-禁止
void DCMotor_Cmd(FunctionalState NewState)
{
	if(NewState != DISABLE)
	{
		GPIO_SetBits(MOTOR1_EN_PIN);
		GPIO_SetBits(MOTOR2_EN_PIN);
		GPIO_SetBits(MOTOR3_EN_PIN);
		GPIO_SetBits(MOTOR4_EN_PIN);

		TIM_Cmd(TIM3, ENABLE);
		
	}
	else
	{
		TIM_Cmd(TIM3, DISABLE);
		
		GPIO_ResetBits(MOTOR1_EN_PIN);
		GPIO_ResetBits(MOTOR2_EN_PIN);
		GPIO_ResetBits(MOTOR3_EN_PIN);
		GPIO_ResetBits(MOTOR4_EN_PIN);
	}
}

//设置四个通道的电机速度，速度范围-100~100
void DCMotor_SetSpeed(int8_t nSpeed1,int8_t nSpeed2,int8_t nSpeed3,int8_t nSpeed4)
{
	if(nSpeed1 == 0)
	{
		MOTOR1_STP;
		DCMotor_TIM3PWM_SetOC1Duty(100); //占空比为100，则使得ENA脚保持高电平
	}
	else if(nSpeed1 < 0)
	{
		MOTOR1_REV;
		DCMotor_TIM3PWM_SetOC1Duty(-nSpeed1);
	}
	else
	{
		MOTOR1_FWD;
		DCMotor_TIM3PWM_SetOC1Duty(nSpeed1);
	}
	
	if(nSpeed2 == 0)
	{
		MOTOR2_STP;
		DCMotor_TIM3PWM_SetOC2Duty(100);		
	}
	else if(nSpeed2 < 0)
	{
		MOTOR2_REV;
		DCMotor_TIM3PWM_SetOC2Duty(-nSpeed2);
	}
	else
	{
		MOTOR2_FWD;
		DCMotor_TIM3PWM_SetOC2Duty(nSpeed2);
	}
	
	if(nSpeed3 == 0)
	{
		MOTOR3_STP;
		DCMotor_TIM3PWM_SetOC3Duty(100);		
	}
	else if(nSpeed3 < 0)
	{
		MOTOR3_REV;
		DCMotor_TIM3PWM_SetOC3Duty(-nSpeed3);
	}
	else
	{
		MOTOR3_FWD;
		DCMotor_TIM3PWM_SetOC3Duty(100-nSpeed3);
	}
	
	if(nSpeed4 == 0)
	{
		MOTOR4_STP;
		DCMotor_TIM3PWM_SetOC4Duty(100);		
	}
	else if(nSpeed4 < 0)
	{
		MOTOR4_REV;
		DCMotor_TIM3PWM_SetOC4Duty(-nSpeed4);
	}
	else
	{
		MOTOR4_FWD;
		DCMotor_TIM3PWM_SetOC4Duty(100-nSpeed4);
	}
}

void DCMotor_TIM3PWM_SetOC1Duty(uint8_t nDuty)
{
	if(nDuty>100) TIM3->CCR1 = DCMotor_ARRValue;
	else TIM3->CCR1 = (DCMotor_ARRValue * nDuty / 100);
}

void DCMotor_TIM3PWM_SetOC2Duty(uint8_t nDuty)
{
	if(nDuty>100) TIM3->CCR2 = DCMotor_ARRValue;
	else TIM3->CCR2 = (DCMotor_ARRValue * nDuty / 100);
}

void DCMotor_TIM3PWM_SetOC3Duty(uint8_t nDuty)
{
	if(nDuty>100) TIM3->CCR3 = DCMotor_ARRValue;
	else TIM3->CCR3 = (DCMotor_ARRValue * nDuty / 100);
}

void DCMotor_TIM3PWM_SetOC4Duty(uint8_t nDuty)
{
	if(nDuty>100) TIM3->CCR4 = DCMotor_ARRValue;
	else TIM3->CCR4 = (DCMotor_ARRValue * nDuty / 100);
}
