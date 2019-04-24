#ifndef __LED_H__
#define __LED_H__

#include "stm8s.h"

//#define LED1_GPIO_PORT  (GPIOD)				
//#define LED1_GPIO_PINS  (GPIO_PIN_5)
//#define LED2_GPIO_PORT  (GPIOD)
//#define LED2_GPIO_PINS  (GPIO_PIN_4)

//#define GREEN_LED_H() GPIO_WriteHigh(LED1_GPIO_PORT,(GPIO_Pin_TypeDef)LED1_GPIO_PINS)
//#define GREEN_LED_L() GPIO_WriteLow(LED1_GPIO_PORT, (GPIO_Pin_TypeDef)LED1_GPIO_PINS)
//#define RED_LED_H()   GPIO_WriteHigh(LED2_GPIO_PORT,(GPIO_Pin_TypeDef)LED2_GPIO_PINS)
//#define RED_LED_L()   GPIO_WriteLow(LED2_GPIO_PORT, (GPIO_Pin_TypeDef)LED2_GPIO_PINS)

#define GREEN_LED_H()
#define GREEN_LED_L()
#define RED_LED_H()
#define RED_LED_L()

//extern void LED_Init(void);
#endif