#include "config.h"


/* UART interrupt function */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	  u8 test[] = "OK\r\n";
	  /* Debug uart5 */
	  if(huart->Instance == UART5)
		{
				unsigned char ch[2] = {'O','K'};
				HAL_UART_Transmit(huart,ch,2,1000);
				while(__HAL_UART_GET_FLAG(&huart5,UART_FLAG_TC) != SET);
				return;
	  }
		/* usart1 DXL */
		if(huart->Instance == USART1)
		{
			  
			  /* Pull the data out of the BUFFER */
			  	  /* Transmit the data */
				DXL_SendData(test,4);
			  DXL_SendData(DXL_aRxBuffer,DXL_RXBUFFSIZE);
			  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14|GPIO_PIN_15);
//			  DXL_ReceiveData();
			  return;
		}
		/*  USB2UART  */
		if(huart->Instance == UART4)
		{
//		    USB2UART_SendData(test,4);
			  USB2UART_SendData(USB2UART_aRxBuffer,USB2UART_RXBUFFSIZE);
//			  USB2UART_SendData("\r\n",2);
			  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14|GPIO_PIN_15);
		}
		
}







/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
