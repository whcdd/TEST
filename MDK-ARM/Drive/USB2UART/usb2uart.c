#include "usb2uart.h"


/* External variables ---------------------------------------------------------*/
UART_HandleTypeDef huart4;
u8 USB2UART_aRxBuffer[USB2UART_RXBUFFSIZE] = {0};



/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
void MX_UART4_Init(u32 baud)
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
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */
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

