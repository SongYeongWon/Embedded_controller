/**
******************************************************************************
* @author  Name : Song Yeong Won
* @Mod		 2021-10-06 by YWSONG  	
* @brief   Embedded Controller:  LAB Digital In/Out
*					 - Toggle LED LD2 by Button B1  pressing
* 
******************************************************************************
*/


#include "stm32f4xx.h"
#include "ecRCC.h"
#include "ecGPIO.h"

#define LED_PA5 	5
#define LED_PA6		6
#define LED_PA7 	7
#define LED_PB6 	6
#define BUTTON_PIN 13


uint32_t ButtonST(uint32_t value){
	uint32_t next =0;
	if(value == 0){
		next =1;
	}
	if(value ==1){
		next =2;
	}
	if(value ==2){
		next =3;
	}
	if(value ==3){
		next =4;
	}
	if(value ==4){	
		next =1;
	}
	return next;
}

void setup(void);
int main(void) { 
	// Initialiization --------------------------------------------------------
    setup();
	uint32_t current_state = 0;	// 0=released ,1=pressed // 
	uint32_t prev_state = 1;  		// 0=pressed	,1=released // button  	
	uint32_t button_nextstate = 0; //button pressed state
	uint32_t LED_Control = 0; //button pressed state
	// Inifinite Loop ----------------------------------------------------------
	while(1){

		/*if(GPIO_read(GPIOC, BUTTON_PIN) == 1){
				current_state =0;
		}*/
		if(GPIO_read(GPIOC, BUTTON_PIN) == 0){
			prev_state =0;
			current_state =1;
			LED_Control =1;
		}else current_state = 0;
		if(current_state == 0 &&  prev_state == 0){
			if(LED_Control == 1){
				button_nextstate = ButtonST(button_nextstate); //check button is pressed and go to next state
			}	
			if(button_nextstate ==1){
			GPIO_write(GPIOA, LED_PA5, HIGH);
			GPIO_write(GPIOA, LED_PA6, LOW);
			GPIO_write(GPIOA, LED_PA7, LOW);
			GPIO_write(GPIOB, LED_PB6, LOW);
			prev_state = 1;
			LED_Control = 0;
			}
			if(button_nextstate ==2){	
			GPIO_write(GPIOA, LED_PA5, LOW);
			GPIO_write(GPIOA, LED_PA6, HIGH);
			GPIO_write(GPIOA, LED_PA7, LOW);
			GPIO_write(GPIOB, LED_PB6, LOW);
			prev_state = 1;
			LED_Control = 0;	
			}
			if(button_nextstate ==3){
			GPIO_write(GPIOA, LED_PA5, LOW);
			GPIO_write(GPIOA, LED_PA6, LOW);
			GPIO_write(GPIOA, LED_PA7, HIGH);
			GPIO_write(GPIOB, LED_PA6, LOW);
			prev_state = 1;
			LED_Control = 0;
			}
			if(button_nextstate ==4){
			GPIO_write(GPIOA, LED_PA5, LOW);
			GPIO_write(GPIOA, LED_PA6, LOW);
			GPIO_write(GPIOA, LED_PA7, LOW);
			GPIO_write(GPIOB, LED_PB6, HIGH);
			prev_state = 1;
			LED_Control = 0;	
			}	
		}
	}
}


// Initialiization 
void setup(void)
{
	RCC_HSI_init();	
	GPIO_init(GPIOA, LED_PA5, OUTPUT);    // calls RCC_GPIOA_enable()		PA5
	GPIO_init(GPIOA, LED_PA6, OUTPUT);    // calls RCC_GPIOA_enable()		PA6
	GPIO_init(GPIOA, LED_PA7, OUTPUT);    // calls RCC_GPIOA_enable()		PA7
	GPIO_init(GPIOB, LED_PB6, OUTPUT);    // calls RCC_GPIOB_enable()		PB6
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);  // calls RCC_GPIOC_enable()		
	
	
	//GPIOA PA5
	GPIO_mode(GPIOA, LED_PA5, OUTPUT);
	GPIO_ospeed(GPIOA, LED_PA5, MEDIUM_SPEED);
	GPIO_otype(GPIOA, LED_PA5, OUTPUT_PP);
	GPIO_pudr(GPIOA, LED_PA5, PU);

	//GPIOA PA6
	GPIO_mode(GPIOA, LED_PA6, OUTPUT);
	GPIO_ospeed(GPIOA, LED_PA6, MEDIUM_SPEED);
	GPIO_otype(GPIOA, LED_PA6, OUTPUT_PP);
	GPIO_pudr(GPIOA, LED_PA6, PU);
	
	//GPIOA PA7
	GPIO_mode(GPIOA, LED_PA7, OUTPUT);
	GPIO_ospeed(GPIOA, LED_PA7, MEDIUM_SPEED);
	GPIO_otype(GPIOA, LED_PA7, OUTPUT_PP);
	GPIO_pudr(GPIOA, LED_PA7, PU);
	
	//GPIOB PB6
	GPIO_mode(GPIOB, LED_PB6, OUTPUT);
	GPIO_ospeed(GPIOB, LED_PB6, MEDIUM_SPEED);
	GPIO_otype(GPIOB, LED_PB6, OUTPUT_PP);
	GPIO_pudr(GPIOB, LED_PB6, PU);
	
	//GPIOC
	GPIO_mode(GPIOC, BUTTON_PIN, INPUT);
	GPIO_pudr(GPIOC, BUTTON_PIN , PU);
}

