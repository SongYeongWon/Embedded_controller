/**
******************************************************************************
* @author  Song Yeong Won 	
* @brief   Embedded Controller:  TEST1_Q1
* 
******************************************************************************
*/

#include "stm32f411xe.h"
#include "ecRCC.h"
#include "ecGPIO.h"
#include "ecEXTI.h"
#include "ecTIM.h"
#include "ecSysTick.h"


void setup(void);
void EXTI15_10_IRQHandler(void);
void TIM2_IRQHandler(void);
void LED_toggle(GPIO_TypeDef *Port, uint32_t pin);

static uint32_t count= 0;
static	uint32_t button_press =0;

int main(void){

  setup();
	
	while(1){
		
		if(EX==0){
		GPIO_write(GPIOA,5,LOW);
		GPIO_write(GPIOB,6,LOW);
		}else if(EX==1){
			LED_toggle(GPIOA,5);
			delay_ms(500);
		}else if(EX==2){
			GPIO_write(GPIOA,5,LOW);
		}else if(EX==3){
			EX=0;
		}
	}
}

void setup(void){	
	
	RCC_PLL_init();
	SysTick_init(1);
	
  EXTI_init(GPIOC, BUTTON_PIN, RISE_EXTI, 0);
  GPIO_init(GPIOC, BUTTON_PIN, INPUT);
  GPIO_pupdr(GPIOC, BUTTON_PIN, PD);
	
	GPIO_init(GPIOA, 5, OUTPUT );  
	GPIO_ospeed(GPIOA, 5,MEDIUM_SPEED);  
	GPIO_otype(GPIOA, 5, OUTPUT_PP);
	GPIO_pupdr(GPIOA, 5, NOPUPD);	
	
	GPIO_init(GPIOB, 6, OUTPUT );  
	GPIO_ospeed(GPIOB, 6,MEDIUM_SPEED);  
	GPIO_otype(GPIOB, 6, OUTPUT_PP);
	GPIO_pupdr(GPIOB, 6, NOPUPD);
	
	
	TIM_INT_init(TIM2,1); //period is 1ms
	
}


void EXTI15_10_IRQHandler(void){
   if(is_pending_EXTI(BUTTON_PIN)){
      button_press++;
		 if(button_press==1){
				EX=1;
		 }else if(button_press==2){
			 EX=2;
		 }else if(button_press==3){
			 EX=3;
			 button_press = 0;
		 }
      clear_pending_EXTI(BUTTON_PIN);
   }
}


void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){// update interrupt flag
		count++;
		if (count >500){
			if(EX==2){
			LED_toggle(GPIOB,6);
			}
			count=0;
		}
		clear_UIF(TIM2);// clear by writing 0
}
}

void LED_toggle(GPIO_TypeDef *Port, uint32_t pin){
	
	Port->ODR ^= (1<<pin);
}

		

