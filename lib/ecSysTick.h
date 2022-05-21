#ifndef __EC_ECSYSTICK_H
#define __EC_ECSYSTICK_H

#include "stm32f411xe.h"
#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void SysTick_init(uint32_t msec);
void delay_ms(uint32_t msec);
uint32_t SysTick_val(void);
void SysTick_reset (void);
void SysTick_enable(void);
void SysTick_disable (void);
void SysTick_Handler(void);
static volatile uint32_t TimeDelay = 0;
static volatile uint32_t EX = 0;
static volatile uint32_t EX1 = 0;
#ifdef __cplusplus
}
#endif /* __cplusplus */
	 
#endif

