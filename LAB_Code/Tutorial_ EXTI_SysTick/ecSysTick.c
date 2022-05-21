#include "ecSysTick.h"

void SysTick_init(uint32_t msec){
	SysTick->CTRL &= 0<<0;      //Disable SysTick        
	SysTick->LOAD = msec/1000 -1;          //Set Reload register
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
void delay_ms(uint32_t msec){
	TimeDelay = msec;
	while(TimeDelay !=0);	
}

