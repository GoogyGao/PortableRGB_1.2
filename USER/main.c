#include "stm8l15x.h"
/**
  ******************************************************************************
  * @file    RTC/RTC_Calendar/main.c
  * @author  
  * @version V1.0
  * @date    PortableLamp
  * @brief   WS2812B  Driver & APPLICATION 
  ******************************************************************************

  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "UartDev.h"
#include "firelamp.h"
#include "LightPwmDrv.h"
/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_Calendar
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef struct _Light_Usart {
  uint8_t StripLightId;
  uint8_t CtrlType;
  uint8_t ColorR1;
  uint8_t ColorG1;
  uint8_t ColorB1;
  uint8_t Brightness1;
  uint8_t Switch1;
  uint8_t Lednum;
  uint8_t CtrlTime;
  uint8_t Scene;

}_Light_Usart_;

struct _ReturnRGBParam{
  uint8_t r[1];
  uint8_t g[1];
  uint8_t b[1];
  uint8_t w[1];
  uint8_t scene[1];
  uint8_t rgb_onoff[1];
};

//struct _Light_Usart Light_Usart;

typedef struct _BLE_Usart {
  
  _Light_Usart_ LightDiscribe;

}_BLE_Usart_;
  
typedef enum{
  USART_SWITCH = 0x01,
  USART_BRIGHTNESS,
  USART_RGB,
  USART_RGBW,
  USART_SCENE,
}USART_CMD;

//struct _Light_Usart Light_Usart;


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Buff_Cnt = 0;
extern uint8_t TouchFlag;
struct _BLE_Usart MyBLE_Usart;
struct _ReturnRGBParam ReturnRGBParam;

uint8_t SWITCH_STATUS = 2;//1 open / 0 close
extern uint8_t FLASHFLAG;
uint8_t RETURN_FLAG  = 0;
uint8_t USART1_FLAG = 0;
uint8_t USART1_ReceiveDataBuf[20];
uint8_t BLEFLAG = 0;
uint8_t SWITCH_SAVE_FLAG = 0;
uint8_t SWITCHOFF = 0;
uint8_t ErrorTip [] = "INPUT ERROR!\n";

/* Public functions ---------------------------------------------------------*/
uint8_t RGBColor[] = { 0xff, 0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff,
                       0xff, 0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff,
                       0xff, 0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff,
                       0xff, 0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff, 0xff,0xff,0xff,
};
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);




/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure PVD
  * @param  None
  * @retval None
  */
static void PVD_Config(void)
{
  /* select PVD threshold*/
  PWR_PVDLevelConfig(PWR_PVDLevel_2V85);

  /*Enable PVD  : optional (PVD is by default enabled)*/
  PWR_PVDCmd(ENABLE);

  /*Enable PVD Interrupt*/
  PWR_PVDITConfig(ENABLE);
}


void RGBCtrl(uint8_t RValue, uint8_t GValue, uint8_t BValue,uint8_t LedNum,uint8_t time)
{
  uint8_t count ;
  LedNum*=3;
  for(count = 0 ; count < LedNum ; count+=3)
  {
    RGBColor[count] = GValue;
    RGBColor[count+1] = RValue;
    RGBColor[count+2] = BValue;
  }
  
  SEND_CODE_DATA(RGBColor ,LedNum, FIRE_LAMP_PIN,time);

}




