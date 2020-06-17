#ifndef __MTI_630_H
#define __MTI_630_H

#include "config.h"


/** @defgroup IMU BUFFER SIZE
  * @{
  */
#define MTi_630_RXBUFFSIZE 65
/**
  * @}
  */
	
/** @defgroup checkSumComplement Status
  * @{
  */
#define VALID   1
#define INVALID 0
/**
  * @}
  */
	

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern u8 MTi_630_aRxBuffer0[MTi_630_RXBUFFSIZE];
extern u8 MTi_630_aRxBuffer1[MTi_630_RXBUFFSIZE];

extern u8 MTi_630_flag;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;

/* Function declaration --------------------------------------------------------*/

/* Initialization and functions  ****************************/
void MTi_630_Init(u32 baud);

/* IMU operation functions *****************************************************/
void MTi_630_SendData(u8 *databuf, u8 len);

#endif
