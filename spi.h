#ifndef __SPI_H__
#define __SPI_H__

#include "stm8s.h"

// SPI-NSS  PA3
#define nSEL_PORT  (GPIOA)				
#define nSEL_PINS  (GPIO_PIN_3)

//SPI-SCK  PC5
#define SCK_PORT  (GPIOC)
#define SCK_PINS  (GPIO_PIN_5)

// SPI-MOSI PC6
#define SDI_PORT  (GPIOC)
#define SDI_PINS  (GPIO_PIN_6)

// SPI-MISO PC7
#define SDO_PORT  (GPIOC)
#define SDO_PINS  (GPIO_PIN_7)

// SPI-RESET PC3
#define SDN_PORT  (GPIOC)
#define SDN_PINS  (GPIO_PIN_3)

// DIO0	PB4
#define TX_DONE_PORT  (GPIOB)
#define TX_DONE_PINS  (GPIO_PIN_4)


#define nSEL_H() GPIO_WriteHigh(nSEL_PORT,(GPIO_Pin_TypeDef)nSEL_PINS)
#define nSEL_L() GPIO_WriteLow(nSEL_PORT, (GPIO_Pin_TypeDef)nSEL_PINS)
#define SCK_H()  GPIO_WriteHigh(SCK_PORT, (GPIO_Pin_TypeDef)SCK_PINS)
#define SCK_L()  GPIO_WriteLow(SCK_PORT,  (GPIO_Pin_TypeDef)SCK_PINS)
#define SDI_H()  GPIO_WriteHigh(SDI_PORT, (GPIO_Pin_TypeDef)SDI_PINS)
#define SDI_L()  GPIO_WriteLow(SDI_PORT,  (GPIO_Pin_TypeDef)SDI_PINS)
#define SDN_H()  GPIO_WriteHigh(SDN_PORT, (GPIO_Pin_TypeDef)SDN_PINS)
#define SDN_L()  GPIO_WriteLow(SDN_PORT,  (GPIO_Pin_TypeDef)SDN_PINS)
#define Get_SDO()GPIO_ReadInputPin(SDO_PORT, (GPIO_Pin_TypeDef)SDO_PINS)
#define Get_NIRQ()GPIO_ReadInputPin(TX_DONE_PORT, (GPIO_Pin_TypeDef)TX_DONE_PINS)

extern void SPI_GPIO_Init(void);
extern void SPICmd8bit(u8 WrPara);
extern u8 SPIRead(u8 adr);
extern u8 SPIRead8bit(void);
extern void SPIWrite(u8 adr, u8 WrPara);
extern void SPIBurstRead(u8 adr, u8 *ptr, u8 length);
extern void BurstWrite(u8 adr, u8 *ptr, u8 length);


#endif