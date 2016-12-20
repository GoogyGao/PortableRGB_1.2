

//#include "stm8s103f3p.h"
// stm8L151K4
#include "main.h"
#include "nRF24L01_API.h"
#include "ALSensor.h"
//#include "iomacro.h"
#include "LightPwmDrv.h"


#define uchar unsigned char
#define uint unsigned int
/*********     24L01���ͽ������ݿ�ȶ���	  ***********/
#define TX_ADR_WIDTH    5     //5�ֽڵ�ַ���
#define RX_ADR_WIDTH    5     //5�ֽڵ�ַ���
#define TX_PLOAD_WIDTH  32    //32�ֽ���Ч���ݿ��
#define RX_PLOAD_WIDTH  32    //32�ֽ���Ч���ݿ��

const uchar TX_ADDRESS[TX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //���͵�ַ
const uchar RX_ADDRESS[RX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //���͵�ַ

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
_Bool NRF_CSN   @PC_ODR:3;  //Ƭѡ�źţ��������  
_Bool NRF_IRQ   @PC_IDR:2;  //IRQ������������,��������
_Bool NRF_CE    @PC_ODR:4;  //���ͽ���ģʽѡ���������
_Bool NRF_SCK   @PC_ODR:5;  //PC5 �������
_Bool NRF_MOSI  @PC_ODR:6;  //PC6 MOSI ����������� ���������ʳ�ʼ��PC_CR2�Ĵ�����
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
	rece_buf[0]=10;	                                        //һ��Ҫ����10���ֽڣ�rece_buf[0]������10������	
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
    if(NRF_IRQ==0)	 	// �������ģ����յ�����
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
  for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // ���8λ
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
/* �������ܣ���24L01�ļĴ���дֵ��һ���ֽڣ� */
/* ��ڲ�����reg   Ҫд�ļĴ�����ַ          */
/*           value ���Ĵ���д��ֵ            */
/* ���ڲ�����status ״ֵ̬                   */
/*********************************************/
uchar NRF24L01_Write_Reg(uchar reg,uchar value)
{
	uchar status;

	NRF_CSN(0);                  //CSN=0;   
  	status = SPI_RW(reg);		//���ͼĴ�����ַ,����ȡ״ֵ̬
	SPI_RW(value);
	NRF_CSN(1);                  //CSN=1;

	return status;
}
/*************************************************/
/* �������ܣ���24L01�ļĴ���ֵ ��һ���ֽڣ�      */
/* ��ڲ�����reg  Ҫ���ļĴ�����ַ               */
/* ���ڲ�����value �����Ĵ�����ֵ                */
/*************************************************/
uchar NRF24L01_Read_Reg(uchar reg)
{
 	uchar value;

	NRF_CSN(0);              //CSN=0;   
  	SPI_RW(reg);			//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	value = SPI_RW(NOP);
	NRF_CSN(1);             	//CSN=1;

	return value;
}
/*********************************************/
/* �������ܣ���24L01�ļĴ���ֵ������ֽڣ�   */
/* ��ڲ�����reg   �Ĵ�����ַ                */
/*           *pBuf �����Ĵ���ֵ�Ĵ������    */
/*           len   �����ֽڳ���              */
/* ���ڲ�����status ״ֵ̬                   */
/*********************************************/
uchar NRF24L01_Read_Buf(uchar reg,uchar *pBuf,uchar len)
{
	uchar status,u8_ctr;
	NRF_CSN(0);                   	//CSN=0       
	status=SPI_RW(reg);				//���ͼĴ�����ַ,����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	pBuf[u8_ctr]=SPI_RW(0XFF);		//��������
	NRF_CSN(1);                 		//CSN=1
	return status;        			//���ض�����״ֵ̬
}
/**********************************************/
/* �������ܣ���24L01�ļĴ���дֵ������ֽڣ�  */
/* ��ڲ�����reg  Ҫд�ļĴ�����ַ            */
/*           *pBuf ֵ�Ĵ������               */
/*           len   �����ֽڳ���               */
/**********************************************/
uchar NRF24L01_Write_Buf(uchar reg, uchar *pBuf, uchar len)
{
	uchar status,u8_ctr;
	NRF_CSN(0);
	status = SPI_RW(reg);			//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	SPI_RW(*pBuf++); 				//д������
	NRF_CSN(1);
  return status;          		//���ض�����״ֵ̬
}							  					   

/*********************************************/
/* �������ܣ�24L01��������                   */
/* ��ڲ�����rxbuf ������������              */
/* ����ֵ�� 0   �ɹ��յ�����                 */
/*          1   û���յ�����                 */
/*********************************************/
uchar NRF24L01_RxPacket(uchar *rxbuf)
{
	uchar state;
	state=NRF24L01_Read_Reg(STATUS);  			//��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(WRITE_REG+STATUS,state); //���TX_DS��MAX_RT�жϱ�־
	if(state&RX_OK)								//���յ�����
	{
		NRF_CE(0);
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);					//���RX FIFO�Ĵ���
		NRF_CE(1);
		delay_150us(); 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}
/**********************************************/
/* �������ܣ�����24L01Ϊ����ģʽ              */
/* ��ڲ�����txbuf  ������������              */
/* ����ֵ�� 0x10    �ﵽ����ط�����������ʧ��*/
/*          0x20    �ɹ��������              */
/*          0xff    ����ʧ��                  */
/**********************************************/
uchar NRF24L01_TxPacket(uchar *txbuf)
{
	uchar state;
   
	NRF_CE(0);						//CE���ͣ�ʹ��24L01����
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);	//д���ݵ�TX BUF  32���ֽ�
 	NRF_CE(1);						//CE�øߣ�ʹ�ܷ���	   
	while(NRF_IRQ==1);					//�ȴ��������
	state=NRF24L01_Read_Reg(STATUS);  			//��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(WRITE_REG+STATUS,state); 		//���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_TX)					//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);		//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(state&TX_OK)						//�������
	{
          
		return TX_OK;
	}
	return 0xff;						//����ʧ��
}

/********************************************/
/* �������ܣ����24L01�Ƿ����              */
/* ����ֵ��  0  ����                        */
/*           1  ������                      */
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
  	NRF24L01_Write_Reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
	NRF24L01_Write_Reg(FLUSH_RX,0xff);									//���RX FIFO�Ĵ���    
  	NRF24L01_Write_Buf(WRITE_REG+TX_ADDR,(uchar*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(WRITE_REG+RX_ADDR_P0,(uchar*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
  	NRF24L01_Write_Reg(WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(WRITE_REG+RF_CH,0);        //����RFͨ��Ϊ2.400GHz  Ƶ��=2.4+0GHz
  	NRF24L01_Write_Reg(WRITE_REG+RF_SETUP,0x0F);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(WRITE_REG+CONFIG,0x0f);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF_CE(1);									  //CE�øߣ�ʹ�ܷ���
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