#ifndef __EC_EXTI_H
#define __EC_EXTI_H
#include "ecSysTick.h"
#include "stm32f411xe.h"


//EXTI interrupt
#define PA_PIN 0
#define PB_PIN 1
#define PC_PIN 2
#define PD_PIN 3
#define PE_PIN 4
#define PH_PIN 7

#define FALL 0
#define RISE 1
#define BOTH 2

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void EXTI_init(GPIO_TypeDef *Port, uint32_t pin, int trig,int priority);
void EXTI_clearpending(uint32_t pin);
void EXTI_enable(uint32_t pin);
void EXTI_disable(uint32_t pin);
uint32_t  is_pending_EXTI(uint32_t pin);
void clear_pending_EXTI(uint32_t pin);

#ifdef __cplusplus
}
#endif /* __cplusplus */
	 
#endif