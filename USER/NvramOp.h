/*;*****************************************************************************
; FILENAME		: NvramOp.h
; AUTHOR		: Feng Liu
; PURPOSE		: Operation for internal E2PROM (NVRAM) controller of stm8l051
;                       : 256 bytes internal E2PROM 
; REVISION		: 1.0
; SYSTEM CLOCK	        : 16MHz
; MCU type              : STM8L051F3            
; Date Time             : 2016-04-18
; Copy right :          : Eastfield Lighting Co., Ltd.
;******************************************************************************/

#ifndef __NVRAM_OP_H_
#define __NVRAM_OP_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t Nvram_Read (uint8_t u8Offset);
uint8_t Nvram_Write (uint8_t u8Offset, uint8_t u8Value);

void NvramWrite_TrancBegin ();
void NvramWrite_TrancEnd ();
void NvramWrite_Tranction (uint8_t u8Offset, uint8_t u8Value);


#endif
/******************* (C) COPYRIGHT 2016 Eastfield Lighting *****END OF FILE****/