#define TEST_EEPROM_OPERATION 0
#if TEST_EEPROM_OPERATION
void test_eeprom (void)
{
    FLASH_Unlock(FLASH_MemType_Data);
    NVRAM[0] = 0x06 + 2;
    NVRAM[1] = 0x05 + 2;
    NVRAM[2] = 0x04 + 2;
    NVRAM[3] = 0x03 + 2;
    NVRAM[4] = 0x02 + 2;
    NVRAM[5] = 0x01 + 2;
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    FLASH_Lock(FLASH_MemType_Data);
}
#endif
void InitLightParams()
{
  MyBLE_Usart.LightDiscribe.Lednum = 49;
  MyBLE_Usart.LightDiscribe.CtrlTime = 0;
/*
  if(Nvram_Read(enmE2promMark)==10)
  {
    MyBLE_Usart.LightDiscribe.ColorR1 = Nvram_Read(enmColorR1);
    MyBLE_Usart.LightDiscribe.ColorG1 = Nvram_Read(enmColorG1);
    MyBLE_Usart.LightDiscribe.ColorB1 = Nvram_Read(enmColorB1);
    MyBLE_Usart.LightDiscribe.Brightness1 = Nvram_Read(enmBrightness1);
    MyBLE_Usart.LightDiscribe.Switch1 = Nvram_Read(enmPowerOnOffState1);
    MyBLE_Usart.LightDiscribe.Scene = Nvram_Read(enmScene);
    
 
  }else 
 // {
*/
    MyBLE_Usart.LightDiscribe.ColorR1 = 255;
    MyBLE_Usart.LightDiscribe.ColorG1 = 255;
    MyBLE_Usart.LightDiscribe.ColorB1 = 255;
    MyBLE_Usart.LightDiscribe.Brightness1 = 100;
    MyBLE_Usart.LightDiscribe.Switch1 = 1 ;
    MyBLE_Usart.LightDiscribe.Scene = 0;
   
 // }
 // if(MyBLE_Usart.LightDiscribe.Brightness1 == 0)
 //   MyBLE_Usart.LightDiscribe.Brightness1 = 100;
  if(MyBLE_Usart.LightDiscribe.Scene == 0)
    RGBLightCtr(MyBLE_Usart.LightDiscribe.ColorR1, MyBLE_Usart.LightDiscribe.ColorG1, MyBLE_Usart.LightDiscribe.ColorB1 ,
                        MyBLE_Usart.LightDiscribe.Brightness1 ,MyBLE_Usart.LightDiscribe.CtrlTime ,MyBLE_Usart.LightDiscribe.Lednum );
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
//TIM2_GetCounter()    该函数用来获取定时器计数的值
void main(void)
{
    uint8_t i, UsartSendState;
    CLK_DeInit();
    //After reset, the device restarts by default with the HSI clock divided by 8.
    /* High speed internal clock prescaler: 1 */
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);  // now: HSI=16M prescale = 1; sysclk = 16M
    //CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);  // now: HSI=16M prescale = 16; sysclk = 1M
    Usart1_Config();
    
    LampGpioInit();
    
    
    enableInterrupts(); //open all IT
    Delay(8000);//wait for led power supply normally
    InitLightParams();
    
    
    
    while (1)
    {
      //RGBLightCtr(255,0,0,10,0,49);
      //APP_WARMWHITE();
      //APP_BigFIRE();
      //APP_SmallFIRE();
#if 1
      if((USART1_FLAG == 1)&&(USART1_ReceiveDataBuf[0] !=0x00))
      {
        for(i = 0 ; i < 9 ;i++)
        {
          USART1_ReceiveDataBuf[i] = USART1_ReceiveDataBuf[i+1];
        }
      }
      if((USART1_FLAG == 1)&&(USART1_ReceiveDataBuf[0]  ==0x00))
      {
        
        /*TIM4 counter enable */
        //TIM4_Cmd(ENABLE);
        //USART1_ReceiveDataBuf[4] represent device type
        //0001	便携式RGB
        //0010	三色纸灯
        //0011	运动射灯
        //0100	智能灯头
        MyBLE_Usart.LightDiscribe.StripLightId = USART1_ReceiveDataBuf[1];
        MyBLE_Usart.LightDiscribe.CtrlType = USART1_ReceiveDataBuf[2];
        MyBLE_Usart.LightDiscribe.Scene = 0;
        if((MyBLE_Usart.LightDiscribe.StripLightId == 0x20)||(MyBLE_Usart.LightDiscribe.StripLightId == 0x2f))//ctrl light1 RGB
        {
          if((MyBLE_Usart.LightDiscribe.CtrlType != 1))
            MyBLE_Usart.LightDiscribe.Switch1 = 1;
          switch(MyBLE_Usart.LightDiscribe.CtrlType)
          {
            case USART_SWITCH:
          MyBLE_Usart.LightDiscribe.Switch1 = USART1_ReceiveDataBuf[4];
          SWITCH_STATUS = USART1_ReceiveDataBuf[4];
          if(MyBLE_Usart.LightDiscribe.Switch1)
            RGBLightCtr(MyBLE_Usart.LightDiscribe.ColorR1, MyBLE_Usart.LightDiscribe.ColorG1, MyBLE_Usart.LightDiscribe.ColorB1 ,
                        MyBLE_Usart.LightDiscribe.Brightness1 ,MyBLE_Usart.LightDiscribe.CtrlTime ,MyBLE_Usart.LightDiscribe.Lednum );
          else
            RGBLightCtr(0, 0, 0 ,
                        MyBLE_Usart.LightDiscribe.Brightness1 ,MyBLE_Usart.LightDiscribe.CtrlTime ,MyBLE_Usart.LightDiscribe.Lednum );
          Usart1_SendByte(0);
          break;
        
        case USART_BRIGHTNESS:
          MyBLE_Usart.LightDiscribe.Brightness1 = USART1_ReceiveDataBuf[4];
          RGBLightCtr(MyBLE_Usart.LightDiscribe.ColorR1, MyBLE_Usart.LightDiscribe.ColorG1, MyBLE_Usart.LightDiscribe.ColorB1 ,
                        MyBLE_Usart.LightDiscribe.Brightness1 ,MyBLE_Usart.LightDiscribe.CtrlTime ,MyBLE_Usart.LightDiscribe.Lednum );
          Usart1_SendByte(0);
          break;
        case USART_RGB:
          MyBLE_Usart.LightDiscribe.ColorR1 = USART1_ReceiveDataBuf[4];
          MyBLE_Usart.LightDiscribe.ColorG1 = USART1_ReceiveDataBuf[5];
          MyBLE_Usart.LightDiscribe.ColorB1 = USART1_ReceiveDataBuf[6];
          RGBLightCtr(MyBLE_Usart.LightDiscribe.ColorR1, MyBLE_Usart.LightDiscribe.ColorG1, MyBLE_Usart.LightDiscribe.ColorB1 ,
                        MyBLE_Usart.LightDiscribe.Brightness1 ,MyBLE_Usart.LightDiscribe.CtrlTime ,MyBLE_Usart.LightDiscribe.Lednum );
          
          Usart1_SendByte(0);
          break;
          
        case USART_SCENE:
          MyBLE_Usart.LightDiscribe.Scene = USART1_ReceiveDataBuf[4];

          Usart1_SendByte(0);
          break;

        }
        }
        else
        {
          //operate faild
          Usart1_SendByte(1);
        }
#if 0
          //eeprom SAVE PARAM 
          NvramWrite_TrancBegin ();
          
          NvramWrite_Tranction(enmColorR1,MyBLE_Usart.LightDiscribe.ColorR1);
          NvramWrite_Tranction(enmColorG1,MyBLE_Usart.LightDiscribe.ColorG1);
          NvramWrite_Tranction(enmColorB1,MyBLE_Usart.LightDiscribe.ColorB1);
          NvramWrite_Tranction(enmBrightness1,MyBLE_Usart.LightDiscribe.Brightness1);
          NvramWrite_Tranction(enmPowerOnOffState1,MyBLE_Usart.LightDiscribe.Switch1);
          NvramWrite_Tranction(enmScene,MyBLE_Usart.LightDiscribe.Scene);

          NvramWrite_Tranction(enmE2promMark,10);//set FLAG to save Light params
          NvramWrite_TrancEnd ();
#endif
          USART1_FLAG = 0;
          Buff_Cnt = 0;
          BLEFLAG = 1;
         
      }
      //send save param to BT module
      if((RETURN_FLAG == 1)&&(USART1_ReceiveDataBuf[0] == 0xff))
      {
        ReturnRGBParam.r[0] =  MyBLE_Usart.LightDiscribe.ColorR1;
        ReturnRGBParam.g[0] =  MyBLE_Usart.LightDiscribe.ColorG1;
        ReturnRGBParam.b[0] =  MyBLE_Usart.LightDiscribe.ColorB1;
        ReturnRGBParam.w[0] =  MyBLE_Usart.LightDiscribe.Brightness1;
        ReturnRGBParam.rgb_onoff[0]  = MyBLE_Usart.LightDiscribe.Switch1;
        ReturnRGBParam.scene[0] =  MyBLE_Usart.LightDiscribe.Scene;
        
   
        //on/off stAte processing
        uint8_t OnOffState = 0;
        if((SWITCH_STATUS == 0)&&(USART1_ReceiveDataBuf[0] == 0xff))
        {
          OnOffState = (MyBLE_Usart.LightDiscribe.StripLightId & 0x0f);
          switch(OnOffState)
          {
          case 0:
            ReturnRGBParam.rgb_onoff[0]  = 0;
            break;
          case 1:
            ReturnRGBParam.rgb_onoff[1]  = 0;
            break;
          case 0x0f:
            ReturnRGBParam.rgb_onoff[0]  = 0;
            ReturnRGBParam.rgb_onoff[1]  = 0;
            break;
          }
        
        }
        
        UsartSendState = Usart1_SendDatArray((uint8_t *)&ReturnRGBParam ,6);
        
        Usart1_SendByte(UsartSendState);//success return 1,fail return 2
        RETURN_FLAG  = 0;
        Buff_Cnt = 0;
      }
      if(BLEFLAG == 0)
      {
        
        if(TouchFlag ==1)
          MyBLE_Usart.LightDiscribe.Scene = 1;
        else if(TouchFlag ==2)
          MyBLE_Usart.LightDiscribe.Scene = 2;
        else if(TouchFlag ==3)
          MyBLE_Usart.LightDiscribe.Scene = 3;
        else if(TouchFlag ==4)
          MyBLE_Usart.LightDiscribe.Scene = 4;
        else if(TouchFlag ==5)
          MyBLE_Usart.LightDiscribe.Scene = 5;
        else if(TouchFlag ==6)
          MyBLE_Usart.LightDiscribe.Scene = 6;
        else if(TouchFlag ==7)
          MyBLE_Usart.LightDiscribe.Scene = 7;
      }
      
      if(MyBLE_Usart.LightDiscribe.Scene == 1)
        APP_ICE();
      else if(MyBLE_Usart.LightDiscribe.Scene == 2)
        APP_COLDFIRE();
      else if(MyBLE_Usart.LightDiscribe.Scene == 3)
        APP_BigFIRE();
      else if(MyBLE_Usart.LightDiscribe.Scene == 4)
        APP_SmallFIRE();
      else if(MyBLE_Usart.LightDiscribe.Scene == 5)
        APP_WARMWHITE();
      else if(MyBLE_Usart.LightDiscribe.Scene == 6)
        APP_COLDWHITE();
      else if(MyBLE_Usart.LightDiscribe.Scene == 7)
        APP_SWITCHOFF();
      
      
#endif
      
    }
}


/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)   
{
  nCount *=1;
  for (; nCount != 0; nCount--)
  {
    //if((BLEFLAG==0)||(RETURN_FLAG==1)||(USART1_FLAG==1))
     // nCount = 0;
  }
}



