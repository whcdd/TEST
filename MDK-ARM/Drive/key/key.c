#include "key.h"


void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOB_CLK_ENABLE();


		/*Configure GPIO pins : PB12 PB13 */
		GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		/* EXTI interrupt init*/
		HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	  HAL_Delay(50);
    switch(GPIO_Pin)
		{
			case GPIO_PIN_12:
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12) == GPIO_PIN_RESET)
				  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
			  break;
				
			case GPIO_PIN_13:
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13) == GPIO_PIN_RESET)
				  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
			  break;
		}
}
