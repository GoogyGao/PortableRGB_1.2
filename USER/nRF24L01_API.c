

//#include "stm8s103f3p.h"
// stm8L151K4
#include "main.h"
#include "nRF24L01_API.h"
#include "ALSensor.h"
//#include "iomacro.h"
#include "LightPwmDrv.h"


#define uchar unsigned char
#define uint unsigned int
/*********     24L01发送接收数据宽度定义	  ***********/
#define TX_ADR_WIDTH    5     //5字节地址宽度
#define RX_ADR_WIDTH    5     //5字节地址宽度
#define TX_PLOAD_WIDTH  32    //32字节有效数据宽度
#define RX_PLOAD_WIDTH  32    //32字节有效数据宽度

const uchar TX_ADDRESS[TX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //发送地址
const uchar RX_ADDRESS[RX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //发送地址

struct _MySensors_ MySensors;
int bright_value = 0;
long int warmcold_value = 0;
uint16_t ch_warmcold_value = 0;
uint16_t pwm_Warm = 0;
uint16_t pwm_Cold = 0;


int Node_Id = 0 ;
int Child_Node_Id = 0;
int Message_Type = 0;
int ACK = 0;
int Device_Type = 0;
int Bright_Value = 0;
int Warm_Cold_Value = 0;
#define BUFFERSIZE    32

#define NRF24L01 1
#define DHT11 '2'
#define VOICESENSOR_GPIO '3'
#define INFRARED '4'
#define CCT_LIGHT '1'
//MEANS 3800
#define CT_SCOPE 38    
#define CT_STEP ((6500-2700)/10)



#if 0
_Bool NRF_CSN   @PC_ODR:3;  //片选信号，推挽输出  
_Bool NRF_IRQ   @PC_IDR:2;  //IRQ主机数据输入,上拉输入
_Bool NRF_CE    @PC_ODR:4;  //发送接收模式选择推挽输出
_Bool NRF_SCK   @PC_ODR:5;  //PC5 推完输出
_Bool NRF_MOSI  @PC_ODR:6;  //PC6 MOSI 主机推挽输出 （根据速率初始化PC_CR2寄存器）
_Bool NRF_MISO  @PC_IDR:7;  //PC7 MISO


typedef struct _NRF_GPIOB_MAP_T {
  uint8_t bit0:1;
  uint8_t bit1:1;
  uint8_t bit2:1;
  uint8_t bit3:1;
  uint8_t bit4:1;
  uint8_t bit5:1;
  uint8_t bit6:1;
  uint8_t bit7:1; 
} nrfGpioMap_t;

nrfGpioMap_t * sgNrfGpioBODRMap = (nrfGpioMap_t*)&GPIOB->ODR;
nrfGpioMap_t * sgNrfGpioBIDRMap = (nrfGpioMap_t*)&GPIOB->IDR;
nrfGpioMap_t * sgNrfGpioAIDRMap = (nrfGpioMap_t*)&GPIOA->IDR;

#define NRF_CSN    sgNrfGpioBODRMap->bit4
#define NRF_CE     sgNrfGpioAODRMap->bit1
#define NRF_SCK    sgNrfGpioBODRMap->bit5
#define NRF_MOSI   sgNrfGpioBODRMap->bit6
#define NRF_MISO   sgNrfGpioBIDRMap->bit7
#define NRF_IRQ    sgNrfGpioBIDRMap->bit0
#endif

#define VS_Input()      ((BitStatus)(GPIOC->IDR & (uint8_t)GPIO_Pin_4))
#define INFRARED_IRQ()  ((BitStatus)(GPIOC->IDR & (uint8_t)GPIO_Pin_0))

// GPIO_SPI for stm8L151k4
void initNrfGPIO_SPI (void)
{
  //NRF_CSN
  GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);
  //NRF_MISO
  GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);
  //NRF_SCK
  GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Slow);
  //NRF_MOSI
  GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Slow);
  //NRF_CE
  GPIO_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Slow);  
  //NRF_IRQ
  GPIO_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_In_FL_No_IT);  
  
  //NRF_CSN(0);
  //NRF_CSN(0x80);
  //NRF_CSN(0);
  //NRF_CSN(0x80);
#if 0
	NRF_SCK(0);
       	NRF_SCK(1);
	NRF_SCK(0);
	NRF_SCK(1);

	NRF_CE(0);
	NRF_CE(1);
	NRF_CE(0);
	NRF_CE(1);
        
	NRF_MOSI(0);
	NRF_MOSI(1);
	NRF_MOSI(0);
	NRF_MOSI(1);
#endif
}

uchar rece_buf[32];

