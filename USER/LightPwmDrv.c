/*;*******************************************************************************
; FILENAME		: LightPwmDrv.c
; AUTHOR		: Feng Liu
; PURPOSE		: PWM-based Lighting Driver
; REVISION		: 1.0
; SYSTEM CLOCK	        : 16MHz
; MCU type              : STM8L051F3            
; Date Time             : 2016-04-18
; Copy right :          : Eastfield Lighting Co., Ltd.
;*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "LightPwmDrv.h"
#include "typedef.h"
#include "global.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define COLD_LIGHT_PWM_PIN_PORT         GPIOB
#define COLD_LIGHT_PWM_PIN_ID           GPIO_Pin_3

#define WARM_LIGHT_PWM_PIN_PORT         GPIOB
#define WARM_LIGHT_PWM_PIN_ID           GPIO_Pin_2

#define PIC_LIGHT_PWM_PIN_PORT         GPIOB
#define PIC_LIGHT_PWM_PIN_ID           GPIO_Pin_0

// base frequency = 1M Hz
// PWM frequency = (base Freq) / (TIM2_PWM_PERIOD + 1)
// TIM2_PWM_PERIOD    199      
//                    5K
#define TIMx_PWM_PERIOD         254
#define TIMx_PWM_PULSE          255

#define TIM1_PRESCALER              64
#define TIM1_REPTETION_COUNTER      0

#define MAXLIGHTBRIGHTNESS            100
#define TIM4_PERIOD                   254
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
// 


//pwm1:R:PD2 G:PB0 B:PB1
//pwm2:R:PD4 G:PB2 B:PD0 W:PD5
/**
  * @brief  Configure peripherals GPIO for WARM/COLD PWM   
  * @param  None
  * @retval None
  */
static void TIMxRGBWLightPwm_ClkGpioConfig(void)
{
  /* Enable TIM2 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  //CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  /* TIM2 Channel1 configuration: PB2 */
  /* TIM1 Channels 3N, 1  ,3 and 1N configuration: PD2, PD4 and PD7 */
  GPIO_Init(GPIOD, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 , GPIO_Mode_Out_PP_Low_Fast);

  /* TIM1 Break input pin configuration */
  GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  , GPIO_Mode_Out_PP_Low_Fast);

}
              

/**
  * @brief  Configure TIM2 peripheral   
  * @param  None
  * @retval None
  */
static void TIMxPWMFunction_Config(void)
{
  //////////////////tim1///////////////////////
  TIM1_TimeBaseInit(TIM1_PRESCALER, TIM1_CounterMode_Up, TIMx_PWM_PERIOD,TIM1_REPTETION_COUNTER);
  
  TIM1_OC1Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               TIMx_PWM_PULSE, TIM1_OCPolarity_High, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_OC1PreloadConfig(DISABLE);
  
  TIM1_OC2Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               TIMx_PWM_PULSE, TIM1_OCPolarity_High, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_OC2PreloadConfig(DISABLE);
  
  TIM1_OC3Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               TIMx_PWM_PULSE, TIM1_OCPolarity_High, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_OC3PreloadConfig(DISABLE);

  TIM1_CtrlPWMOutputs(ENABLE);
  TIM1_Cmd(ENABLE);
  //////////////////tim2/////////////////////
  
  /* TIM2 configuration:
     - TIM2 counter is clocked by HSI div 2 8M
      so the TIM2 counter clock used is HSI / 1 = 16M / 1 = 16 MHz
    TIM2 Channel1 output frequency = TIM2CLK / (TIM2 Prescaler * (TIM2_PERIOD + 1))
                                   = 16M / (1 * 2) = 8M Hz 
  = 16M / (8 * 200) = 10K Hz //TIM2_Prescaler_8; TIM2_PWM_PERIOD = 199
  */
  /* Time Base configuration */
  TIM2_TimeBaseInit(TIM2_Prescaler_64, TIM2_CounterMode_Up, TIMx_PWM_PERIOD);
  //TIM2_ETRClockMode2Config(TIM2_ExtTRGPSC_DIV4, TIM2_ExtTRGPolarity_NonInverted, 0);

  /* Channel 1 configuration in PWM1 mode */
  /* TIM2 channel Duty cycle is 100 * (TIM2_PERIOD + 1 - TIM2_PULSE) / (TIM2_PERIOD + 1) = 100 * 4/8 = 50 % */
  TIM2_OC1Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable, TIMx_PWM_PULSE, TIM2_OCPolarity_High, TIM2_OCIdleState_Set);

  /* Channel 2 configuration in PWM1 mode */
  /* TIM2 channel Duty cycle is 100 * (TIM2_PERIOD + 1 - TIM2_PULSE) / (TIM2_PWM_PERIOD + 1) = 100 * 4/8 = 50 % */
  TIM2_OC2Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable, TIMx_PWM_PULSE, TIM2_OCPolarity_High, TIM2_OCIdleState_Set);

  /* TIM2 Main Output Enable */
  TIM2_CtrlPWMOutputs(ENABLE);

  /* TIM2 counter enable */
  TIM2_Cmd(ENABLE);
  
  //////////////////////////tim3/////////////////
  
  TIM3_TimeBaseInit(TIM3_Prescaler_64, TIM3_CounterMode_Up, TIMx_PWM_PERIOD);
  //TIM2_ETRClockMode2Config(TIM2_ExtTRGPSC_DIV4, TIM2_ExtTRGPolarity_NonInverted, 0);

  /* Channel 1 configuration in PWM1 mode */
  /* TIM2 channel Duty cycle is 100 * (TIM2_PERIOD + 1 - TIM2_PULSE) / (TIM2_PERIOD + 1) = 100 * 4/8 = 50 % */
  TIM3_OC1Init(TIM3_OCMode_PWM1, TIM3_OutputState_Enable, TIMx_PWM_PULSE, TIM3_OCPolarity_High, TIM3_OCIdleState_Set);

  /* Channel 2 configuration in PWM1 mode */
  /* TIM2 channel Duty cycle is 100 * (TIM2_PERIOD + 1 - TIM2_PULSE) / (TIM2_PWM_PERIOD + 1) = 100 * 4/8 = 50 % */
  TIM3_OC2Init(TIM3_OCMode_PWM1, TIM3_OutputState_Enable, TIMx_PWM_PULSE, TIM3_OCPolarity_High, TIM3_OCIdleState_Set);

  
  TIM3_CtrlPWMOutputs(ENABLE);

  
  TIM3_Cmd(ENABLE);
  
