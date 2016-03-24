#include "stm32f10x.h"
#include "misc.h"
#include "delay.h"
#include "DCMotor.h"
#include "StepMotor.h"
#include "ServoMotor.h"
#include "USART.h"
//RCC_Configuration(), NVIC_Configuration(), GPIO_Config() ������ǰҪ����һ�¡�
void RCC_Configuration(void); //ϵͳʱ������
void NVIC_Configuration(void); //�ж�����
void GPIO_Config(void); //ͨ����������˿�����

int8_t nSpeed=0;
uint16_t nServoPulse=1000;
int main(void)
{
	uint8_t i;
	
	RCC_Configuration(); //ʱ�ӳ�ʼ��
	GPIO_Config(); //�˿ڳ�ʼ��
	NVIC_Configuration();  //�жϳ�ʼ��
	
	//����ֱ�����
	DCMotor_Init();
	
	DCMotor_Cmd(ENABLE);
	
	//���Բ������
	StepMotor_Init();
	StepMotor_Cmd(ENABLE);
	StepMotor_SetSteps(1,1000000);
	StepMotor_SetSteps(3,-1000000);
	
	//���Զ��
	ServoMotor_Init();
	
	//���Դ���1
	USART1_Init();  //Ҫ�Ĳ�����ʲô���뵽������ȥ�ģ�Ĭ����115200
	
	while(1)
	{
		
		Delay_ms(1000);
		DCMotor_SetSpeed(nSpeed,nSpeed,nSpeed,nSpeed);
		nSpeed++;
		if(nSpeed>90) nSpeed = -90;
		
		//StepMotor_SetSteps(1,1000000);
		
		ServoMotor_SetPulse(1,nServoPulse);
		ServoMotor_SetPulse(2,nServoPulse);
		ServoMotor_SetPulse(3,nServoPulse);
		ServoMotor_SetPulse(4,nServoPulse);
		nServoPulse += 10;
		if(nServoPulse>2000) nServoPulse = 1000;
		
		//���Դ���
		USART_Process();   //���Ե�����������
		
		//���߿���ֱ��ʹ������֡��־λ�Լ����ջ���������
		if(USART_FrameFlag == 1)
		{
			for(i=0;i<CMD_BYTE_LENGTH;i++)
			{
				USART_SendData(USART1,USART_Rx1Buff[i]);
			}
			
			USART_FrameFlag = 0;
		}
		
	}
	
}

/*ϵͳʱ�����ú���*/
void RCC_Configuration(void)
{
	
//        SystemInit();//Դ��system_stm32f10x.c�ļ�,ֻ��Ҫ���ô˺���,������RCC��Ĭ������.�����뿴2_RCC
	
				//���ǻ����Լ������ð�
	      RCC_DeInit();

				RCC_HSEConfig(RCC_HSE_ON);  //ʹ���ⲿ8MHz����
        
        while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)        
        {        
        }

//        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

//        FLASH_SetLatency(FLASH_Latency_2);
				
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        //APB2
        RCC_PCLK2Config(RCC_HCLK_Div1);
        //APB1
        RCC_PCLK1Config(RCC_HCLK_Div2);
        //PLL ��Ƶ
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);        //�ⲿ����*9���ⲿ����8MHz������PLL����Ƶ9����PLL���72MHz
        RCC_PLLCmd(ENABLE);                        										//ʹ�ܱ�Ƶ
                                                                                                         
				//�ȴ�ָ���� RCC ��־λ���óɹ� �ȴ�PLL��ʼ���ɹ�
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        //��PLL�����Ϊϵͳʱ�ӡ�

        /**************************************************
        ��ȡRCC����Ϣ,������
        ��ο�RCC_ClocksTypeDef�ṹ�������,��ʱ��������ɺ�,
        ���������ֵ��ֱ�ӷ�ӳ�������������ֵ�����Ƶ��
        ***************************************************/
                
        while(RCC_GetSYSCLKSource() != 0x08){}		//����0x08˵��ʹ��PLL��Ϊϵͳʱ�ӳɹ�
					
}

/*�ж����ú���*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;  //NVIC�ж������ṹ��
	RCC_ClocksTypeDef RCC_Clocks;  //RCCʱ�ӽṹ��
  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* ���ȼ��� ˵������ռ���ȼ����õ�λ�����������ȼ����õ�λ����������2��2*/    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		   
  

//	  /* Enable the RTC Interrupt ʹ��ʵʱʱ���ж�*/
//  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;					//�����ⲿ�ж�Դ�����жϣ� 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);  
	
	//ʹ��USART1�ж�
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	
	/*ʹ�ܶ�ʱTIM2�ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;												//ָ���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;							
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;										//ָ����Ӧ���ȼ���
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 
	
	//�������ʱ���ж�
	RCC_GetClocksFreq(&RCC_Clocks);		//��ȡϵͳʱ��
	if (SysTick_Config(RCC_Clocks.SYSCLK_Frequency/100000))		   //ʱ�ӽ����ж�ʱ10usһ��  ���ڶ�ʱ�� ����ʱ��Ƶ��48MHz��/480����10usһ��
  { 
    /* Capture error */ 
    while (1);
  } 
}

/*�˿����ú���*/
void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//ʹ��GPIOA/GPIOD�����ߣ�����˿ڲ��ܹ����������������˿ڣ����Բ���ʹ�ܡ�
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);	

	//����PA8ΪLED0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
	//����PD2ΪLED1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //PD2	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	//ʹ��TIM3ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	//PA6��PA7��PB0��PB1 ��ΪTIM3�ĸ�PWM���ͨ������Ϊֱ�����PWM�ź�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//����PA4\PA5\PC4\PC5�ֱ���Ƶ���ķ���PC13Ϊ����ʹ�ܶ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//ʹ��TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	
	//����PA0\PA1\PA2\PA3 ��ΪTIM2�ĸ�PWM���ͨ������Ϊ�������CLK�ź�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	//����PC0\PC1\PC2\PC3 �ֱ���Ʋ�������ķ���PC14Ϊ�������ʹ�ܶ�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	
		//ʹ��TIM4ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  
	
	//����PB6\PB7\PB8\PB9 ��ΪTIM4�ĸ�PWM���ͨ������Ϊ��������ź�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	
		/*ʹ��  USART1 ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	/* USART1 GPIO config */ 
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
}
