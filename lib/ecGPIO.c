// Distributed for LAB: GPIO
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecGPIO.h"

void GPIO_init(GPIO_TypeDef *Port, int pin, uint32_t mode){
	if (Port == GPIOA)
		RCC_GPIOA_enable();
	
	if (Port == GPIOB)
		RCC_GPIOB_enable();

	if (Port == GPIOC)
		RCC_GPIOC_enable();

	GPIO_mode(Port, pin, mode);
}

void GPIO_mode(GPIO_TypeDef *Port, int pin, uint32_t mode){
		Port->MODER  &= ~(RESET<<( pin *2));        
		Port->MODER  |= mode<<(pin *2);
	
}

void GPIO_write(GPIO_TypeDef *Port, int pin, uint32_t Output){
	if(Output == 0) //Output Low
	{ 
	Port->ODR &= ~(1<<pin);
	}
	if(Output == 1) //Output High
	{
	Port->ODR |= 1<<pin ;
	}
}

uint32_t  GPIO_read(GPIO_TypeDef *Port, int pin){
	
	return (Port->IDR) & (1<<pin) ;
}


void GPIO_ospeed(GPIO_TypeDef* Port, int pin, uint32_t speed){
		Port->OSPEEDR  &= ~(RESET<<( pin *2));        
		Port->OSPEEDR  |= speed<<(pin *2);
}

void GPIO_otype(GPIO_TypeDef* Port, int pin, uint32_t type){ // 0 : Output push-pull 1: Output open-drain 
	if(type == 0) //Output push-pull
	{ 
	Port->OTYPER &= ~(1<<pin);
	}
	if(type == 1) //Output Open-drain
	{
	Port->OTYPER |= 1<<pin ;
	}
}

void GPIO_pupdr(GPIO_TypeDef* Port, int pin, uint32_t pudr){
		Port->PUPDR  &= ~(RESET<<( pin *2));        
		Port->PUPDR  |= pudr<<(pin *2);
}


void sevensegment_decoder(uint32_t num){

	int PIN[8]={LED_PA5,LED_PA6,LED_PA7,LED_PB6,LED_PC7,LED_PA9,LED_PA8,LED_PB10};
	//LED ON : 0 , LED OFF : 1
	uint32_t number[11][8]={
               		  {0,0,0,0,0,0,1,1},          //zero
                    {1,0,0,1,1,1,1,1},          //one
                    {0,0,1,0,0,1,0,1},          //two
                    {0,0,0,0,1,1,0,1},          //three
                    {1,0,0,1,1,0,0,1},          //four
                    {0,1,0,0,1,0,0,1},          //five
                    {0,1,0,0,0,0,0,1},          //six
                    {0,0,0,1,1,0,1,1},          //seven
                    {0,0,0,0,0,0,0,1},          //eight
                    {0,0,0,0,1,0,0,1},          //nine
										{1,1,1,1,1,1,1,1},          //no-display
                  };
	
	for (int i=0; i<8; i++){
				if(i==0 || i==1 || i==2|| i==5 || i==6){
				GPIO_write(GPIOA, PIN[i], number[num][i]);
				}
				if(i==3 || i==7){
				GPIO_write(GPIOB, PIN[i], number[num][i]);
				}
				if(i==4){
				GPIO_write(GPIOC, PIN[i], number[num][i]);
				}
			}
}

uint32_t ButtonST(uint32_t value){
	uint32_t next =0;
	if(value ==0) next =1; 
	if(value ==1)	next =2;
	if(value ==2) next =3;
	if(value ==3)	next =4;
	if(value ==4)	next =5;
	if(value ==5) next =6;
	if(value ==6)	next =7;
	if(value ==7)	next =8;
	if(value ==8)	next =9;
	if(value ==9)	next =0;
	return next;
}

