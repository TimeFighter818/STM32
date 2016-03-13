#include "stm32f10x.h"
#include "TIM3PWM.h"
uint16_t PrescalerValue = 0;
uint16_t ARRValue = 0;

//TIM3初始化，四个PWM通道初始化，占空比初始为0
void TIM3PWM_Init(void)
{
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
  /* Compute the prescaler value */
	//PrescalerValue的计算方法：系统时钟频率/TIM3计数时钟频率 - 1
  PrescalerValue = (uint16_t) (SystemCoreClock / TIM3_COUNTER_CLK_FREQ) - 1;
  /* Time base configuration */
	//使TIM3输出频率（即PWM频率）的计算方法：TIM3计数时钟频率/（ARR+1），这里的ARR就是TIM_Period的值，设成1999，则PWM频率为12KHz
	ARRValue = (uint16_t) (TIM3_COUNTER_CLK_FREQ/TIM3_PWM_FREQ) - 1;
  TIM_TimeBaseStructure.TIM_Period = ARRValue;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
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
}

void TIM3PWM_SetOC1Duty(uint8_t nDuty)
{
	if(nDuty>100) TIM3->CCR1 = ARRValue;
	else TIM3->CCR1 = (ARRValue / 100) * nDuty;
}

void TIM3PWM_SetOC2Duty(uint8_t nDuty)
{
	if(nDuty>100) TIM3->CCR2 = ARRValue;
	else TIM3->CCR2 = (ARRValue / 100) * nDuty;
}

void TIM3PWM_SetOC3Duty(uint8_t nDuty)
{
	if(nDuty>100) TIM3->CCR3 = ARRValue;
	else TIM3->CCR3 = (ARRValue / 100) * nDuty;
}

void TIM3PWM_SetOC4Duty(uint8_t nDuty)
{
	if(nDuty>100) TIM3->CCR4 = ARRValue;
	else TIM3->CCR4 = (ARRValue / 100) * nDuty;
}
