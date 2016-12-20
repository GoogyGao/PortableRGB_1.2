/*;*****************************************************************************
; FILENAME		: NvramOp.c
; AUTHOR		: Feng Liu
; PURPOSE		: Operation for internal E2PROM (NVRAM) controller of stm8l051
;                       : 256 bytes internal E2PROM 
; REVISION		: 1.0
; SYSTEM CLOCK	        : 16MHz
; MCU type              : STM8L051F3            
; Date Time             : 2016-04-18
; Copy right :          : Eastfield Lighting Co., Ltd.
;******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "NvramOp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef _IAR_
__no_init __eeprom unsigned char NVRAM[0x100];
#endif

/* Public functions Declaration ----------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/* Public functions ----------------------------------------------------------*/
uint8_t Nvram_Read (uint8_t u8Offset) 
{
  return NVRAM[u8Offset];
}

uint8_t Nvram_Write (uint8_t u8Offset, uint8_t u8Value)
{
  FLASH_Unlock(FLASH_MemType_Data);
  NVRAM[u8Offset] = u8Value;
  FLASH_WaitForLastOperation(FLASH_MemType_Data);
  FLASH_Lock(FLASH_MemType_Data);
  
  return 0;
}

void NvramWrite_TrancBegin ()
{
  FLASH_Unlock(FLASH_MemType_Data);
}

void NvramWrite_TrancEnd ()
{
  FLASH_WaitForLastOperation(FLASH_MemType_Data);
  FLASH_Lock(FLASH_MemType_Data);
}
  
void NvramWrite_Tranction (uint8_t u8Offset, uint8_t u8Value)
{
  NVRAM[u8Offset] = u8Value;
}



/******************* (C) COPYRIGHT 2016 Eastfield Lighting *****END OF FILE****/