void sendUserPacket (void)
{
	rece_buf[1]=0x45;					//E
	rece_buf[2]=0x61;					//a
	rece_buf[3]=0x73;					//s
	rece_buf[4]=0x74;					//t
	rece_buf[5]=0x66;					//f
	rece_buf[6]=0x69;					//i
	rece_buf[7]=0x65;					//e
	rece_buf[8]=0x6c;					//l
	rece_buf[9]=0x64;					//d
	rece_buf[0]=10;	                                        //一共要发送10个字节，rece_buf[0]必须是10！！！	
	delay_us(100);
        SEND_BUF(rece_buf);
}
    
uint8_t h2i(char c) {
	uint8_t i = 0;
	if (c <= '9')
		i += c - '0';
	else if (c >= 'a')
		i += c - 'a' + 10;
	else
		i += c - 'A' + 10;
	return i;
}
void CCT2ColdWarm (uint16_t ucBright, uint16_t ucWarmCold)
{
 
  if(ucBright>100)
    ucBright = 100;
  if(ucWarmCold>=6500)
    ucWarmCold = 6500;
  if(ucWarmCold<=2700)
    ucWarmCold = 2700;
  
  ucWarmCold -= 2700;
  
  ucWarmCold*= 10;
  
  ucWarmCold/=CT_SCOPE;
  
  pwm_Warm = (1000 - ucWarmCold)*ucBright/1000 ;

  pwm_Cold = ucWarmCold*ucBright/1000 ;
}

int str2dec (uint8_t *c , uint8_t len ,uint8_t start_byte){
    uint16_t i = 0;
    uint8_t cnt = 0;
    for( ; cnt<len ; cnt ++){
      
      if (c[start_byte] <= '9')
      {
          i =i+ (c[start_byte] - '0');
          i *= 10 ;
          start_byte++;
      }
    }
    i /=10;

    return i;

}
uint8_t parameter_offset[8];
void userParseMysensorProtocle (uchar *pbuf)
{
    uint8_t buf_len = 0;
    uint8_t offset = 0 ;
    
    uint8_t parameter_counter = 0;
    
    buf_len = pbuf[0];
    
    //find semicolon
    while(1){
      
      if(pbuf[offset+1]==';'){
        parameter_offset[parameter_counter] =offset ;
        parameter_counter++;
        
      }
      offset++;
      if(offset>buf_len)
      {
        parameter_offset[parameter_counter] =buf_len ;
        break;
      }
    }
     

}

void Nrf_Parser_Protocle (uint8_t *pbuf){
  
      userParseMysensorProtocle(pbuf);
      
      MySensors.NodeId = str2dec(pbuf ,parameter_offset[0] , 1);
      
      MySensors.SensorId = str2dec(pbuf ,parameter_offset[1]-parameter_offset[0] , parameter_offset[0]+2);
      
      MySensors.Messagetype = str2dec(pbuf ,parameter_offset[2]-parameter_offset[1] , parameter_offset[1]+2);
      
      MySensors.Ack = str2dec(pbuf ,parameter_offset[3]-parameter_offset[2] , parameter_offset[2]+2);
      
      MySensors.Devicetype = str2dec(pbuf ,parameter_offset[4]-parameter_offset[3] , parameter_offset[3]+2);
      
      if(MySensors.Ack == 1)
      {
      
      
      }
      
      if(MySensors.Messagetype == 1){
        
      switch(MySensors.Devicetype){
      case S_RGBW_LIGHT :   
        Bright_Value = str2dec(rece_buf ,parameter_offset[5]-parameter_offset[4] , parameter_offset[4]+2);
                
        Warm_Cold_Value = str2dec(rece_buf ,parameter_offset[6]-parameter_offset[5]-1 , parameter_offset[5]+2);
        
        CCT2ColdWarm(Bright_Value,Warm_Cold_Value);
            
        driveColdWarmLightPwm(pwm_Cold,pwm_Warm);
        
        break; 
        
      case S_SOUND :
        MySensors.Value = str2dec(pbuf ,parameter_offset[5]-parameter_offset[4]-1 , parameter_offset[4]+2);//the last param length need -1;
          
        break;
      
      case S_TEMP :
        MySensors.Value = str2dec(pbuf ,parameter_offset[5]-parameter_offset[4]-1 , parameter_offset[4]+2);//the last param length need -1;
        
        break;
        
      case S_HUM :
        MySensors.Value = str2dec(pbuf ,parameter_offset[5]-parameter_offset[4]-1 , parameter_offset[4]+2);//the last param length need -1;
        
        break;
      //use for Infrared sensor
      case S_DIMMER :
        MySensors.Value = str2dec(pbuf ,parameter_offset[5]-parameter_offset[4]-1 , parameter_offset[4]+2);//the last param length need -1;
        
        break;
      }  
      }
         
}


