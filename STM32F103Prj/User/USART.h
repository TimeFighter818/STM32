#ifndef __USART_H__
#define __USART_H__
#include "stm32f10x.h"
#include "stdio.h"
#include "stdarg.h"

//ʹ��usart1����ӦPA9/TX��PA10/RX��
//�˿ڳ�ʼ����USART.c�ļ��е�USART1_Init�����У�
//�ж�������main.c�ļ��е�RCC_Configuration�����У�
//�ж���Ӧ��stm32f10x_it.c�ļ���USART1_IRQHandler������

#define CMD_BYTE_LENGTH 6 //����ͨѶһ֡���ݵ��ֽ�����Ʃ��6���ֽ�Ϊһ������������֡����һ���ֽڴ����������ͣ���2~5�ֽ��������������6���ֽ�Ϊ֡������־��0xFF��
#define CMD_BYTE_END 0xFF //����֡�����ֽڱ�־

//���������֡���ձ�־�ͽ��ջ���������ɿɹ�usart.c�����Դ����ֱ��ʹ�ã�ֻҪinclude��USART.h�ļ���
extern __IO uint8_t USART_Rx1Buff[CMD_BYTE_LENGTH]; //���ջ�����
extern __IO uint8_t USART_FrameFlag; //������������֡��־��1������0������

void USART1_Init(void);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
char *itoa(int value, char *string, int radix);
int fputc(int ch, FILE *f);
void USART_GetChar(uint8_t nChar); //���ڽ��յ�һ���ֽ�
void USART_Process(void);


#endif
