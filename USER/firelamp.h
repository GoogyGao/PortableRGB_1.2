#ifndef _FIRELAMP_H_
#define _FIRELAMP_H_

#include "stm8l15x.h"

#define FIRE_LAMP_PORT                  GPIOB
#define FIRE_LAMP_PIN                   GPIO_Pin_3

#define TOUCH_PORT                      GPIOB
#define TOUCH_PIN                       GPIO_Pin_4


#define CODE_ZERN(GPIO_PINx)   {FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)0;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)0;}  

#define CODE_ZERN1(GPIO_PINx)   {FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)0;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)0;}  

#define CODE_ONE(GPIO_PINx)    {FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)0;}

#define CODE_ONE1(GPIO_PINx)    {FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)GPIO_PINx;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)0;\
                                FIRE_LAMP_PORT->ODR = (uint8_t)0;\
                                }

void LampGpioInit(void);
void SEND_CODE_DATA(uint8_t ch[],int dataLen,int gpio_pinx ,uint32_t time);
void RGBLightCtr(uint8_t RValue, uint8_t GValue, uint8_t BValue , uint8_t Brightness ,uint16_t Time ,uint8_t count);
void APP_BigFIRE(void);
void APP_RGBLoop(void);
void APP_WARMWHITE(void);
void APP_COLDWHITE(void);
void APP_ICE(void);
void APP_SWITCHOFF(void);
void APP_COLDFIRE(void);
void APP_SmallFIRE(void);

#endif