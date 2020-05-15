#ifndef __DEBUG_UART5
#define __DEBUG_UART5

#include "main.h"
#include "config.h"

#define RXBUFFSIZE 2

extern unsigned char aRxBuffer[RXBUFFSIZE];
extern UART_HandleTypeDef huart5;

void Debug_UART5_Init(unsigned int baud);


#endif
