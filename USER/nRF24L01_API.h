#include "nRF24L01.h"

#define uchar unsigned char
#define uint unsigned int


#define NRF_CSN(x)      {if ((x) > 0) GPIOB->ODR |= GPIO_Pin_4; else GPIOB->ODR &= ~GPIO_Pin_4;}
#define NRF_CE(x)       {if ((x) > 0) GPIOA->ODR |= GPIO_Pin_6; else GPIOA->ODR &= ~GPIO_Pin_6;}
#define NRF_SCK(x)      {if ((x) > 0) GPIOB->ODR |= GPIO_Pin_5; else GPIOB->ODR &= ~GPIO_Pin_5;}
#define NRF_MOSI(x)     {if ((x) > 0) GPIOB->ODR |= GPIO_Pin_6; else GPIOB->ODR &= ~GPIO_Pin_6;}
#define NRF_MISO        ((BitStatus)(GPIOB->IDR & (uint8_t)GPIO_Pin_7))
#define NRF_IRQ         ((BitStatus)(GPIOA->IDR & (uint8_t)GPIO_Pin_4))

void delay_us(uint16_t num);
void delay_150us(void);
uchar SPI_RW(uchar byte);
uchar NRF24L01_Write_Reg(uchar reg,uchar value);
uchar NRF24L01_Read_Reg(uchar reg);
uchar NRF24L01_Read_Buf(uchar reg,uchar *pBuf,uchar len);
uchar NRF24L01_Write_Buf(uchar reg, uchar *pBuf, uchar len);
uchar NRF24L01_RxPacket(uchar *rxbuf);
uchar NRF24L01_TxPacket(uchar *txbuf);
uchar NRF24L01_Check(void);
void NRF24L01_RT_Init(void);
void SEND_BUF(uchar *buf);
void MyNrfSend_Buf(uchar *buf);
void NRF_RX_MODE (void);

void initNrfGPIO_SPI (void);
void sendUserPacket (void);
void receiveUserPacket (void);