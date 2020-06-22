/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_it.h"
#include "debug_uart5.h"
#include "DynamixelMotor.h"
#include "config.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */
  
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) 
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  HAL_RCC_NMI_IRQHandler();
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
		u32 RxLen;
		if(__HAL_UART_GET_IT(&huart2,UART_IT_RTO))
		{
				__HAL_UART_CLEAR_IT(&huart2,UART_CLEAR_RTOF);
				
				/* Disable the DMA */
				__HAL_DMA_DISABLE(&hdma_usart2_rx);
			  
			  /* Derive the receiving data length from the NDTR register */
				RxLen = MTi_630_RXBUFFSIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
			
			  /* Clear DMA TCIF2& HTIF2 flag */
			  __HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_TCIF1_5);
				__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_TCIF1_5);
			  
			  /* Reset the DMA NDTR */
			  __HAL_DMA_SET_COUNTER(&hdma_usart2_rx,MTi_630_RXBUFFSIZE);
				
				/* Progress the reveiving data */
			  USB2UART_SendData(MTi_630_aRxBuffer0,RxLen);
				
			  /* Enable the DMA */
				__HAL_DMA_ENABLE(&hdma_usart2_rx);
				
		}
}

/**
  * @brief This function handles DMA1 stream5 hdma_usart2_rx global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
	volatile u32 RxLen;
	if(__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_TCIF1_5))
	{
			/* Clear DMA Transfer complete flag */
			__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_TCIF1_5);
		
			  
			/* Derive the receiving data length from the NDTR register */
			RxLen = MTi_630_RXBUFFSIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);	

		  
		  /* Progress the reveiving data */
			if(READ_BIT(hdma_usart2_rx.Instance->CR,DMA_SxCR_CT) == 0)
			{
					USB2UART_SendData(MTi_630_aRxBuffer0,MTi_630_RXBUFFSIZE);
			}
			else
			{
					USB2UART_SendData(MTi_630_aRxBuffer1,MTi_630_RXBUFFSIZE);
			}
	}
}

/**
  * @brief This function handles DMA1 stream6 global interrupt.
  */
void DMA1_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream6_IRQn 0 */

  /* USER CODE END DMA1_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
  /* USER CODE BEGIN DMA1_Stream6_IRQn 1 */

  /* USER CODE END DMA1_Stream6_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.   USB2UART
  */
void UART4_IRQHandler(void)
{
		u32 RxLen;
	  volatile u32 a = 0;

//  HAL_UART_IRQHandler(&huart4);
		if(__HAL_UART_GET_IT(&huart4,UART_IT_RTO))
		{
				__HAL_UART_CLEAR_IT(&huart4,UART_CLEAR_RTOF);
				
				/* Disable the DMA */
				__HAL_DMA_DISABLE(&hdma_uart4_rx);
			  
			  /* Derive the receiving data length from the NDTR register */
				RxLen = USB2UART_RXBUFFSIZE - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
			
			  /* Clear DMA TCIF2& HTIF2 flag */
			  __HAL_DMA_CLEAR_FLAG(&hdma_uart4_rx,DMA_FLAG_TCIF2_6);
			  
			  /* Reset the DMA NDTR */
			  __HAL_DMA_SET_COUNTER(&hdma_uart4_rx,USB2UART_RXBUFFSIZE);
				
				/* Progress the reveiving data */
			  USB2UART_SendData(USB2UART_aRxBuffer0,RxLen);
				
			  /* Enable the DMA */
				__HAL_DMA_ENABLE(&hdma_uart4_rx);
				
		}
}

/**
  * @brief This function handles UART5 global interrupt.
  */
void UART5_IRQHandler(void)
{
  /* USER CODE BEGIN UART5_IRQn 0 */
	
  /* USER CODE END UART5_IRQn 0 */
  HAL_UART_IRQHandler(&huart5);
  /* USER CODE BEGIN UART5_IRQn 1 */
  /* USER CODE END UART5_IRQn 1 */
}

void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
	
  /* USER CODE END EXTI15_10_IRQn 0 */
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
