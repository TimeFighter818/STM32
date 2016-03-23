#include "StepMotor.h"

__IO uint32_t StepMotor_Freq = 1000;
__IO uint32_t StepMotor_CH1Steps=0;
__IO uint32_t StepMotor_CH2Steps=0;
__IO uint32_t StepMotor_CH3Steps=0;
__IO uint32_t StepMotor_CH4Steps=0;

void StepMotor_Init(void) //初始化
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	StepMotor_Cmd(DISABLE);  //先禁止步进电机
	
  /* Compute the prescaler value */
	//PrescalerValue的计算方法：系统时钟频率/TIM2计数时钟频率（固定为2MHz） - 1
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 2000000) - 1;
  /* Time base configuration */
	//使TIM2输出频率（即PWM频率）的计算方法：TIM2计数时钟频率/（ARR+1），这里的ARR就是TIM_Period的值，设成1999，如果TIM2计数时钟频率为2M，则PWM频率为1KHz
  TIM_TimeBaseStructure.TIM_Period = (uint16_t) (2000000/StepMotor_Freq) - 1;

  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //打开溢出中断

  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}

//使能电机，1-使能，0-禁止
void StepMotor_Cmd(FunctionalState NewState)
{
	if(NewState != DISABLE)
	{
		GPIO_SetBits(STEPMOTOR1_EN_PIN);
		GPIO_SetBits(STEPMOTOR2_EN_PIN);
		GPIO_SetBits(STEPMOTOR3_EN_PIN);
		GPIO_SetBits(STEPMOTOR4_EN_PIN);
		
	}
	else
	{
		GPIO_ResetBits(STEPMOTOR1_EN_PIN);
		GPIO_ResetBits(STEPMOTOR2_EN_PIN);
		GPIO_ResetBits(STEPMOTOR3_EN_PIN);
		GPIO_ResetBits(STEPMOTOR4_EN_PIN);
	}
}

void StepMotor_SetFreq(uint32_t nFreq) //设置脉冲频率
{
	StepMotor_Freq = nFreq; //设置新的频率值
	StepMotor_Cmd(DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	StepMotor_Init(); //重新初始化下TIM2
	StepMotor_Cmd(ENABLE);
	
}

uint8_t StepMotor_SetSteps(uint8_t nCH, int32_t nSteps)
{
	switch(nCH)
	{
		case 1:
			if(StepMotor_CH1Steps>0) return 0; //上一次还没走完，返回错误
			if(nSteps<0)
			{
				StepMotor_CH1Steps = -nSteps;
				STEPMOTOR1_REV;
			}
			else
			{
				StepMotor_CH1Steps = nSteps;
				STEPMOTOR1_FWD;
			}
			TIM2->CCR1 = (uint16_t) (1000000/StepMotor_Freq) - 1;  //设置通道一的占空比为50%；
			break;
		case 2:
			if(StepMotor_CH2Steps>0) return 0; //上一次还没走完，返回错误
			if(nSteps<0)
			{
				StepMotor_CH2Steps = -nSteps;
				STEPMOTOR2_REV;
			}
			else
			{
				StepMotor_CH2Steps = nSteps;
				STEPMOTOR2_FWD;
			}
			TIM2->CCR2 = (uint16_t) (1000000/StepMotor_Freq) - 1;
			break;
		case 3:
			if(StepMotor_CH3Steps>0) return 0; //上一次还没走完，返回错误
			if(nSteps<0)
			{
				StepMotor_CH3Steps = -nSteps;
				STEPMOTOR3_REV;
			}
			else
			{
				StepMotor_CH3Steps = nSteps;
				STEPMOTOR3_FWD;
			}
			TIM2->CCR3 = (uint16_t) (1000000/StepMotor_Freq) - 1;
			break;
		case 4:
			if(StepMotor_CH4Steps>0) return 0; //上一次还没走完，返回错误
			if(nSteps<0)
			{
				StepMotor_CH4Steps = -nSteps;
				STEPMOTOR4_REV;
			}
			else
			{
				StepMotor_CH4Steps = nSteps;
				STEPMOTOR4_FWD;
			}
			TIM2->CCR4 = (uint16_t) (1000000/StepMotor_Freq) - 1;
			break;
		default:
			return 0;
	}
	return 1;
}

void StepMotor_Run(void)  //在TIM2溢出中断中调用，每溢出一次Steps减一，直到0，把占空比设为0，等于无脉冲输出
{
	//CH1
	if(StepMotor_CH1Steps>0)
	{
		StepMotor_CH1Steps--;
		if(StepMotor_CH1Steps == 0) TIM2->CCR1 = 0;  //如果已经减到0，则把占空比设为0
	}
	//CH2
	if(StepMotor_CH2Steps>0)
	{
		StepMotor_CH1Steps--;
		if(StepMotor_CH2Steps == 0) TIM2->CCR2 = 0;
	}
	//CH3
	if(StepMotor_CH3Steps>0)
	{
		StepMotor_CH3Steps--;
		if(StepMotor_CH3Steps == 0) TIM2->CCR3 = 0;
	}
	//CH4
	if(StepMotor_CH4Steps>0)
	{
		StepMotor_CH4Steps--;
		if(StepMotor_CH4Steps == 0) TIM2->CCR4 = 0;
	}
}
