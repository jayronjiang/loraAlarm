#include "timer1.h"

u16 SysTime;
u8 time_flag;
u16 time2_count;


void TIMER1_Init(void)
{
	TIM1->ARRH   = 0x03;
	TIM1->ARRL   = 0xe8;           /* Freq control register: ARR 自动重装1ms */
	TIM1->PSCRH  = 0x00;           /* Configure TIM1 prescaler_H       */
	TIM1->PSCRL  = 0x0f;           /* Configure TIM1 prescaler_L   16分频     */
	TIM1->IER   |= 0x01;						//enable 更新中断
	TIM1->CR1   |= 0x80;
	
	TIM2->ARRH   = 0x03;
	TIM2->ARRL   = 0xe8;           /* Freq control register: ARR    自动重装1ms */
	TIM2->PSCR   = 0x04;           /* Configure TIM1 prescaler_L   16分频*/
	TIM2->IER   |= 0x01;					/*enable 更新中断 */
	TIM2->CR1   |= 0x80;
}