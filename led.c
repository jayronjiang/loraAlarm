#include "stm8s.h"
#include "led.h"
#include "delay.h"

void LED_Init(void)
{
	GPIO_Init(LED1_GPIO_PORT, (GPIO_Pin_TypeDef)LED1_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
	//GPIO_Init(LED2_GPIO_PORT, (GPIO_Pin_TypeDef)LED2_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
	GREEN_LED_H();
	//RED_LED_H();
	delay_ms(500);
	GREEN_LED_L();
	//RED_LED_L();
}

