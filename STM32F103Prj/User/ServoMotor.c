#include "ServoMotor.h"

uint16_t nCHPulse[4];  //保存上一次设置的通道CH1的脉冲宽度

void ServoMotor_Init(void) //初始化TIM4四路PWM通道
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	nCHPulse[0]=0;
	nCHPulse[1]=0;
	nCHPulse[2]=0;
	nCHPulse[3]=0;
  /* Compute the prescaler value */
	//PrescalerValue的计算方法：系统时钟频率/TIM4计数时钟频率（固定为1MHz） - 1
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000000) - 1;
  /* Time base configuration */
	//使TIM4输出频率（即PWM频率）的计算方法：TIM4计数时钟频率(1MHz)/（ARR+1），这里的ARR就是TIM_Period的值，设成19999，如果TIM4计数时钟频率为1M，则PWM频率为50Hz
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
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //打开溢出中断

  /* TIM3 enable counter */
  TIM_Cmd(TIM4, ENABLE);	
}

//void ServoMotor_Cmd(uint8_t nCH, FunctionalState NewState) //开启或关闭nCH通道的PWM输出，通过设置占空比为0来关闭PWM输出
//{
//	if(NewState != DISABLE)
//	{
//		ServoMotor_SetPulse(nCH,nCHPulse[nCH-1]); //开启时，设为上次的占空比
//	}
//	else
//	{
//		ServoMotor_SetPulse(nCH,0);
//	}
//}

uint8_t ServoMotor_SetPulse(uint8_t nCH, uint16_t nPulse) //设置通道nCH的占空比，数值在1000~2000，分别对应1/20~2/20。如果设为0，则无脉冲输出，相当于关了舵机。如果不在这个范围内返回0，否则返回1
{
	if( (nPulse>0 && nPulse<1000) || nPulse>2000) return 0;
	switch(nCH)
	{
		case 1:
			TIM4->CCR1 = nPulse;  //设置通道一的占空比为nPulse/20000；
			nCHPulse[0] = nPulse;  //保存当前设置值到变量中
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

uint16_t ServoMotor_GetPulse(uint8_t nCH) //获取通道nCH当前设置的占空比值
{
	return nCHPulse[nCH];
}
