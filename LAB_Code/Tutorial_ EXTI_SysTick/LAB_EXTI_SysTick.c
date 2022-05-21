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
#include "ecSysTick.h"


//Initialization
static volatile uint32_t count = 0;
static volatile uint32_t PLL_CLK = 84000000;
void EXTI15_10_IRQHandler(void);

void setup(void)
{
	RCC_PLL_init();
	SysTick_init(PLL_CLK);
	sevensegment_init();
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);
	EXTI_init(GPIOC,BUTTON_PIN,FALL,0);
	GPIO_pupdr(GPIOC, BUTTON_PIN, PU);
}


int main (void){
	
	setup();	
	
	while(1){
		sevensegment_decoder(count);
		delay_ms(1000);
		count++;
		if(EX == 1){
			delay_ms(1000);
			EX=0;
			count = 1;
		}
		if(count == 10)count =0;
	}
}

void EXTI15_10_IRQHandler(void){
   if(is_pending_EXTI(BUTTON_PIN)){
		 SysTick_reset();
		 sevensegment_decoder(0);
		 EX = 1;
		 clear_pending_EXTI(BUTTON_PIN);
   }
}


