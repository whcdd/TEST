/*********************************************************************************************************************************/
/**********************************zhejianglab************************************************************************************/
/***FileName:      dataFrame.c                                                                                                 ***/
/***Author:                                                                                                                    ***/
/***Version:       1.0                                                                                                         ***/
/***Date:                                                                                                                      ***/ 
/***Description:   数据组帧                                                                                                    ***/
/**                                                                                                                            ***/
/**                                                                                                                            ***/
/***Others:                                                                                                                    ***/
/***Function List:                                                                                                             ***/
/**    1.                                                                                                                      ***/
/**    2.                                                                                                                      ***/
/***History:                                                                                                                   ***/
/**    1.Date:   2020/06/24                                                                                                    ***/
/**      Author: Caodandan                                                                                                     ***/
/**      Modification:                                                                                                         ***/
/**    2.                                                                                                                      ***/
/*********************************************************************************************************************************/
#include "dataFrame.h"


/* External variables -----------------------------------------------------------------------------------------------------------*/
volatile u8 *Motor_Buffer;
volatile u8 *IMU_Buffer;
volatile u8 *MCU_Buffer;
volatile u8 *Left_Buffer;
volatile u8 *Right_Buffer;
//volatile u8 *sendBuffer;


/* Private function prototypes --------------------------------------------------------------------------------------------------*/
/** @addtogroup dataFrame_Private_Functions
  * @{
  */
void Data_Send(void);
void Packet_Frame(void);
void MCU_Frame(void);
void Left_Frame(void);
void Right_Frame(void);
void IMU_Frame(void);
void Motor_Frame(void);



/*********************************************************************************************************************************/
/**Function Name:         Data_Send                                                                                            **/
/**Function Description:  返回数据                                                                                               **/
/**Inputs:                void                                                                                                  **/
/**                                                                                                                             **/
/**Outputs:               void                                                                                                  **/
/**                                                                                                                             **/
/**Notes:                                                                                                                       **/
/*********************************************************************************************************************************/
void Data_Send(void)
{
	
	Motor_Frame();
	IMU_Frame();
	MCU_Frame();
	Left_Frame();
	Right_Frame();
	Packet_Frame();

}
/*********************************************************************************************************************************/
/**Function Name:         MCU_Frame                                                                                             **/
/**Function Description:  MCU返回数据组帧函数                                                                                    **/
/**Inputs:                void                                                                                                  **/
/**                                                                                                                             **/
/**Outputs:               void                                                                                                  **/
/**                                                                                                                             **/
/**Notes:                                                                                                                       **/
/*********************************************************************************************************************************/
void MCU_Frame(void)
{

}
/*********************************************************************************************************************************/
/**Function Name:         Left_Frame                                                                                            **/
/**Function Description:  压力传感器左脚返回数据组帧函数                                                                          **/
/**Inputs:                void                                                                                                  **/
/**                                                                                                                             **/
/**Outputs:               void                                                                                                  **/
/**                                                                                                                             **/
/**Notes:                                                                                                                       **/
/*********************************************************************************************************************************/
void Left_Frame(void)
{

}
/*********************************************************************************************************************************/
/**Function Name:         Motor_Frame                                                                                           **/
/**Function Description:  压力传感器右脚返回数据组帧函数                                                                          **/
/**Inputs:                void                                                                                                  **/
/**                                                                                                                             **/
/**Outputs:               void                                                                                                  **/
/**                                                                                                                             **/
/**Notes:                                                                                                                       **/
/*********************************************************************************************************************************/
void Right_Frame(void)
{

}
/*********************************************************************************************************************************/
/**Function Name:         Motor_Frame                                                                                           **/
/**Function Description:  舵机返回数据组帧函数                                                                                   **/
/**Inputs:                void                                                                                                  **/
/**                                                                                                                             **/
/**Outputs:               void                                                                                                  **/
/**                                                                                                                             **/
/**Notes:                                                                                                                       **/
/*********************************************************************************************************************************/
void Motor_Frame(void)
{
	int i = 0;
	Motor_Buffer[0] = 0x05;
	Motor_Buffer[1] = 0x00;
	Motor_Buffer[2] = 0x00;
	
	if(UART1RxEndFlag && Motor_Read_Flag)//接收到舵机传来数据且在传数据前上位机已经发送读舵机指令
//	if(UART1RxEndFlag)//测试用
	{
		switch(Motor_Read_Cmd)
		{
			case 0x02://read
//				Motor_Buffer = (u8 *)malloc(UART1RxLen*sizeof(u8));
//				memset(Motor_Buffer,0,UART1RxLen);
				Motor_Length_Sign += UART1RxLen;
				for(i = 3;i<Motor_Length_Sign;i++)
				{
					Motor_Buffer[i] = DXL_aRxBuffer[i]; //测试volatile
				}
					/* Progress the reveiving data */
	//			DXL_SendData(DXL_aTxBuffer,UART1RxLen);
//				USB2UART_SendData(DXL_aTxBuffer,UART1RxLen);
				return;
				break;
			case 0x82://Sync Read
				Sync_Read_Len--;
			//此处试着malloc不定长的数组
				for(i = 0;i<UART1RxLen;i++)
				{
					Motor_Buffer[i+Motor_Length_Sign] = DXL_aRxBuffer[i];
				}
				Motor_Length_Sign += UART1RxLen;
			//拼接数据
				if(Sync_Read_Len == 0)
				{
					//此处为拼帧处理；
//					USB2UART_SendData(Motor_Buffer,Motor_Length_Sign);//此处待继续优化，和IMU拼接组帧
					return;
				}
				break;
			case 0x92://Bulk Read
				Bulk_Read_Len--;
				for(i = 0;i<UART1RxLen;i++)
				{
					Motor_Buffer[i+Motor_Length_Sign] = DXL_aRxBuffer[i];
				}
				Motor_Length_Sign += UART1RxLen;
			//拼接数据,将数据传到一个大包中，待后续拼接完成传送
			if(Bulk_Read_Len == 0)
			{
//				USB2UART_SendData(Motor_Buffer,Motor_Length_Sign);//此处待继续优化，和IMU拼接组帧
				return;
			}
				break;
			default:
				//向上位机返回设备命令错误字
				return;
		}

	
		UART1RxEndFlag = 0;
		UART1RxLen = 0;

	}
}

