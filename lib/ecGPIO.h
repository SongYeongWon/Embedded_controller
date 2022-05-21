// Distributed for LAB: GPIO

#include "stm32f411xe.h"
#include "ecRCC.h"

#ifndef __ECGPIO_H
#define __ECGPIO_H

#define LOW  					0
#define HIGH 					1

#define RESET 3UL
	
//MODER
#define INPUT 				0
#define OUTPUT  			1
#define ALTERNATE 		2
#define ANALOG  			3

//OSPEED
#define LOW_SPEED 		0
#define MEDIUM_SPEED  1
#define FAST_SPEED 		2
#define HIGH_SPEED  	3

//PUPDR
#define NOPUPD 				0
#define PU  					1 //pull-up
#define PD 						2 //pull-down
#define RESERVED  		3


//OTYPE

#define OUTPUT_PP			0
#define OUTPUT_OD 		1

//7-Segment LED PINs
#define LED_PA5   		5
#define LED_PA6    		6
#define LED_PA7    		7
#define LED_PB6    		6
#define LED_PC7    		7
#define LED_PA9    		9	
#define LED_PA8    		8
#define LED_PB10   		10
#define BUTTON_PIN 		13

//PWM
#define PA1   		1

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void setup(void);
void GPIO_init(GPIO_TypeDef *Port, int pin, uint32_t mode);
void GPIO_mode(GPIO_TypeDef* Port, int pin, uint32_t mode);

void GPIO_write(GPIO_TypeDef *Port, int pin, uint32_t Output);
uint32_t  GPIO_read(GPIO_TypeDef *Port, int pin);
void GPIO_ospeed(GPIO_TypeDef* Port, int pin, uint32_t speed);
void GPIO_otype(GPIO_TypeDef* Port, int pin, uint32_t type);
void GPIO_pupdr(GPIO_TypeDef* Port, int pin, uint32_t pudr);


uint32_t ButtonST(uint32_t value);
void sevensegment_init(void);
void sevensegment_decoder(uint32_t num);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