///////////////tim4//////////////////
  //TIM4_TimeBaseInit(TIM4_Prescaler_16, TIM4_PERIOD);
  /* TIM4 update interrupt enable */
  //TIM4_ITConfig(TIM4_IT_Update, ENABLE);
}

void initTimPWMFunction (void)
{
  TIMxRGBWLightPwm_ClkGpioConfig ();//gpio³õÊ¼»¯
  TIMxPWMFunction_Config ();
  //BrightnessCtrLight
  //driveRGBLightPwm1(255,0,0);
  //Delay(100000);
}

void Tim4CounterInit()
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_Prescaler_256, TIM4_PERIOD);
  /* TIM4 update interrupt enable */
  TIM4_ITConfig(TIM4_IT_Update, ENABLE);
  TIM4_Cmd(ENABLE);
}




uint8_t tmpvalue = 0;


void RGBBrightnessCtrLight(unsigned char WValue)
{
  if (WValue >= 100)
    WValue = 100;
  WValue = WValue *255 / 100;
    
  TIM1_OC3Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               WValue, TIM1_OCPolarity_High, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  
}

void RGBWBrightnessCtrLight(unsigned char WValue)
{
  if (WValue >= 255)
    WValue = 255;

  TIM1_OC3Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               WValue, TIM1_OCPolarity_High, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  
}


#if 0
void BrightnessCtrLight2(void)
{
  if(LIGHT2_BRIGHTNESS > MAXLIGHTBRIGHTNESS)
    LIGHT2_BRIGHTNESS = 100;

    tmpvalue2++;

    if(tmpvalue2==100)
      tmpvalue2 = 0;

  if(tmpvalue2 < LIGHT2_BRIGHTNESS)
    GPIO_SetBits(GPIOB,GPIO_Pin_3);
  else
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);

}
#endif
void regulateLightPulseWidthNO1 (unsigned char RValue, unsigned char GValue,unsigned char BValue )
{
  //uint16_t pulseWidth;
  
  
  
  TIM1_OC1Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               RValue , TIM1_OCPolarity_High, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  
  TIM2_OC1Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable, GValue , TIM2_OCPolarity_High, TIM2_OCIdleState_Set);  
  
  TIM3_OC1Init(TIM3_OCMode_PWM1, TIM3_OutputState_Enable, BValue , TIM3_OCPolarity_High, TIM3_OCIdleState_Set);
}

void regulateLightPulseWidthNO2 (unsigned char RValue, unsigned char GValue,unsigned char BValue)
{
  //uint16_t pulseWidth;
  
  

  TIM1_OC2Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               RValue , TIM1_OCPolarity_High, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  
  TIM2_OC2Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable, GValue , TIM2_OCPolarity_High, TIM2_OCIdleState_Set);  
  
  TIM3_OC2Init(TIM3_OCMode_PWM1, TIM3_OutputState_Enable, BValue , TIM3_OCPolarity_High, TIM3_OCIdleState_Set);
}



void driveRGBLightPwm1 (unsigned char RValue, unsigned char GValue,unsigned char BValue)
{
  regulateLightPulseWidthNO1(RValue ,GValue ,BValue );

}
void driveRGBLightPwm2 (unsigned char RValue, unsigned char GValue,unsigned char BValue)
{
  regulateLightPulseWidthNO1(RValue ,GValue ,BValue );
  
}
void RGBChangeBrighness1(unsigned char RValue, unsigned char GValue,unsigned char BValue,unsigned char WValue)
{
  if(WValue>100)
    WValue = 100;
  RValue = RValue * WValue /100 ;
  GValue = GValue * WValue /100 ;
  BValue = BValue * WValue /100 ;
  regulateLightPulseWidthNO1(RValue ,GValue ,BValue );
}

void RGBChangeBrighness2(unsigned char RValue, unsigned char GValue,unsigned char BValue,unsigned char WValue)
{
  if(WValue>100)
    WValue = 100;
  RValue = RValue * WValue /100 ;
  GValue = GValue * WValue /100 ;
  BValue = BValue * WValue /100 ;
  regulateLightPulseWidthNO2(RValue ,GValue ,BValue );
}

/******************* (C) COPYRIGHT 2016 Eastfield Lighting *****END OF FILE****/