void sevensegment_init(void)
{
	//RCC_HSI_init();
	//RCC_PLL_init();
	GPIO_init(GPIOA, LED_PA5, OUTPUT);    // calls RCC_GPIOA_enable()		PA5
	GPIO_init(GPIOA, LED_PA6, OUTPUT);    // calls RCC_GPIOA_enable()		PA6
	GPIO_init(GPIOA, LED_PA7, OUTPUT);    // calls RCC_GPIOA_enable()		PA7
	GPIO_init(GPIOB, LED_PB6, OUTPUT);    // calls RCC_GPIOB_enable()		PB6
	GPIO_init(GPIOC, LED_PC7, OUTPUT);    // calls RCC_GPIOC_enable()		PC7
	GPIO_init(GPIOA, LED_PA9, OUTPUT);    // calls RCC_GPIOA_enable()		PA9
	GPIO_init(GPIOA, LED_PA8, OUTPUT);    // calls RCC_GPIOA_enable()		PA8
	GPIO_init(GPIOB, LED_PB10, OUTPUT);   // calls RCC_GPIOB_enable()		PB10
	
	//GPIOA PA5																Sevensegment A
	GPIO_mode(GPIOA, LED_PA5, OUTPUT);
	GPIO_ospeed(GPIOA, LED_PA5, FAST_SPEED);
	GPIO_otype(GPIOA, LED_PA5, OUTPUT_PP);
	GPIO_pupdr(GPIOA, LED_PA5, NOPUPD);

	//GPIOA PA6																Sevensegment B
	GPIO_mode(GPIOA, LED_PA6, OUTPUT);
	GPIO_ospeed(GPIOA, LED_PA6, FAST_SPEED);
	GPIO_otype(GPIOA, LED_PA6, OUTPUT_PP);
	GPIO_pupdr(GPIOA, LED_PA6, NOPUPD);
	
	//GPIOA PA7																Sevensegment C
	GPIO_mode(GPIOA, LED_PA7, OUTPUT);
	GPIO_ospeed(GPIOA, LED_PA7, FAST_SPEED);
	GPIO_otype(GPIOA, LED_PA7, OUTPUT_PP);
	GPIO_pupdr(GPIOA, LED_PA7, NOPUPD);
	
	//GPIOB PB6																Sevensegment D
	GPIO_mode(GPIOB, LED_PB6, OUTPUT);
	GPIO_ospeed(GPIOB, LED_PB6, FAST_SPEED);
	GPIO_otype(GPIOB, LED_PB6, OUTPUT_PP);
	GPIO_pupdr(GPIOB, LED_PB6, NOPUPD);
	
	//GPIOC PC7																Sevensegment E
	GPIO_mode(GPIOC, LED_PC7, OUTPUT);
	GPIO_ospeed(GPIOC, LED_PC7, FAST_SPEED);
	GPIO_otype(GPIOC, LED_PC7, OUTPUT_PP);
	GPIO_pupdr(GPIOC, LED_PC7, NOPUPD);
	
  //GPIOA PA9 	 														Sevensegment F
	GPIO_mode(GPIOA, LED_PA9, OUTPUT);
	GPIO_ospeed(GPIOA, LED_PA9, FAST_SPEED);
	GPIO_otype(GPIOA, LED_PA9, OUTPUT_PP);
	GPIO_pupdr(GPIOA, LED_PA9, NOPUPD);

	//GPIOA PA8																Sevensegment G
	GPIO_mode(GPIOA, LED_PA8, OUTPUT);
	GPIO_ospeed(GPIOA, LED_PA8, FAST_SPEED);
	GPIO_otype(GPIOA, LED_PA8, OUTPUT_PP);
	GPIO_pupdr(GPIOA, LED_PA8, NOPUPD);
	
	//GPIOB PB10  														Sevensegment DOT
	GPIO_mode(GPIOB, LED_PB10, OUTPUT);
	GPIO_ospeed(GPIOB, LED_PB10, FAST_SPEED);
	GPIO_otype(GPIOB, LED_PB10, OUTPUT_PP);
	GPIO_pupdr(GPIOB, LED_PB10, NOPUPD);
}
