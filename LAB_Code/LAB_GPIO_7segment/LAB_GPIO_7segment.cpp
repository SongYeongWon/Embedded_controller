/**
******************************************************************************
* @author  Name : Song Yeong Won
* @Mod		 2021-10-15 by YWSONG  	
* @brief   Embedded Controller:  LAB Digital In/Out
*					 - 7-Segment Display
* 
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecRCC.h"
#include "ecGPIO.h"

int main(void) { 
	// Initialiization --------------------------------------------------------
  setup();
	sevensegment_init();
	
	uint32_t current_state = 0;	// 0=released ,1=pressed // 
	uint32_t prev_state = 1;  		// 0=pressed	,1=released // button  	
	uint32_t button_nextstate = 0; //button pressed state
	uint32_t LED_Control = 0; //button pressed state
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		sevensegment_decoder(button_nextstate);
		if(GPIO_read(GPIOC, BUTTON_PIN) == 0){
			prev_state =0;
			current_state =1;
			LED_Control =1;
		}else current_state = 0;
		if(current_state == 0 &&  prev_state == 0){
			if(LED_Control == 1){
				button_nextstate = ButtonST(button_nextstate); //check button is pressed and go to next state
			}
			prev_state = 1;
			LED_Control = 0;
		}
	}
}

// Initialiization 
void setup(void)
{
	RCC_HSI_init();	
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);  // calls RCC_GPIOC_enable()		

	//GPIOC
	GPIO_mode(GPIOC, BUTTON_PIN, INPUT);
	GPIO_pupdr(GPIOC, BUTTON_PIN , PU);
}
