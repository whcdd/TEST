#include "MTi_630.h"

/* External variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;
u8 MTi_630_aRxBuffer[MTi_630_RXBUFFSIZE] = {0};
u8 MTi_630_flag=0;

/**
* @brief IMU (USART2) Initialization Function
* @param baud
* @retval None
*/

void MTi_630_Init(u32 baud)
{

		 GPIO_InitTypeDef GPIO_InitStruct = {0};
		 
		/* RS232 Initialization*/
		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOA_CLK_ENABLE();

		/*Configure GPIO pin : PA4 PA5 PA6 PA7 */
		/*PA4: INVALID PA5:FORCE_ON PA6:#FORCE_OFF PA7:READY*/
		GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		 
	  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		 
		/*Configure GPIO pin Output Level */
		/* Enable MAX32327E */
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);
	
		huart2.Instance = USART2;
		huart2.Init.BaudRate = baud;
		huart2.Init.WordLength = UART_WORDLENGTH_8B;
		huart2.Init.StopBits = UART_STOPBITS_1;
		huart2.Init.Parity = UART_PARITY_NONE;
		huart2.Init.Mode = UART_MODE_TX_RX;
		huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart2.Init.OverSampling = UART_OVERSAMPLING_16;
		huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		if (HAL_UART_Init(&huart2) != HAL_OK)
		{
			Error_Handler();
		}
		HAL_UART_Receive_IT(&huart2,MTi_630_aRxBuffer,MTi_630_RXBUFFSIZE);
}

/**
  * @brief IMU  Send Data to IMU 
  * @param databuf: the starting address of the buffer memory    
  * @param len: the size of the buffer
  * @retval None
  */
void MTi_630_SendData(u8 *databuf, u8 len)
{
	  /* Transmit the data */
	  HAL_UART_Transmit(&huart2,databuf,len,1000);
	  while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC) != SET);
}


/**
  * @brief USART2 DMA Initialization Function
  * @param NONE
  * @retval None
  */
void MTi_630_DMA_Init(void) 
{

  /* DMA controller clock enable */
		__HAL_RCC_DMA1_CLK_ENABLE();
			/* USART2 DMA Init */
    /* USART2_TX Init */
    hdma_usart2_tx.Instance = DMA1_Stream6;
    hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//		hdma_usart2_tx.Init.MemBurst = DMA_MBURST_SINGLE;
//		hdma_usart2_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(&huart2,hdmatx,hdma_usart2_tx);
  /* DMA interrupt init */
  /* DMA1_Stream6_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/**
  * @brief  Update on the fly the receiver timeout value in RTOR register.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *                    the configuration information for the specified UART module.
  * @param  TimeoutValue receiver timeout value in number of baud blocks. The timeout
  *                     value must be less or equal to 0x0FFFFFFFF.
  * @retval None
  */
void MTi_630_ReceiverTimeoutInit(void)
{
	  /* Set the timeout of 2 character times (2*10bit) */
		HAL_UART_ReceiverTimeout_Config(&huart2,20);
    HAL_UART_EnableReceiverTimeout(&huart2);
}
