/**
******************************************************************************
* @author  Name : Song Yeong Won
* @Mod		 2021-10-29 by YWSONG  	
* @brief   Embedded Controller:  LAB : PWM Out - Servo Motor
*					
* 
******************************************************************************
*/


#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecSysTick.h"
#include "ecRCC.h"
#include "ecEXTI.h"
#include "ecTIM.h"
#include "ecPWM.h"

static uint32_t count= 0;
static volatile uint32_t PLL_CLK = 84000000;
PWM_t pwm1;
PWM_t pwm2;
PWM_t pwm3;
void EXTI15_10_IRQHandler(void);
void TIM2_IRQHandler(void);
void LED_toggle(GPIO_TypeDef *Port, uint32_t pin);

//Initialization
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	
	/*GPIO_init(GPIOA, 5, OUTPUT );  
	GPIO_ospeed(GPIOA, 5,HIGH_SPEED);  
	GPIO_otype(GPIOA, 5, OUTPUT_PP);
	GPIO_pupdr(GPIOA, 5, NOPUPD);	
	
	GPIO_init(GPIOB, 10, OUTPUT );  
	GPIO_ospeed(GPIOB, 10,HIGH_SPEED);  
	GPIO_otype(GPIOB, 10, OUTPUT_PP);
	GPIO_pupdr(GPIOB, 10, NOPUPD);  
	
	GPIO_init(GPIOB, 3, OUTPUT );  
	GPIO_ospeed(GPIOB, 3,HIGH_SPEED); 
	GPIO_otype(GPIOB, 3, OUTPUT_PP);
	GPIO_pupdr(GPIOB, 3, NOPUPD);*/


	//TIM_init(TIM2, 20);
	//TIM_INT_init(TIM2,1000); //period is 1ms
	//TIM_INT_enable(TIM2);
	/*TIM_INT_CCIE_enable(TIM2);
	TIM2->CCR1 =1000;
	TIM2->CCR2 =4000;	
	TIM2->CCR3 =5000;
	TIM2->CCR4 =10000;*/
	
	//TIM_INT_enable(TIM2);
	
	PWM_init(&pwm1,GPIOA,5);
	PWM_init(&pwm2,GPIOA,1);
	PWM_init(&pwm3,GPIOB,10);
	
	TIM_init(TIM2,1000);
	
	PWM_pulsewidth_ms(&pwm1,500);
	PWM_pulsewidth_ms(&pwm2,500);
	PWM_pulsewidth_ms(&pwm3,500);
	
}

int main (void){
	setup();	

	while(1){
	/*if(EX==1){
		GPIO_write(GPIOA,5,HIGH);
		GPIO_write(GPIOB,10,LOW);
		GPIO_write(GPIOB,3,LOW);
		EX=0;
	}else if(EX==2){
		GPIO_write(GPIOA,5,LOW);
		GPIO_write(GPIOB,10,HIGH);
		GPIO_write(GPIOB,3,LOW);

		EX=0;
	}else if(EX==3){
		GPIO_write(GPIOA,5,LOW);
		GPIO_write(GPIOB,10,LOW);
		GPIO_write(GPIOB,3,HIGH);
		EX=0;
	}*/
	}
}
/*
void TIM2_IRQHandler(void){
	if(is_CCIF(TIM2)){// update interrupt flag
		count ++;
		if(count==2){
			EX=1;
		}
		else if(count ==3){
			EX=2;
		}
		else if(count ==4){
			EX=3;
		}
		else if(count ==5){
			EX=4;
			count=0;
		}
		//TIM2->SR &= ~(15UL<<1);
		clear_CCIF(TIM2);
	}
}*/

/*
void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){// update interrupt flag
		count++;
		if (count >1000){
			LED_toggle(GPIOA,5);
			count=0;}
		//EX=1;
		clear_UIF(TIM2);// clear by writing 0
}
}*/

void LED_toggle(GPIO_TypeDef *Port, uint32_t pin){
	
	Port->ODR ^= (1<<pin);
}

