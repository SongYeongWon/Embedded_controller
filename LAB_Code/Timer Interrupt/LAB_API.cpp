/**
******************************************************************************
* @author  Name : Song Yeong Won
* @Mod		 2021-10-22 by YWSONG  	
* @brief   Embedded Controller:  LAB SysTick and External Interrupt
*					
* 
******************************************************************************
*/

#include "EC_API.h"
EC_Ticker tick(1);
volatile uint32_t count = 0;

//Initialization	
void setup(void)
{
	RCC_PLL_init();
	sevensegment_init();
}

int main (void){
	setup();	
	while(1){
		sevensegment_decoder(count);
		tick.Delay_ms(1000);
		count++;
		if(count == 10)count =0;
		tick.reset();
	}
}