void receiveUserPacket (void)
{
    if(NRF_IRQ==0)	 	// 如果无线模块接收到数据
    {		
            if(NRF24L01_RxPacket(rece_buf)==0)
            {			   
                Usart1_SendString((rece_buf+1));
                
                Nrf_Parser_Protocle(rece_buf);
                  
#if 0
                //CCT  XX;X;X;X;XX;XXXX...
                if(rece_buf[3]==';'&&rece_buf[5]==';'&&rece_buf[7]==';'&&rece_buf[9]==';'&&rece_buf[10]=='2'&&rece_buf[11]=='7'&&rece_buf[12]==';')
                {
                    bright_value = h2i(rece_buf[13])*16+h2i(rece_buf[14]);
                    warmcold_value = h2i(rece_buf[15])*16+h2i(rece_buf[16]);
                    bright_value = bright_value * 100 / 255 ;
                    ch_warmcold_value = warmcold_value *3800 / 255  ;
                    CCT2ColdWarm(bright_value,ch_warmcold_value);
                    
                    driveColdWarmLightPwm(pwm_Cold,pwm_Warm);
                }
#endif   
            } 
    }
}

        
void delay_us(uint16_t num) 
{
  uint16_t i,j; 
  
  for(i=0;i>num;i++)
    for(j=10;j>0;j--);
}
void delay_150us(void)
{
  uint i;
  for(i=0;i>600;i++);
}
uchar SPI_RW(uchar byte)
{
  uchar bit_ctr;
  for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // 输出8位
  {
		//NRF_MOSI=(byte&0x80); 			// MSB TO MOSI
    NRF_MOSI(byte&0x80); 
		byte=(byte<<1);					// shift next bit to MSB
		//NRF_SCK=1;
                NRF_SCK(1);
                if (NRF_MISO)
                  byte = byte | 0x01;	        		// capture current MISO bit
                
		NRF_SCK(0);
  }
  return byte;
}
/*********************************************/
/* 函数功能：给24L01的寄存器写值（一个字节） */
/* 入口参数：reg   要写的寄存器地址          */
/*           value 给寄存器写的值            */
/* 出口参数：status 状态值                   */
/*********************************************/
uchar NRF24L01_Write_Reg(uchar reg,uchar value)
{
	uchar status;

	NRF_CSN(0);                  //CSN=0;   
  	status = SPI_RW(reg);		//发送寄存器地址,并读取状态值
	SPI_RW(value);
	NRF_CSN(1);                  //CSN=1;

	return status;
}
/*************************************************/
/* 函数功能：读24L01的寄存器值 （一个字节）      */
/* 入口参数：reg  要读的寄存器地址               */
/* 出口参数：value 读出寄存器的值                */
/*************************************************/
uchar NRF24L01_Read_Reg(uchar reg)
{
 	uchar value;

	NRF_CSN(0);              //CSN=0;   
  	SPI_RW(reg);			//发送寄存器值(位置),并读取状态值
	value = SPI_RW(NOP);
	NRF_CSN(1);             	//CSN=1;

	return value;
}
/*********************************************/
/* 函数功能：读24L01的寄存器值（多个字节）   */
/* 入口参数：reg   寄存器地址                */
/*           *pBuf 读出寄存器值的存放数组    */
/*           len   数组字节长度              */
/* 出口参数：status 状态值                   */
/*********************************************/
uchar NRF24L01_Read_Buf(uchar reg,uchar *pBuf,uchar len)
{
	uchar status,u8_ctr;
	NRF_CSN(0);                   	//CSN=0       
	status=SPI_RW(reg);				//发送寄存器地址,并读取状态值   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	pBuf[u8_ctr]=SPI_RW(0XFF);		//读出数据
	NRF_CSN(1);                 		//CSN=1
	return status;        			//返回读到的状态值
}
/**********************************************/
/* 函数功能：给24L01的寄存器写值（多个字节）  */
/* 入口参数：reg  要写的寄存器地址            */
/*           *pBuf 值的存放数组               */
/*           len   数组字节长度               */
/**********************************************/
uchar NRF24L01_Write_Buf(uchar reg, uchar *pBuf, uchar len)
{
	uchar status,u8_ctr;
	NRF_CSN(0);
	status = SPI_RW(reg);			//发送寄存器值(位置),并读取状态值
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	SPI_RW(*pBuf++); 				//写入数据
	NRF_CSN(1);
  return status;          		//返回读到的状态值
}							  					   

