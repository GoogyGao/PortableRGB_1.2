/*;*******************************************************************************
; FILENAME		: UartDev.h
; AUTHOR		: Feng Liu
; PURPOSE		: Operation for UART of stm8l051
; REVISION		: 1.0
; SYSTEM CLOCK	        : 16MHz
; MCU type              : STM8L051F3            
; Date Time             : 2016-04-18
; Copy right :          : Eastfield Lighting Co., Ltd.
;*******************************************************************************/

#ifndef __UART_DEVICE_
#define __UART_DEVICE_

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Usart1_Config(void);
void Usart1_SendByte(u8 data);
void Usart1_MPUSend(uint16_t data);
unsigned char Usart1_SendDatArray (u8 * pBuf, unsigned char ucLen);
unsigned char Usart1_SendString (u8 * pBuf);
unsigned char Usart1_SendDHT11Data (u8 * pBuf);

#endif
/******************* (C) COPYRIGHT 2016 Eastfield Lighting *****END OF FILE****/