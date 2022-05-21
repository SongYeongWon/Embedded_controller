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
#include "EC_GPIO_API_student.h"

#define LED_PIN 	5
#define BUTTON_PIN 13

EC_DigitalIn button(GPIOC,BUTTON_PIN);
EC_DigitalOut led(GPIOA,LED_PIN);

	
int main(void) { 
	while(1){
		if(!button)	led=1;			//if(!button.read())
		else 				led=0;
	}
}
