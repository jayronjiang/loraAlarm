#include "timer1.h"

u16 SysTime;
u8 time_flag;
u16 time2_count;


void TIMER1_Init(void)
{
	TIM1->ARRH   = 0x03;
	TIM1->ARRL   = 0xe8;           /* Freq control register: ARR �Զ���װ1ms */
	TIM1->PSCRH  = 0x00;           /* Configure TIM1 prescaler_H       */
	TIM1->PSCRL  = 0x0f;           /* Configure TIM1 prescaler_L   16��Ƶ     */
	TIM1->IER   |= 0x01;						//enable �����ж�
	TIM1->CR1   |= 0x80;
	
	TIM2->ARRH   = 0x03;
	TIM2->ARRL   = 0xe8;           /* Freq control register: ARR    �Զ���װ1ms */
	TIM2->PSCR   = 0x04;           /* Configure TIM1 prescaler_L   16��Ƶ*/
	TIM2->IER   |= 0x01;					/*enable �����ж� */
	TIM2->CR1   |= 0x80;
}