/**
******************************************************************************
* @author  Name : Song Yeong Won
* @Mod		 2021-10-22 by YWSONG  	
* @brief   Embedded Controller:  LAB SysTick and External Interrupt
*					
* 
******************************************************************************
*/


#include "ecHAL.h"

//Initialization
static volatile uint32_t count = 0;
static volatile uint32_t PLL_CLK = 84000000;
void EXTI9_5_IRQHandler(void);

void setup(void)
{
	RCC_PLL_init();
	SysTick_init(PLL_CLK);
	sevensegment_init();
	GPIO_init(GPIOB, 8, INPUT);
	EXTI_init(GPIOB,8,FALL,0);
	GPIO_pupdr(GPIOB, 8, PU);
}


int main (void){
	
	setup();	
	printf("IR1\r\n");
	while(1){
		sevensegment_decoder(count);
		delay_ms(1000);
		count++;
		if(EX == 1){
			printf("IR1 = %d\r\n",1);	
			//delay_ms(1000);
			EX=0;
			count = 1;
		}
		if(count == 10)count =0;
	}
}

void EXTI9_5_IRQHandler(void){
   if(is_pending_EXTI(8)){
		 SysTick_reset();
		 sevensegment_decoder(0);
		 EX = 1;
		 clear_pending_EXTI(8);
   }
}


