#include "stm8s.h"
#include "stm8s_spi.h"
#include "spi.h"
#include "delay.h"

void SPI_GPIO_Init(void)
{
	GPIO_Init(nSEL_PORT,(GPIO_Pin_TypeDef)nSEL_PINS,GPIO_MODE_OUT_PP_LOW_FAST);	
	GPIO_Init(SCK_PORT, (GPIO_Pin_TypeDef)SCK_PINS, GPIO_MODE_OUT_PP_LOW_FAST);	
	GPIO_Init(SDI_PORT, (GPIO_Pin_TypeDef)SDI_PINS, GPIO_MODE_OUT_PP_LOW_FAST);	
	GPIO_Init(SDN_PORT, (GPIO_Pin_TypeDef)SDN_PINS, GPIO_MODE_OUT_PP_LOW_FAST);	
	GPIO_Init(SDO_PORT, (GPIO_Pin_TypeDef)SDO_PINS, GPIO_MODE_IN_PU_NO_IT);	
	//Below is for Lora TX done pin
	GPIO_Init(TX_DONE_PORT, (GPIO_Pin_TypeDef)TX_DONE_PINS, GPIO_MODE_IN_PU_IT);
	// 从右至左依次2位是ABCD口的设置,这里C口
	EXTI->CR1 |= 0x30;
	//EXTI->CR1 |= 0x0C;
	SDN_L();
	delay_ms(50);
	SDN_H();	
}

void SPICmd8bit(u8 WrPara)
{
  u8 bitcnt;  
  nSEL_L();
  SCK_L();
  
  for(bitcnt=8; bitcnt!=0; bitcnt--)
  {
    SCK_L();
    if(WrPara&0x80)
      SDI_H();
    else
      SDI_L();
    SCK_H();
    WrPara <<= 1;
  }
  SCK_L();
  SDI_H();
}

u8 SPIRead8bit(void)
{
  u8 RdPara = 0;
  u8 bitcnt;
 
  nSEL_L();
  SDI_H();                                                 //Read one byte data from FIFO, MOSI hold to High
  for(bitcnt=8; bitcnt!=0; bitcnt--)
  {
    SCK_L();
    RdPara <<= 1;
    SCK_H();
    if(Get_SDO())
      RdPara |= 0x01;
    else
      RdPara |= 0x00;
  }
  SCK_L();
  return(RdPara);
} 
 
u8 SPIRead(u8 adr)
{
  u8 tmp; 
  SPICmd8bit(adr);                                         //Send address first
  tmp = SPIRead8bit();  
  nSEL_H();
  return(tmp);
} 
 
void SPIWrite(u8 adr, u8 WrPara)  
{
	
	nSEL_L();						
 
	SPICmd8bit(adr|0x80);		 //写入地址 
	SPICmd8bit(WrPara);//写入数据
	
	SCK_L();
  SDI_H();
  nSEL_H();
} 
 
void SPIBurstRead(u8 adr, u8 *ptr, u8 length)
{
  u8 i;
  if(length<=1)                                            //length must more than one
    return;
  else
  {
    SCK_L(); 
    nSEL_L();
    SPICmd8bit(adr); 
    for(i=0;i<length;i++)
    ptr[i] = SPIRead8bit();
    nSEL_H();  
  }
}

void BurstWrite(u8 adr, u8 *ptr, u8 length)
{ 
  u8 i;

  if(length<=1)                                            //length must more than one
    return;
  else  
  {   
    SCK_L();
    nSEL_L();        
    SPICmd8bit(adr|0x80);
    for(i=0;i<length;i++)
			SPICmd8bit(ptr[i]);
    nSEL_H();  
  }
}





































