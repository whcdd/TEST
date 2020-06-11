#ifndef  __USB2UART_H
#define  __USB2UART_H

#include "config.h"


#define USB2UART_RXBUFFSIZE 100

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart4;
extern u8 USB2UART_aRxBuffer[USB2UART_RXBUFFSIZE];

void USB2UART_Init(u32 baud);
void USB2UART_DMA_Init(void);
void USB2UART_ReceiverTimeoutInit(void);

void USB2UART_SendData(u8 *databuf, u8 len);

#endif
