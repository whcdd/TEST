#include "DynamixelMotor.h"
#include <string.h>


const CtrlTableTypeDef CtrlTable[] = {
	  /*  Control Table of EEPROM Area  */
    {0,2,R,1120,0,0},                                     /*!<  Model Number                   */
		{2,4,R,0,0,0},                                        /*!<  Model Information              */
		{6,1,R,0,0,0},                                        /*!<  Firmware Version               */
		{7,1,RW,1,0,253},                                     /*!<  ID                             */
		{8,1,RW,1,0,7},                                       /*!<  Baud Rate                      */
		{9,1,RW,250,0,254},                                   /*!<  Return Delay Time              */
		{10,1,RW,0,0,2},                                      /*!<  Drive Mode                     */
		{11,1,RW,3,0,16},                                     /*!<  Operating Mode                 */
		{12,1,RW,255,0,252},                                  /*!<  Secondary(Shadow) ID           */
		{13,1,RW,2,1,2},                                      /*!<  Protocol Type                  */
		{20,4,RW,0,-1044479,1044479},                         /*!<  Homing Offset                  */
		{24,4,RW,10,0,1023},                                  /*!<  Moving Threshold               */
		{31,1,RW,80,0,100},                                   /*!<  Temperature Limit              */
		{32,2,RW,160,95,160},                                 /*!<  Max Voltage Limit              */
		{34,2,RW,95,95,160},                                  /*!<  Min Voltage Limit              */
		{36,2,RW,885,0,885},                                  /*!<  PWM Limit                      */
		{38,2,RW,2047,0,2047},                                /*!<  Current Limit                  */
		{44,4,RW,128,0,1023},                                 /*!<  Velocity Limit                 */
		{48,4,RW,4095,0,4095},                                /*!<  Max Position Limit             */
		{52,4,RW,0,0,4095},                                   /*!<  Min Position Limit             */
		{56,1,RW,3,0,3},                                      /*!<  External Port Mode 1           */
		{57,1,RW,3,0,3},                                      /*!<  External Port Mode 2           */
		{58,1,RW,3,0,3},                                      /*!<  External Port Mode 3           */
		{63,1,RW,52,0,0},                                     /*!<  Shutdown                       */
		
		/*  Control Table of RAM Area  */
		{64,1,RW,0,0,1},                                      /*!<  Torque Enable                  */
		{65,1,RW,0,0,1},                                      /*!<  LED                            */
		{68,1,RW,2,0,2},                                      /*!<  Status Return Level            */
		{69,1,R,0,0,1},                                       /*!<  Registered Instruction	       */
		{70,1,R,0,0,0},                                       /*!<  Hardware Error Status	         */
		{76,2,RW,1920,0,16383},                               /*!<  Velocity I Gain                */
		{78,2,RW,100,0,16383},                                /*!<  Velocity P Gain                */
		{80,2,RW,0,0,16383},                                  /*!<  Position D Gain                */
		{82,2,RW,0,0,16383},	                                /*!<  Position I Gain                */
		{84,2,RW,800,0,16383},                                /*!<  Position P Gain                */
		{88,2,RW,0,0,16383},                                  /*!<  Feedforward 2nd Gain	         */
		{90,2,RW,0,0,16383},                                  /*!<  Feedforward 1st Gain           */
		{98,1,RW,0,1,127},                                    /*!<  Bus Watchdog                   */
		{100,2,RW,0,-885,885},                                /*!<  Goal PWM                       */
		{102,2,RW,0,-2047,2047},                              /*!<  Goal Current                   */
		{104,4,RW,0,-1023,1023},                              /*!<  Goal Velocity                  */
		{108,4,RW,0,0,32767},                                 /*!<  Profile Acceleration	         */
		{112,4,RW,0,0,32767},                                 /*!<  Profile Velocity               */
		{116,4,RW,0,0,4095},                                  /*!<  Goal Position                  */
		{120,2,R,0,0,32767},                                  /*!<  Realtime Tick                  */
		{122,1,R,0,0,1},                                      /*!<  Moving                         */
		{123,1,R,0,0,0},                                      /*!<  Moving Status	                 */
		{124,2,R,0,0,0},                                      /*!<  Present PWM                    */
		{126,2,R,0,0,0},                                      /*!<  Present Current                */
		{128,4,R,0,0,0},                                      /*!<  Present Velocity               */
		{132,4,R,0,0,0},                                      /*!<  Present Position               */		
		{136,4,R,0,0,0},                                      /*!<  Velocity Trajectory            */
		{140,4,R,0,0,0},                                      /*!<  Position Trajectory            */
		{144,2,R,0,0,0},                                      /*!<  Present Input Voltage          */
		{146,1,R,0,0,0},                                      /*!<  Present Temperature            */
		{152,2,RW,0,0,0},                                     /*!<  External Port Mode 1           */
		{154,2,RW,0,0,0},                                     /*!<  External Port Mode 2           */
		{156,2,RW,0,0,0}                                      /*!<  External Port Mode 3           */		
};






/* Private function prototypes -----------------------------------------------*/
/** @addtogroup DXL_Private_Functions
  * @{
  */
static unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);
/**
  * @}
  */



/* External variables --------------------------------------------------------*/
UART_HandleTypeDef huart1;
u8 DXL_aRxBuffer[DXL_RXBUFFSIZE] = {0};

/** @defgroup DXL direction define
  * @{ DE: Sending data enable   RE: Receiving data enable
  */
void DXL_DIR(DXL_DIRTypeDef dir)  
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,(GPIO_PinState) dir);
} 


