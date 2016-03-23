#include "StepMotor.h"

__IO uint32_t StepMotor_Freq = 1000;
__IO uint32_t StepMotor_CH1Steps=0;
__IO uint32_t StepMotor_CH2Steps=0;
__IO uint32_t StepMotor_CH3Steps=0;
__IO uint32_t StepMotor_CH4Steps=0;

void StepMotor_Init(void) //��ʼ��
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	StepMotor_Cmd(DISABLE);  //�Ƚ�ֹ�������
	
  /* Compute the prescaler value */
	//PrescalerValue�ļ��㷽����ϵͳʱ��Ƶ��/TIM2����ʱ��Ƶ�ʣ��̶�Ϊ2MHz�� - 1
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 2000000) - 1;
  /* Time base configuration */
	//ʹTIM2���Ƶ�ʣ���PWMƵ�ʣ��ļ��㷽����TIM2����ʱ��Ƶ��/��ARR+1���������ARR����TIM_Period��ֵ�����1999�����TIM2����ʱ��Ƶ��Ϊ2M����PWMƵ��Ϊ1KHz
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
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //������ж�

  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}

//ʹ�ܵ����1-ʹ�ܣ�0-��ֹ
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

void StepMotor_SetFreq(uint32_t nFreq) //��������Ƶ��
{
	StepMotor_Freq = nFreq; //�����µ�Ƶ��ֵ
	StepMotor_Cmd(DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	StepMotor_Init(); //���³�ʼ����TIM2
	StepMotor_Cmd(ENABLE);
	
}

uint8_t StepMotor_SetSteps(uint8_t nCH, int32_t nSteps)
{
	switch(nCH)
	{
		case 1:
			if(StepMotor_CH1Steps>0) return 0; //��һ�λ�û���꣬���ش���
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
			TIM2->CCR1 = (uint16_t) (1000000/StepMotor_Freq) - 1;  //����ͨ��һ��ռ�ձ�Ϊ50%��
			break;
		case 2:
			if(StepMotor_CH2Steps>0) return 0; //��һ�λ�û���꣬���ش���
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
			if(StepMotor_CH3Steps>0) return 0; //��һ�λ�û���꣬���ش���
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
			if(StepMotor_CH4Steps>0) return 0; //��һ�λ�û���꣬���ش���
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

void StepMotor_Run(void)  //��TIM2����ж��е��ã�ÿ���һ��Steps��һ��ֱ��0����ռ�ձ���Ϊ0���������������
{
	//CH1
	if(StepMotor_CH1Steps>0)
	{
		StepMotor_CH1Steps--;
		if(StepMotor_CH1Steps == 0) TIM2->CCR1 = 0;  //����Ѿ�����0�����ռ�ձ���Ϊ0
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
