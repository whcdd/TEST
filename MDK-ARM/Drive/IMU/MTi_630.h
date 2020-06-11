#ifndef __MTI_630_H
#define __MTI_630_H

#include "config.h"


/** @defgroup IMU BUFFER SIZE
  * @{
  */
#define MTi_630_RXBUFFSIZE 5
/**
  * @}
  */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern u8 MTi_630_aRxBuffer[MTi_630_RXBUFFSIZE];
extern u8 MTi_630_flag;
extern DMA_HandleTypeDef hdma_usart2_tx;

/* Function declaration --------------------------------------------------------*/

/* Initialization and functions  ****************************/
void MTi_630_Init(u32 baud);
void MTi_630_DMA_Init_DMA_Init(void);
void MTi_630_ReceiverTimeoutInit(void);

/* IMU operation functions *****************************************************/
void MTi_630_SendData(u8 *databuf, u8 len);

#endif
