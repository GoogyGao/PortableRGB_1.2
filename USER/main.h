/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date   
  * @brief
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
//#include "stm8_eval.h"
//#include "stm8_eval_lcd.h"
#include "stdio.h"
#include "stm8l15x_conf.h"

#include "NvramOp.h"
#include "UartDev.h"



/* Exported types ------------------------------------------------------------*/
#define GPIO_CFG_SCHEMA_1   0
#define TWOLAMP         0
#define PIC_LIGHT 1

#define StepMotor 1

#if StepMotor
#define ForwardDirection 1
#define BackwardDirection 2


#endif

#if 0
enum _Enm_E2PROM_FUNC {
  enmE2promMark = 0,
  enmPowerOnOffState1,
  enmPowerOnOffState2,
  enmColorTemp,  //Bright? 
  enmColorR1,
  enmColorG1,
  enmColorB1,
  enmColorR2,
  enmColorG2,
  enmColorB2,
  enmBrightness1,
  enmBrightness2,
  enmReserveState,
  enmReserveHour,
  enmReserveMin,
  enmSleepPolicy,
  enmSleepHour,
  enmSleepMin,
  enmTimeSchedState,
  enmPowerOnSchedOnHour,
  enmPowerOnSchedOnMin,
  enmPowerOffSchedOffHour,
  enmPoserOffSchedOffMin,
  enmRgbMode,
  enmRgbState
};

#endif

enum _Enm_E2PROM_FUNC {
  enmE2promMark = 0,
  enmPowerOnOffState1,
  enmPowerOnOffState2,
  enmColorTemp,  //Bright? 
  enmColorR1,
  enmColorG1,
  enmColorB1,
  enmColorR2,
  enmColorG2,
  enmColorB2,
  enmBrightness1,
  enmBrightness2,
  enmScene,
};

/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
    
void Delay(__IO uint32_t nCount);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
