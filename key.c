#include "stm8s.h"
#include "key.h"

u8 operation_flag;
u8 key_flag;
u16 key_count;

void KEY_Init(void)
{
	GPIO_Init(KEY1_GPIO_PORT, (GPIO_Pin_TypeDef)KEY1_GPIO_PINS, GPIO_MODE_IN_PU_IT);
	EXTI->CR1 |= 0x0C;
}




































