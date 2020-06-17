/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "config.h"
#include "main.h"
#include "clock_init.h"
#include "led.h"
#include "debug_uart5.h"
#include "key.h"
#include "usb2uart.h"

/**
  * @brief  The application entry point.
  * @retval int
  */


int main(void)
{
	u8 test[] = "hello";
	u8 len;
  /* MCU Configuration--------------------------------------------------------*/
  CacheEnalble();
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  SystemClock_Config();
  /* Initialize all configured peripherals */
  LED_Init();
	USB2UART_Init(2000000);
	
	/* Note: MTi630 must initialize at last!!! */
	MTi_630_Init(921600);

  while (1)
  {
		
  }
}



