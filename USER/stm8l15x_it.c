/**
  ******************************************************************************
  * @file    Project/Template/stm8l15x_it.c
  * @author  MCD Tools Team
  * @version V1.2.0
  * @date    09/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l15x_it.h"

#include "main.h"

#include "LightPwmDrv.h"
extern uint8_t USART1_ReceiveDataBuf[20] ;
extern uint8_t Buff_Cnt ;
extern uint8_t USART1_FLAG ;
extern uint8_t RETURN_FLAG ;
uint8_t TIM4COUNT = 0;
uint8_t FLASHFLAG = 0;
uint8_t TouchFlag = 0;
extern uint8_t BLEFLAG;
/** @addtogroup IT_Functions
  * @{
  */
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief FLASH Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief RTC Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

INTERRUPT_HANDLER(RTC_IRQHandler,4)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  //while (1);
  //IRET;
  //AlarmOccurred = TRUE;
  RTC_ClearITPendingBit(RTC_IT_ALRA);
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  if (PWR_PVDGetITStatus() != RESET)
  {
    PWR_PVDClearITPendingBit();
    if (PWR_GetFlagStatus(PWR_FLAG_PVDOF) != RESET)
    {/* Falling Direction Detected*/
      //LCD_SetCursorPos(LCD_LINE1, 0);
      //LCD_Print("MCU : Halt Mode");
      //LCD_SetCursorPos(LCD_LINE2, 0);
      //LCD_Print("VDD < PVD_level");

      //halt();
    }
    else
    {/* Rising Direction Detected*/
      //LCD_SetCursorPos(LCD_LINE1, 0);
      //LCD_Print("MCU : Run  Mode");
      //LCD_SetCursorPos(LCD_LINE2, 0);
      //LCD_Print("VDD > PVD_level");
    }
  }
}

/**
  * @brief External IT PORTB Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTIB_IRQHandler,6)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PORTD Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTID_IRQHandler,7)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
#if GPIO_CFG_SCHEMA_1
  //irdaDecodingFiniteStateMachine ();
#endif
  EXTI_ClearITPendingBit(EXTI_IT_Pin0);
}
/**
  * @brief External IT PIN1 Interrupt routine.
  *   On User button pressed:
  *   Check if button presse a long time (4-5 sec.) if yes --> Set Autotest
  *   Else update status_machine to pass to next measuremetn.
  *   Update the LCD bar graph
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
  //while (1);
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  

  EXTI_ClearITPendingBit(EXTI_IT_Pin2);
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  BLEFLAG =0;
  TouchFlag++;
  if(TouchFlag > 7)
    TouchFlag = 1;
  EXTI_ClearITPendingBit(EXTI_IT_Pin4);
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
#if 0
  disableInterrupts();
  
  /* To clear External IT Pin6 */
  EXTI_ClearITPendingBit(EXTI_IT_Pin6);
  
  enableInterrupts();
#else
  EXTI_ClearITPendingBit(EXTI_IT_Pin6);
  
#endif
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  
  while (1);
}
/**
  * @brief LCD start of new frame Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(LCD_IRQHandler,16)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
//extern uint16_t ADCdata;
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*///
  /* Get ADC convertion value */
  //ADCdata = ADC_GetConversionValue(ADC1);
  //while (1);
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_IRQHandler,19)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
}

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM2_CAP_IRQHandler,20)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_IRQHandler,21)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
}
/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM3_CAP_IRQHandler,22)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  //if(MotorDirection == 1)
    //Motor_Forward_Direction();
  //if(MotorDirection == 2)
   // Motor_Backward_Direction();
  /* Clear Interrupt pending bit */
  TIM1_ClearITPendingBit(TIM1_IT_COM);
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM1_CAP_IRQHandler,24)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
  if(TIM4_GetITStatus(TIM4_IT_Update) == SET)
  {
    TIM4COUNT ++;
    if(TIM4COUNT >= 200)
      FLASHFLAG = 1;

  }
  /* Clear Interrupt Pending bit */
  TIM4_ClearITPendingBit(TIM4_IT_Update);
  

}
/**
  * @brief SPI1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  //while (1);

}

/**
  * @brief USART1 TX Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(USART1_TX_IRQHandler,27)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief USART1 RX Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
uint8_t key = 1;
uint8_t RX_TX_BUFF =0;
INTERRUPT_HANDLER(USART1_RX_IRQHandler,28)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)          //如果串口接收到数据
     {
       RX_TX_BUFF = USART_ReceiveData8(USART1);

    
       USART1_ReceiveDataBuf[Buff_Cnt] = RX_TX_BUFF ;
       if((Buff_Cnt == 7))
        {
          USART1_FLAG = 1;
        }
       if(USART1_ReceiveDataBuf[0] == 0xff)
       {
          RETURN_FLAG = 1;
       }

     }
  Buff_Cnt++;   
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}


/**
  * @brief I2C1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(I2C1_IRQHandler,29)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  
  while(1);
}

/**
  * @}
  */
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

