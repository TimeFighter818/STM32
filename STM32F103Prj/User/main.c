#include "stm32f10x.h"
#include "misc.h"
#include "delay.h"
#include "DCMotor.h"
#include "StepMotor.h"
#include "ServoMotor.h"
#include "USART.h"
//RCC_Configuration(), NVIC_Configuration(), GPIO_Config() 在引用前要声明一下。
void RCC_Configuration(void); //系统时钟配置
void NVIC_Configuration(void); //中断配置
void GPIO_Config(void); //通用输入输出端口配置

int8_t nSpeed=0;
uint16_t nServoPulse=1000;
int main(void)
{
	uint8_t i;
	
	RCC_Configuration(); //时钟初始化
	GPIO_Config(); //端口初始化
	NVIC_Configuration();  //中断初始化
	
	//测试直流电机
	DCMotor_Init();
	
	DCMotor_Cmd(ENABLE);
	
	//测试步进电机
	StepMotor_Init();
	StepMotor_Cmd(ENABLE);
	StepMotor_SetSteps(1,1000000);
	StepMotor_SetSteps(3,-1000000);
	
	//测试舵机
	ServoMotor_Init();
	
	//测试串口1
	USART1_Init();  //要改波特率什么的请到函数里去改，默认是115200
	
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
		
		//测试串口
		USART_Process();   //可以调用这个命令处理
		
		//或者可以直接使用完整帧标志位以及接收缓冲区变量
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

/*系统时钟配置函数*/
void RCC_Configuration(void)
{
	
//        SystemInit();//源自system_stm32f10x.c文件,只需要调用此函数,则可完成RCC的默认配置.具体请看2_RCC
	
				//我们还是自己来配置吧
	      RCC_DeInit();

				RCC_HSEConfig(RCC_HSE_ON);  //使用外部8MHz晶振
        
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
        //PLL 倍频
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);        //外部晶振*9，外部晶振8MHz，进入PLL，倍频9，则PLL输出72MHz
        RCC_PLLCmd(ENABLE);                        										//使能倍频
                                                                                                         
				//等待指定的 RCC 标志位设置成功 等待PLL初始化成功
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        //以PLL输出作为系统时钟。

        /**************************************************
        获取RCC的信息,调试用
        请参考RCC_ClocksTypeDef结构体的内容,当时钟配置完成后,
        里面变量的值就直接反映了器件各个部分的运行频率
        ***************************************************/
                
        while(RCC_GetSYSCLKSource() != 0x08){}		//返回0x08说明使用PLL作为系统时钟成功
					
}

/*中断配置函数*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;  //NVIC中断向量结构体
	RCC_ClocksTypeDef RCC_Clocks;  //RCC时钟结构体
  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* 优先级组 说明了抢占优先级所用的位数，和子优先级所用的位数。这里是2，2*/    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		   
  

//	  /* Enable the RTC Interrupt 使能实时时钟中断*/
//  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;					//配置外部中断源（秒中断） 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);  
	
	//使能USART1中断
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	
	/*使能定时TIM2中断*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;												//指定中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;							
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;										//指定响应优先级别
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 
	
	//设置嘀嗒时钟中断
	RCC_GetClocksFreq(&RCC_Clocks);		//获取系统时钟
	if (SysTick_Config(RCC_Clocks.SYSCLK_Frequency/100000))		   //时钟节拍中断时10us一次  用于定时。 例如时钟频率48MHz，/480，则10us一次
  { 
    /* Capture error */ 
    while (1);
  } 
}

/*端口配置函数*/
void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能GPIOA/GPIOD的总线，否则端口不能工作，如果不用这个端口，可以不用使能。
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);	

	//配置PA8为LED0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
	//配置PD2为LED1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //PD2	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	//使能TIM3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	//PA6、PA7、PB0、PB1 设为TIM3四个PWM输出通道，作为直流电机PWM信号
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//配置PA4\PA5\PC4\PC5分别控制电机的方向，PC13为公共使能端
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//使能TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	
	//配置PA0\PA1\PA2\PA3 设为TIM2四个PWM输出通道，作为步进电机CLK信号
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	//配置PC0\PC1\PC2\PC3 分别控制步进电机的方向，PC14为步进电机使能端
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	
		//使能TIM4时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  
	
	//配置PB6\PB7\PB8\PB9 设为TIM4四个PWM输出通道，作为舵机控制信号
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	
		/*使能  USART1 时钟 */
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
