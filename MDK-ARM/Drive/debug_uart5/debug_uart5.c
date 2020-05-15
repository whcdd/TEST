#include "debug_uart5.h"

unsigned char aRxBuffer[RXBUFFSIZE] = {0};

UART_HandleTypeDef huart5;

/**
  * @brief UART5 Initialization Function
  * @param baud
  * @retval None
  */
void Debug_UART5_Init(unsigned int baud)
{
  huart5.Instance = UART5;
  huart5.Init.BaudRate = baud;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */
	HAL_UART_Receive_IT(&huart5,aRxBuffer,RXBUFFSIZE);
  /* USER CODE END UART5_Init 2 */

}

	
