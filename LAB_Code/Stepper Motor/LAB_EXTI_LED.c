/**
******************************************************************************
* @author  Name : Song Yeong Won
* @Mod		 2021-10-22 by YWSONG  	
* @brief   Embedded Controller:  LAB SysTick and External Interrupt
*					
* 
******************************************************************************
*/

#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "exEXTI.h"

#define LED_PIN	5

void setup(void);
void LED_toggle(GPIO_TypeDef *Port, uint32_t pin);
void EXTI15_10_IRQHandler(void);

int main(void){
   setup();
   
   while(1){}
}
void setup(void){
   RCC_PLL_init();
   GPIO_init(GPIOA, LED_PIN, OUTPUT);
   EXTI_init(GPIOC, BUTTON_PIN, FALL, 0);
   GPIO_init(GPIOC, BUTTON_PIN, INPUT);
   GPIO_pudr(GPIOC, BUTTON_PIN, PD);
}

void LED_toggle(GPIO_TypeDef *Port, uint32_t pin){
	
	Port->ODR ^= (1<<pin);
}
	
void EXTI15_10_IRQHandler(void){
   if(is_pending_EXTI(BUTTON_PIN)){
      LED_toggle(GPIOA,LED_PIN);
      clear_pending_EXTI(BUTTON_PIN);
   }
}