/*********************************************/
/* 函数功能：24L01接收数据                   */
/* 入口参数：rxbuf 接收数据数组              */
/* 返回值： 0   成功收到数据                 */
/*          1   没有收到数据                 */
/*********************************************/
uchar NRF24L01_RxPacket(uchar *rxbuf)
{
	uchar state;
	state=NRF24L01_Read_Reg(STATUS);  			//读取状态寄存器的值    	 
	NRF24L01_Write_Reg(WRITE_REG+STATUS,state); //清除TX_DS或MAX_RT中断标志
	if(state&RX_OK)								//接收到数据
	{
		NRF_CE(0);
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);					//清除RX FIFO寄存器
		NRF_CE(1);
		delay_150us(); 
		return 0; 
	}	   
	return 1;//没收到任何数据
}
/**********************************************/
/* 函数功能：设置24L01为发送模式              */
/* 入口参数：txbuf  发送数据数组              */
/* 返回值； 0x10    达到最大重发次数，发送失败*/
/*          0x20    成功发送完成              */
/*          0xff    发送失败                  */
/**********************************************/
uchar NRF24L01_TxPacket(uchar *txbuf)
{
	uchar state;
   
	NRF_CE(0);						//CE拉低，使能24L01配置
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);	//写数据到TX BUF  32个字节
 	NRF_CE(1);						//CE置高，使能发送	   
	while(NRF_IRQ==1);					//等待发送完成
	state=NRF24L01_Read_Reg(STATUS);  			//读取状态寄存器的值	   
	NRF24L01_Write_Reg(WRITE_REG+STATUS,state); 		//清除TX_DS或MAX_RT中断标志
	if(state&MAX_TX)					//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);		//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(state&TX_OK)						//发送完成
	{
          
		return TX_OK;
	}
	return 0xff;						//发送失败
}

/********************************************/
/* 函数功能：检测24L01是否存在              */
/* 返回值；  0  存在                        */
/*           1  不存在                      */
/********************************************/ 	  
uchar NRF24L01_Check(void)
{
	uchar check_in_buf[5]={0x11,0x22,0x33,0x44,0x55};
	uchar check_out_buf[5]={0x00};

	NRF_SCK(0);
	NRF_CSN(1);    
	NRF_CE(0);

	NRF24L01_Write_Buf(WRITE_REG+TX_ADDR, check_in_buf, 5);

	NRF24L01_Read_Buf(READ_REG+TX_ADDR, check_out_buf, 5);

	if((check_out_buf[0] == 0x11)&&\
	   (check_out_buf[1] == 0x22)&&\
	   (check_out_buf[2] == 0x33)&&\
	   (check_out_buf[3] == 0x44)&&\
	   (check_out_buf[4] == 0x55))return 0;
	else return 1;
}			


void NRF24L01_RT_Init(void)
{	
	NRF_CE(0);		  
  	NRF24L01_Write_Reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
	NRF24L01_Write_Reg(FLUSH_RX,0xff);									//清除RX FIFO寄存器    
  	NRF24L01_Write_Buf(WRITE_REG+TX_ADDR,(uchar*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  	NRF24L01_Write_Buf(WRITE_REG+RX_ADDR_P0,(uchar*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  
  	NRF24L01_Write_Reg(WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  	NRF24L01_Write_Reg(WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01_Write_Reg(WRITE_REG+RF_CH,0);        //设置RF通道为2.400GHz  频率=2.4+0GHz
  	NRF24L01_Write_Reg(WRITE_REG+RF_SETUP,0x0F);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(WRITE_REG+CONFIG,0x0f);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF_CE(1);									  //CE置高，使能发送
}

void SEND_BUF(uchar *buf)
{
	NRF_CE(0);
	NRF24L01_Write_Reg(WRITE_REG+CONFIG,0x0e);
	NRF_CE(1);
	//delay_us(300);
	NRF24L01_TxPacket(buf);
        Delay(500); //10ms
	NRF_CE(0);
	NRF24L01_Write_Reg(WRITE_REG+CONFIG, 0x0f);
	NRF_CE(1);	
}

void MyNrfSend_Buf(uchar *buf)
{
        NRF_CE(0);
	NRF24L01_Write_Reg(WRITE_REG+CONFIG,0x0e);
	NRF_CE(1);
	//delay_us(1000);
	NRF24L01_TxPacket(buf);

}

void NRF_RX_MODE (void)
{
    NRF_CE(0);
    NRF24L01_Write_Reg(WRITE_REG+CONFIG, 0x0f);
    NRF_CE(1);

}