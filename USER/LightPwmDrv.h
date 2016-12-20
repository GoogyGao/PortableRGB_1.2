/*;*******************************************************************************
; FILENAME		: LightPwmDrv.h
; AUTHOR		: Feng Liu
; PURPOSE		: PWM-based Lighting Driver
; REVISION		: 1.0
; SYSTEM CLOCK	        : 16MHz
; MCU type              : STM8L051F3            
; Date Time             : 2016-04-18
; Copy right :          : Eastfield Lighting Co., Ltd.
;*******************************************************************************/

#ifndef __LIGHT_PWM_DRIVER_
#define __LIGHT_PWM_DRIVER_
#include "main.h"

/* Includes ------------------------------------------------------------------*/

/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define BRIGHT_MAX		10
#define BRIGHT_MIN		1
#define WARM_MAX		10
#define	WARM_MIN		1
#define DEFAULTBRIGHTNESS       80
#define BRIGHT_COUNT BRIGHT_MAX-BRIGHT_MIN+1

/* Private macros ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#define MODE_CC1_EXTERNAL_PWM_DUTY      
void callbackTim1EventCC (void);
void callbackTim3EventCC (void);

void RGBBrightnessCtrLight(unsigned char WValue);
void RGBWBrightnessCtrLight(unsigned char WValue);

void initExternalPwmCaptureFunction (void);
void initTimPWMFunction (void);
void Tim4CounterInit();
void driveRGBLightPwm1 (unsigned char RValue, unsigned char GValue,unsigned char BValue);
void driveRGBLightPwm2 (unsigned char RValue, unsigned char GValue,unsigned char BValue);
void RGBChangeBrighness1(unsigned char RValue, unsigned char GValue,unsigned char BValue,unsigned char WValue);
void RGBChangeBrighness2(unsigned char RValue, unsigned char GValue,unsigned char BValue,unsigned char WValue);
void configureTIM1PwmCapture (TIM1_Channel_TypeDef TIM1_Channel);
void driveColdWarmLightPwm (unsigned char ucCold, unsigned char ucWarm);

void captureExtPwmParamRecognition (void);

#endif
/******************* (C) COPYRIGHT 2016 Eastfield Lighting *****END OF FILE****/