/*;*******************************************************************************
; FILENAME		: UartDev.c
; AUTHOR		: Feng Liu
; PURPOSE		: Operation for UART Device of stm8l051
; REVISION		: 1.0
; SYSTEM CLOCK	        : 16MHz
; MCU type              : STM8L051F3            
; Date Time             : 2016-04-18
; Copy right :          : Eastfield Lighting Co., Ltd.
;******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "UartDev.h"
#include "typedef.h"
#include "global.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Public functions Declaration ----------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#define uchar unsigned char
/* Public functions ----------------------------------------------------------*/
void Usart1_Config(void)
{
  // init as 115200, 1 stop, no parity, rx int, tx query;
  USART_DeInit (USART1);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,ENABLE);
  SYSCFG->RMPCR1 &= 0xcf;
  SYSCFG->RMPCR1 |= 0x10;
  //GPIO_Init(GPIOC,GPIO_Pin_4,GPIO_Mode_In_PU_No_IT);
  //GPIO_Init(GPIOC,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Fast);
  //GPIO_Init(GPIOA,GPIO_Pin_3,GPIO_Mode_In_PU_No_IT);            //PA3 UART RX
  //GPIO_Init(GPIOA,GPIO_Pin_2,GPIO_Mode_Out_PP_Low_Fast);        //PA2 UART TX
  
  GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_2, ENABLE);         // TX
  GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_3, ENABLE);         // RX
  
  USART_Init( USART1,115200,USART_WordLength_8b,
              USART_StopBits_1,USART_Parity_No,
              (USART_Mode_TypeDef)(USART_Mode_Rx|USART_Mode_Tx));
  
  USART1->CR2 |= 0x0c;
  
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  
  USART_Cmd(USART1,ENABLE);
}

void Usart1_MPUSend (uint16_t data)
{
  uint8_t highdata , lowdata;
  highdata = (uint8_t)(data >> 8);
  lowdata  = (uint8_t) data ;
  
  USART_SendData8(USART1, (uint8_t) highdata);

  // Loop until the end of tranmission
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  
  USART_SendData8(USART1, (uint8_t) lowdata);

  // Loop until the end of tranmission
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  
}


void Usart1_SendByte(u8 data)
{
  // Usart1_SendData8 ((unsigned char) data);
  USART_SendData8(USART1, (uint8_t) data);
  
  // Loop until the end of tranmission
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

unsigned char Usart1_SendDatArray (u8 * pBuf, unsigned char ucLen)
{
  if ((!pBuf) || (!ucLen))
    return 2;
  
  for (unsigned char uc = 0; uc < (ucLen); uc ++) {
    Usart1_SendByte(pBuf[uc]);
  }
  
  return 1;
}

unsigned char Usart1_SendString (u8 * pBuf)
{
  unsigned char ucPos;
  if (!pBuf)
    return 0;
  
  ucPos = 0;
  do {
    if (pBuf[ucPos] != '\0') {
      Usart1_SendByte(pBuf[ucPos ++]);
    } else 
      break;
  } while (1);
  
  return 1;
}

unsigned char Usart1_SendDHT11Data (u8 * pBuf)
{
  unsigned char ucPos;
  if (!pBuf)
    return 0;
  
  ucPos = 2;
  do {
    if (pBuf[ucPos] != '\0') {
      Usart1_SendByte(pBuf[ucPos ++]);
    } else 
      break;
  } while (1);
  
  return 1;
}

/******************* (C) COPYRIGHT 2016 Eastfield Lighting *****END OF FILE****/