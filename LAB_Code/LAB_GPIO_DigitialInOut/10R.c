/**
******************************************************************************
* @author  Name : Song Yeong Won
* @Mod		 2021-10-06 by YWSONG  	
* @brief   Embedded Controller:  LAB Digital In/Out
*					 - Toggle LED LD2 by Button B1  pressing
* 
******************************************************************************
*/

#include "ecRCC.h"
#include "ecSysTick.h"
#include "ecGPIO.h"
#include "stm32f411xe.h"

#define LED_PIN 	5
#define BUTTON_PIN 13

void setup(void);
void EXTI15_10_IRQHandler(void);
void LED_toggle(GPIO_TypeDef *port, uint32_t pin);

int main(void) { 
	// System CLOCK, GPIO Initialiization ----------------------------------------
	setup();
	
	
	// EXTI Initialiization ------------------------------------------------------	
	
	// SYSCFG peripheral clock enable
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;	  	
	
	// Connect External Line to the GPIO
	// Button: PC_13 -> EXTICR3(EXTI13)
	SYSCFG->EXTICR[BUTTON_PIN/4] &= ~SYSCFG_EXTICR4_EXTI13;     
	SYSCFG->EXTICR[BUTTON_PIN/4] |=  SYSCFG_EXTICR4_EXTI13_PC;  

	// Falling trigger enable (Button: pull-up)
	EXTI->FTSR |= 1UL << BUTTON_PIN;		    	

	// Unmask (Enable) EXT interrupt
	EXTI->IMR  |= 1UL << BUTTON_PIN;		    	
		
	// Interrupt IRQn, Priority
	NVIC_SetPriority(EXTI15_10_IRQn, 1);  		// Set EXTI priority as 0	
	NVIC_EnableIRQ(EXTI15_10_IRQn); 					// Enable EXTI 
		
	
	while(1);
}
void EXTI15_10_IRQHandler(void) {  
	if ((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13) {
		LED_toggle(GPIOA,LED_PIN);
		EXTI->PR |= EXTI_PR_PR13; // cleared by writing '1'
	}
}

void LED_toggle(GPIO_TypeDef *Port, uint32_t pin){
	
	Port->ODR ^= (1<<pin);
	
	
}
// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	// Initialize GPIOA_5 for Output
	GPIO_init(GPIOA, LED_PIN, OUTPUT);    // calls RCC_GPIOA_enable()	
	// Initialize GPIOC_13 for Input Button
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);  // calls RCC_GPIOC_enable()
}