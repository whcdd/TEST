#include "MTi_630.h"

/* External variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;
u8 MTi_630_aRxBuffer0[MTi_630_RXBUFFSIZE] = {0};
u8 MTi_630_aRxBuffer1[MTi_630_RXBUFFSIZE] = {0};
u8 MTi_630_flag=0;


/* Private function prototypes -----------------------------------------------*/
/** @addtogroup UART_Private_Functions
  * @{
  */
static void MTi_630_DMA_Init(void);
static void MTi_630_ReceiverTimeoutInit(void);

/**
  * @}
  */

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
		huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
		/* Swap TX/RX */
		huart2.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
		if (HAL_UART_Init(&huart2) != HAL_OK)
		{
			Error_Handler();
		}
		
		/* Initialize USART RX DMA */
		MTi_630_DMA_Init();
		
		/* Enable UART DMA transmitter&receiver   */
		SET_BIT(huart2.Instance->CR3, USART_CR3_DMAT|USART_CR3_DMAR);
		
		/* Wait at least 500ms for entering measure mode */
		HAL_Delay(500);
}

/**
  * @brief USART2 DMA Initialization Function
  * @param NONE
  * @retval None
  */
static void MTi_630_DMA_Init(void) 
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
	 
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(&huart2,hdmatx,hdma_usart2_tx);
		
		/* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Stream5;
    hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(&huart2,hdmatx,hdma_usart2_rx);
		
		/* Set Peripheral and buffer address */
//		HAL_DMA_Start_IT(&hdma_usart2_rx,(u32)&huart2.Instance->RDR,(u32)MTi_630_aRxBuffer0,MTi_630_RXBUFFSIZE);
		HAL_DMAEx_MultiBufferStart_IT(&hdma_usart2_rx, (u32)&huart2.Instance->RDR, (u32)MTi_630_aRxBuffer0, (u32)MTi_630_aRxBuffer1, MTi_630_RXBUFFSIZE);
		
		/* DMA interrupt init */
		/* DMA1_Stream5_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

/**
  * @brief  Update on the fly the receiver timeout value in RTOR register.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *                    the configuration information for the specified UART module.
  * @param  TimeoutValue receiver timeout value in number of baud blocks. The timeout
  *                     value must be less or equal to 0x0FFFFFFFF.
  * @retval None
  */
static void MTi_630_ReceiverTimeoutInit(void)
{
	  /* Set the timeout of 2 character times (2*10bit) */
		HAL_UART_ReceiverTimeout_Config(&huart2,20);
    HAL_UART_EnableReceiverTimeout(&huart2);
	
	  /* Enable receive timeout interrupt */
		__HAL_UART_ENABLE_IT(&huart2,UART_IT_RTO);
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
