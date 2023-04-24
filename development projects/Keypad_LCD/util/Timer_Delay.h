

#ifndef TIMER_DELAY_H_
#define TIMER_DELAY_H_

#define HAL_TIM_MODULE_ENABLED

#include "stm32f4xx_hal.h"


void TimerDelay_Init(void);
void delay_us(uint16_t au16_us);
void delay_ms(uint16_t au16_ms);

#endif /* TIMER_DELAY_H_ */
