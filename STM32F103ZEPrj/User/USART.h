#ifndef __USART_H__
#define __USART_H__
#include "stm32f10x.h"
#include "stdio.h"
#include "stdarg.h"

//使用usart1，对应PA9/TX，PA10/RX，
//端口初始化在USART.c文件中的USART1_Init函数中；
//中断设置在main.c文件中的RCC_Configuration函数中，
//中断响应在stm32f10x_it.c文件中USART1_IRQHandler函数中

#define CMD_BYTE_LENGTH 6 //串口通讯一帧数据的字节数，譬如6个字节为一个完整的数据帧，第一个字节代表命令类型，第2~5字节是命令参数，第6个字节为帧结束标志（0xFF）
#define CMD_BYTE_END 0xFF //数据帧结束字节标志

//这里把数据帧接收标志和接收缓冲区定义成可供usart.c以外的源代码直接使用，只要include了USART.h文件。
extern __IO uint8_t USART_Rx1Buff[CMD_BYTE_LENGTH]; //接收缓冲区
extern __IO uint8_t USART_FrameFlag; //接收完整数据帧标志，1完整，0不完整

void USART1_Init(void);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
char *itoa(int value, char *string, int radix);
int fputc(int ch, FILE *f);
void USART_GetChar(uint8_t nChar); //串口接收到一个字节
void USART_Process(void);


#endif