/*********************************************************************************************************************************/
/**Function Name:         IMU_Frame                                                                                             **/
/**Function Description:  舵机返回数据组帧函数                                                                                   **/
/**Inputs:                UINT8 blockNum                                                                                        **/
/**                                                                                                                             **/
/**Outputs:               void                                                                                                  **/
/**                                                                                                                             **/
/**Notes:                                                                                                                       **/
/*********************************************************************************************************************************/
void IMU_Frame(void)
{
	if(IMU_Read_Flag)
	{
		
		//接口函数取IMU返回值
		IMU_Read_Flag = 0;
	}
}


/*********************************************************************************************************************************/
/**Function Name:         Packet_Frame                                                                                           **/
/**Function Description:  返回数据整体组帧函数                                                                                    **/
/**Inputs:                UINT8 blockNum                                                                                        **/
/**                                                                                                                             **/
/**Outputs:               void                                                                                                  **/
/**                                                                                                                             **/
/**Notes:                                                                                                                       **/
/*********************************************************************************************************************************/
void Packet_Frame(void)
{
	volatile u8 *sendBuffer;
	u8 sendBufferLen;
	u8 i = 5,j = 0;
	u16 CRC16=0;
	u8 CRC_L=0,CRC_H=0;
	sendBuffer[0] = 0xFF;
	sendBuffer[1] = 0xFD;
	sendBuffer[2] = 0x00;
	sendBuffer[3] = 0x00;
	sendBuffer[4] = 0x55;

	if(MCU_Read_Flag)
	{
		for(j = 0;j < 4;j++)
		{
			sendBuffer[i] = MCU_Buffer[j];
			i++;
		}
		memset(MCU_Buffer,0,4);
		MCU_Read_Flag = 0;
	
	}
	if(Left_Read_Flag)
	{
		for(j = 0;j < 5;j++)
		{
			sendBuffer[i] = Left_Buffer[j];
			i++;
		}
		memset(Left_Buffer,0,4);
		Left_Read_Flag = 0;
	}
	if(Right_Read_Flag)
	{
		for(j = 0;j < 5;j++)
		{
			sendBuffer[i] = Right_Buffer[j];
			i++;
		}
		memset(Right_Buffer,0,4);
		Right_Read_Flag = 0;
	}
	if(IMU_Read_Flag)
	{
		for(j = 0;j < 5;j++)//此处长度待更改为IMU包长
		{
			sendBuffer[i] = IMU_Buffer[j];
			i++;
		}
		
		memset(IMU_Buffer,0,5);//长度待更改
		IMU_Read_Flag = 0;
	
	}
	if(Motor_Read_Flag)
	{
		for(j = 0;j < Motor_Length_Sign;j++)
		{
			sendBuffer[i] = Motor_Buffer[j];
			i++;
		}
		memset(Motor_Buffer,0,Motor_Length_Sign);
		Motor_Length_Sign = 3;
		Motor_Read_Cmd = 0;
		Motor_Read_Flag = 0;
		UART1RxEndFlag = 0;
		UART1RxLen = 0;
	
	}
	//取数组长度做CRC校验
	CRC16 = update_crc(0, sendBuffer, i-1);
	CRC_L = CRC16 & 0xff;
    CRC_H = CRC16 >> 8;
		
	sendBuffer[i] =  CRC_L;
	i++;
	sendBuffer[i] =  CRC_H;
	sendBufferLen = i-1;
	USB2UART_SendData(sendBuffer,sendBufferLen);
	memset(sendBuffer,0,UART1RxLen);
	
}
