#ifndef __KEY_H__
#define __KEY_H__

#define KEY1_GPIO_PORT  (GPIOB)
#define KEY1_GPIO_PINS  (GPIO_PIN_5)
#define Get_KEY()		GPIO_ReadInputPin(KEY1_GPIO_PORT, (GPIO_Pin_TypeDef)KEY1_GPIO_PINS)


extern u8 key_flag;
extern u16 key_count;
extern u8	operation_flag;

extern void KEY_Init(void);

#endif