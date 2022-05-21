#include "ecSysTick.h"

void SysTick_init(uint32_t msec){
	SysTick->CTRL &= 0<<0;      //Disable SysTick        
	SysTick->LOAD = 84000000*0.001*msec -1;          //Set Reload register 84,000,000*(msec/1000) -1 = LOAD 
	SysTick->VAL = 0; 		 //Reset the SysTick counter value
	SysTick->CTRL |= 1<<2; //Select processor clock 
	SysTick->CTRL |= 1<<1; //Enable SysTick interrupt
	SysTick->CTRL |= 1<<0; //Enable SysTick
	NVIC_SetPriority(SysTick_IRQn,3); //Set interrupt priority
}

uint32_t SysTick_val(void){	
	return SysTick->VAL &= 0xffffff;
}
void SysTick_reset (void){
	TimeDelay = 0;
	SysTick->VAL = 0;
}
void SysTick_enable(void){
	SysTick->CTRL |= 1<<0;
}
void SysTick_disable (void){
	SysTick->CTRL &= 0<<0;
}
void SysTick_Handler(void){
	  if(TimeDelay >0){
		TimeDelay --;
		}
}
void delay_ms(uint32_t msec){  //1[ms] -> period is 1/84,000 so repeat it 1000 . and then total 1[s] so we count value 1000 to 1 decrease. 
	TimeDelay = msec;
	while(TimeDelay !=0);	
}

