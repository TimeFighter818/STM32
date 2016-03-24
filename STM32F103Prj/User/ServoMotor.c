#include "ServoMotor.h"

uint16_t nCHPulse[4];  //������һ�����õ�ͨ��CH1��������

void ServoMotor_Init(void) //��ʼ��TIM4��·PWMͨ��
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	nCHPulse[0]=0;
	nCHPulse[1]=0;
	nCHPulse[2]=0;
	nCHPulse[3]=0;
  /* Compute the prescaler value */
	//PrescalerValue�ļ��㷽����ϵͳʱ��Ƶ��/TIM4����ʱ��Ƶ�ʣ��̶�Ϊ1MHz�� - 1
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000000) - 1;
  /* Time base configuration */
	//ʹTIM4���Ƶ�ʣ���PWMƵ�ʣ��ļ��㷽����TIM4����ʱ��Ƶ��(1MHz)/��ARR+1���������ARR����TIM_Period��ֵ�����19999�����TIM4����ʱ��Ƶ��Ϊ1M����PWMƵ��Ϊ50Hz
  TIM_TimeBaseStructure.TIM_Period = 19999;

  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC2Init(TIM4, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC3Init(TIM4, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC4Init(TIM4, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //������ж�

  /* TIM3 enable counter */
  TIM_Cmd(TIM4, ENABLE);	
}

//void ServoMotor_Cmd(uint8_t nCH, FunctionalState NewState) //������ر�nCHͨ����PWM�����ͨ������ռ�ձ�Ϊ0���ر�PWM���
//{
//	if(NewState != DISABLE)
//	{
//		ServoMotor_SetPulse(nCH,nCHPulse[nCH-1]); //����ʱ����Ϊ�ϴε�ռ�ձ�
//	}
//	else
//	{
//		ServoMotor_SetPulse(nCH,0);
//	}
//}

uint8_t ServoMotor_SetPulse(uint8_t nCH, uint16_t nPulse) //����ͨ��nCH��ռ�ձȣ���ֵ��1000~2000���ֱ��Ӧ1/20~2/20�������Ϊ0����������������൱�ڹ��˶����������������Χ�ڷ���0�����򷵻�1
{
	if( (nPulse>0 && nPulse<1000) || nPulse>2000) return 0;
	switch(nCH)
	{
		case 1:
			TIM4->CCR1 = nPulse;  //����ͨ��һ��ռ�ձ�ΪnPulse/20000��
			nCHPulse[0] = nPulse;  //���浱ǰ����ֵ��������
			break;
		case 2:
			TIM4->CCR2 = nPulse;
			nCHPulse[1] = nPulse;
			break;
		case 3:
			TIM4->CCR3 = nPulse;
			nCHPulse[2] = nPulse;
			break;
		case 4:
			TIM4->CCR4 = nPulse;
			nCHPulse[3] = nPulse;
			break;
		default:
			return 0;
	}
	return 1;		
}

uint16_t ServoMotor_GetPulse(uint8_t nCH) //��ȡͨ��nCH��ǰ���õ�ռ�ձ�ֵ
{
	return nCHPulse[nCH];
}
