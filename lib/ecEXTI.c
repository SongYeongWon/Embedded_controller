#include "ecEXTI.h"


void EXTI_init(GPIO_TypeDef *port, uint32_t pin, int trig_type, int priority){
	//RCC->APB2ENR |= 1UL << 14;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 
	//EXTIx connection GPIO Pinx
	SYSCFG-> EXTICR[pin/4] &=~ 15UL<<4*(pin%4);
		
	if(port == GPIOA){
	SYSCFG-> EXTICR[pin/4] = PA_PIN<<4*(pin%4);
	}
	if(port == GPIOB){
	SYSCFG-> EXTICR[pin/4] = PB_PIN<<4*(pin%4);
	}
	if(port == GPIOC){
	SYSCFG-> EXTICR[pin/4] = PC_PIN<<4*(pin%4);
	}
	if(port == GPIOD){
	SYSCFG-> EXTICR[pin/4] = PD_PIN<<4*(pin%4);
	}
	if(port == GPIOE){
	SYSCFG-> EXTICR[pin/4] = PE_PIN<<4*(pin%4);
	}
	if(port == GPIOH){
	SYSCFG-> EXTICR[pin/4] = PH_PIN<<4*(pin%4);
	}
	
	//triger type (Falling, rising, both)
	if(trig_type == FALL_EXTI){
		EXTI-> FTSR |= 1<<pin;                
	}
	if(trig_type == RISE_EXTI){
		EXTI-> RTSR |= 1<<pin;
	}
	if(trig_type == BOTH_EXTI){
		EXTI-> FTSR |= 1<<pin; 
		EXTI-> RTSR |= 1<<pin;
	}
	
	EXTI_enable(pin);
	
	// priority
	uint32_t IRQN =0;
	if(pin<=4){
	IRQN = pin+6;
	}
	else if(pin<=9){
	IRQN = 23;
	}
	else if(pin<=15)
	{
	IRQN = 40;
	}
  NVIC_SetPriority(IRQN, priority);  // Set EXTI priority as 0   
  NVIC_EnableIRQ(IRQN);          // Enable EXTI 
}

void EXTI_enable(uint32_t pin){ // mask in IMR
	EXTI -> IMR |= 1<<pin;
	uint32_t EN = 0;
	if(pin<=4){
		EN = pin +6;
	NVIC_EnableIRQ(EN);	
	}
	else if(pin<=9){
		EN = 23;
	NVIC_EnableIRQ(EN);	
	}
	else if(pin<=15){
		EN = 40;
	NVIC_EnableIRQ(EN);	
	}
}

void EXTI_disable(uint32_t pin){// unmask in IMR
	EXTI -> IMR &= 0UL<<pin;  
}	


uint32_t is_pending_EXTI(uint32_t pin){
	
	return (EXTI->PR & 1UL<<pin) == (1<<pin);
}

void clear_pending_EXTI(uint32_t pin){

	EXTI->PR |= 1UL<<pin;  
}