/**
  * @brief USART1 RS485 Initialization Function
	* PB5-->DXL_DIR PB6-->DXL_TX PB7-->DXL_RX
  * @param baud
  * @retval None
  */
DXL_StatusTypeDef DXLMotor_Init(u32 baud)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		/* DXL_DIR Initialization*/
		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOB_CLK_ENABLE();

		/*Configure GPIO pin Output Level */
		/* By default data receiving mode */
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

		/*Configure GPIO pin : PB5 */
		GPIO_InitStruct.Pin = GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		/* USART1 Initialization */
		huart1.Instance = USART1;
		huart1.Init.BaudRate = baud;
		huart1.Init.WordLength = UART_WORDLENGTH_8B;
		huart1.Init.StopBits = UART_STOPBITS_1;
		huart1.Init.Parity = UART_PARITY_NONE;
		huart1.Init.Mode = UART_MODE_TX_RX;
		huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart1.Init.OverSampling = UART_OVERSAMPLING_16;
		huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		if (HAL_UART_Init(&huart1) != HAL_OK)
		{
			Error_Handler();
			return (DXL_StatusTypeDef)DXL_ERROR;
		}
		/* USER CODE BEGIN USART1_Init 2 */
		HAL_UART_Receive_IT(&huart1,DXL_aRxBuffer,DXL_RXBUFFSIZE);
		/* USER CODE END USART1_Init 2 */
		return (DXL_StatusTypeDef)DXL_OK;
}

/**
  * @brief USART1 RS485  Send Motor Data 
  * @param databuf: the starting address of the buffer memory    
  * @param len: the size of the buffer
  * @retval None
  */
DXL_StatusTypeDef DXL_SendData(u8 *databuf, u8 len)
{
    DXL_DIR(DE);
	  /* Transmit the data */
	  HAL_UART_Transmit(&huart1,databuf,len,1000);
	  while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC) != SET);

	  DXL_DIR(RE);
	  return (DXL_StatusTypeDef)DXL_OK;
}


/**
  * @brief  Pull the data out of the buffe and put it into the memory user creates 
  * @param databuf: the starting address of the buffer memory    
  * @param len: the size of the buffer
  * @retval None
  */
DXL_StatusTypeDef DXL_ReceiveData(u8 *datades,u8 len)
{
	  
	  return (DXL_StatusTypeDef)DXL_OK;
}

/**
  * @brief Send complete instruction to motor according Protocol
  * @param DXL_Handler: DXL handler 
  * @retval DXL status
  */
DXL_StatusTypeDef DXL_ProtocolSendData(DXL_HandlerTypeDef *DXL_Handler)
{
	  u16 i=0;
	  u8 CRC_L=0,CRC_H=0;
	  u16 CRC16=0;
	  /*  Define A DataPacket used for sending a complete data to cal  */
	  u8 *DataPacket;
	  u8 PacketLen = 7 + DXL_Handler->len;
	  
		u8 id = DXL_Handler->id;
		u16 len = DXL_Handler->len;
	  /*  distribute the memory to DataPacet */
	  DataPacket = (u8 *)malloc(PacketLen*sizeof(u8));
	  memset(DataPacket,0,PacketLen);
	  
	  /* Generate a complete data packet */
	  DataPacket[0] = 0xFF;
	  DataPacket[1] = 0xFF;
	  DataPacket[2] = 0xFD;
	  DataPacket[3] = 0x00;
		DataPacket[4] = id;
		DataPacket[5] = (DXL_Handler->len)&0xff;
		DataPacket[6] = (DXL_Handler->len)>>8;
		DataPacket[7] = DXL_Handler->Ins;
	  
		if(len > 3)
		{
				for(i=8;i<len+5;i++)
				{
						DataPacket[i] = DXL_Handler->Param[i-8];
				}
		}	  
	  
	/*  Calculate the CRC  */
	  CRC16 = update_crc(0, DataPacket, 5+DXL_Handler->len);
		CRC_L = CRC16 & 0xff;
    CRC_H = CRC16 >> 8;
		
		DataPacket[len+5] =  CRC_L;
		DataPacket[len+6] =  CRC_H;
		
		/*  Send the data  */
		DXL_SendData(DataPacket,7+len);
		
		/*  Free the  memory */
		free(DataPacket);
    return DXL_OK;
}



DXL_StatusTypeDef DXL_ProtocolReceiveData(DXL_HandlerTypeDef *DXL_Handler)
{
    
    
    return DXL_OK;
}

/**
  * @brief Generate CRC-16
  * @param 
					crc_accum : set as 0
					data_blk_ptr : Packet array pointer
					data_blk_size : number of bytes in the Packet excluding the CRC
					data_blk_size = Header(3) + Reserved(1) + Packet ID(1) + Packet Length(2) + Packet Length - CRC(2) = 3 + 1 + 1 + 2 + Pakcet Length - 2 = 5 + Packet Length;
					Packet Length = (LEN_H « 8 ) + LEN_L; //Little-endian
  * @retval crc-16 CRC_L = (CRC & 0x00FF); CRC_H = (CRC>>8) & 0x00FF;//Little-endian 
  */
static unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size)
{
    unsigned short i, j;
    unsigned short crc_table[256] = {
        0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
        0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
        0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
        0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
        0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
        0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
        0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
        0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
        0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
        0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
        0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
        0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
        0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
        0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
        0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
        0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
        0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
        0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
        0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
        0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
        0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
        0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
        0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
        0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
        0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
        0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
        0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
        0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
        0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
        0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
        0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
        0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
    };

    for(j = 0; j < data_blk_size; j++)
    {
        i = ((unsigned short)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
        crc_accum = (crc_accum << 8) ^ crc_table[i];
    }

    return crc_accum;
}




