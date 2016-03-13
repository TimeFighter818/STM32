#include "stm32f10x.h"
#include "misc.h"
#include "delay.h"
#include "TIM3PWM.h"

//RCC_Configuration(), NVIC_Configuration(), GPIO_Config() ������ǰҪ����һ�¡�
void RCC_Configuration(void); //ϵͳʱ������
void NVIC_Configuration(void); //�ж�����
void GPIO_Config(void); //ͨ����������˿�����

uint16_t nDuty=0;
int main(void)
{
	RCC_Configuration(); //ʱ�ӳ�ʼ��
	GPIO_Config(); //�˿ڳ�ʼ��
	NVIC_Configuration();  //�жϳ�ʼ��
	
	TIM3PWM_Init();
	while(1)
	{
		/*
		Delay_ms(1);
		GPIO_SetBits(GPIOC,GPIO_Pin_1);   //��PC1��1
		Delay_ms(1);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);  //��PC1��0
		*/
		
		//����TIM3PWM
		nDuty++;
		if(nDuty>100)nDuty=0;
		TIM3PWM_SetOC1Duty(nDuty);
		TIM3PWM_SetOC2Duty(nDuty);
		TIM3PWM_SetOC3Duty(nDuty);
		TIM3PWM_SetOC4Duty(nDuty);
		Delay_ms(1000);
		
	}
	
}

/*ϵͳʱ�����ú���*/
void RCC_Configuration(void)
{
	
//        SystemInit();//Դ��system_stm32f10x.c�ļ�,ֻ��Ҫ���ô˺���,������RCC��Ĭ������.�����뿴2_RCC
	
				//���ǻ����Լ������ð�
	      RCC_DeInit();

				RCC_HSEConfig(RCC_HSE_OFF);  //��ֹ�ⲿʱ��

        RCC_HSICmd(ENABLE);          //���ڲ�ʱ��
        
        while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)        
        {        
        }

//        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

//        FLASH_SetLatency(FLASH_Latency_2);
				
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        //APB2
        RCC_PCLK2Config(RCC_HCLK_Div1);
        //APB1
        RCC_PCLK1Config(RCC_HCLK_Div1);
        //PLL ��Ƶ
        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);        //�ڲ�ʱ�ӱ�Ƶ*16���ڲ�����8MHz������2����PLL����Ƶ16����PLL���64MHz
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
  /* ���ȼ��� ˵������ռ���ȼ����õ�λ�����������ȼ����õ�λ��   ��������1�� 7 */    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		   
  

	  /* Enable the RTC Interrupt ʹ��ʵʱʱ���ж�*/
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;					//�����ⲿ�ж�Դ�����жϣ� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
	
	/*ʹ�ܶ�ʱ�ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;												//ָ���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;							
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;										//ָ����Ӧ���ȼ���
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 
	

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
	
	//ʹ��GPIOA/GPIOB/GPIOC/GPIOD�����ߣ�����˿ڲ��ܹ����������������˿ڣ����Բ���ʹ�ܡ�
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);	

	//����PA3Ϊ������Դ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
	//���ò�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				 //PC1��PC2��PC3	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;			//PB1��PB2	     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	//ʹ��TIM3������·PWM�˿�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //ʹ��TIM3ʱ��
	
	/* PA6��PA7��PB0��PB1 �ֱ���TIM3���ĸ����ͨ�� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
