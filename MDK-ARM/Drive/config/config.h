#ifndef __CONFIG_H
#define __CONFIG_H


#include "stdint.h"
#include "main.h"
#include "clock_init.h"
#include "debug_uart5.h"
#include "led.h"
#include "key.h"
#include "DynamixelMotor.h"
#include <stdlib.h>
#include <string.h>
#include "usb2uart.h"
#include "MTi_630.h"


extern u8 usb2uart_flag;


void Error_Handler(void);
void assert_failed(uint8_t *file, uint32_t line);
void CacheEnalble(void);
void UART_Transmit(UART_HandleTypeDef huart,u8 *databuf, u8 len);
#endif 
