#include "main.h"
#include "led.h"
#include "key.h"
#include "spi.h"
#include "delay.h"
#include "timer1.h"
#include "lora.h"

#define MASTER 0

void Variable_Init(void)
{
	SysTime        = 0x00;
	mode           = 0x01;//lora mode
	Freq_Sel       = 0x00;//433M
	Power_Sel      = 0x00;//
	Lora_Rate_Sel  = 0x06;//
	BandWide_Sel   = 0x07;
	Fsk_Rate_Sel   = 0x00;
}

void Hardware_Init(void)
{
	CLK->CKDIVR = 0x00;
	delay_ms(100);
	//LED_Init();
	//KEY_Init();
	SPI_GPIO_Init();
	TIMER1_Init();
}

void main(void)
{
	Variable_Init();
	_asm("sim");
	Hardware_Init();
	_asm("rim");
	TIM1->CR1   |= 0x01;
	TIM2->CR1   |= 0x01;
	sx1276_7_8_Config();
	sx1276_7_8_LoRaEntryRx();
	
  while (1)
  {
		if(MASTER)
		{
			if(key_flag)
			{
				key_flag = 0;
				SysTime = 0;
				RED_LED_H();
				sx1276_7_8_LoRaEntryTx();
				sx1276_7_8_LoRaTxPacket();
				RED_LED_L();
				sx1276_7_8_LoRaEntryRx();
				delay_ms(3);

			}
			if(sx1276_7_8_LoRaRxPacket())
			{
				GREEN_LED_H();
				delay_ms(200);
				GREEN_LED_L();
			}
		}
		else
		{
			if(sx1276_7_8_LoRaRxPacket())
			{
				GREEN_LED_H();
				delay_ms(100);
				GREEN_LED_L();
				
				RED_LED_H();
				sx1276_7_8_LoRaEntryTx();
				sx1276_7_8_LoRaTxPacket();
				RED_LED_L();
				sx1276_7_8_LoRaEntryRx();
				delay_ms(3);

			}
		}
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
  }
}
#endif

