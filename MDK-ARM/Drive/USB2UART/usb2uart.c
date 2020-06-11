#include "usb2uart.h"


/* External variables ---------------------------------------------------------*/
UART_HandleTypeDef huart4;
u8 USB2UART_aRxBuffer[USB2UART_RXBUFFSIZE] = {0};
DMA_HandleTypeDef hdma_usart4_tx;


/**
  * @brief USB2UART UART4 Initialization Function
  * @param baud
  * @retval None
  */
void USB2UART_Init(u32 baud)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = baud;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_8;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */
//		__HAL_UART_ENABLE_IT(&huart4,UART_IT_RXNE);
  HAL_UART_Receive_IT(&huart4,USB2UART_aRxBuffer,USB2UART_RXBUFFSIZE);
  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART4  Send Motor Data 
  * @param databuf: the starting address of the buffer memory    
  * @param len: the size of the buffer
  * @retval None
  */
void USB2UART_SendData(u8 *databuf, u8 len)
{
	  /* Transmit the data */
	  HAL_UART_Transmit(&huart4,databuf,len,1000);
	  while(__HAL_UART_GET_FLAG(&huart4,UART_FLAG_TC) != SET);
}

/**
  * @brief USART4 DMA Initialization Function
  * @param NONE
  * @retval None
  */
void USB2UART_DMA_Init(void) 
{

  /* DMA controller clock enable */
		__HAL_RCC_DMA1_CLK_ENABLE();
			/* USART4 DMA Init */
    /* USART4_TX Init */
    hdma_usart4_tx.Instance = DMA1_Stream6;
    hdma_usart4_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart4_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart4_tx.Init.Mode = DMA_NORMAL;
    hdma_usart4_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart4_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(&huart4,hdmatx,hdma_usart4_tx);
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
void USB2UART_ReceiverTimeoutInit(void)
{
	  /* Set the timeout of 2 character times (2*10bit) */
		HAL_UART_ReceiverTimeout_Config(&huart4,20);
    HAL_UART_EnableReceiverTimeout(&huart4);
}
