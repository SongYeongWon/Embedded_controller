/**
******************************************************************************
* @author  Song Yeong Won 	
* @brief   Embedded Controller:  TEST1_Q2
* 
******************************************************************************
*/
#include "stm32f411xe.h"
#include "ecRCC.h"
#include "ecGPIO.h"
#include "ecEXTI.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecSysTick.h"


void setup(void);
void EXTI9_5_IRQHandler(void);
static uint32_t duty= 0;
PWM_t pwm1;
PWM_t pwm2;
// YOUR CODE

int main(void){

  setup();
	
	while(1){

		
	//PWM_pulsewidth_ms(&pwm2,900);
		if(EX == 1){

			PWM_pulsewidth_ms(&pwm2,0.5*duty);
			duty ++;
			if(duty ==3){
				duty=0;	
		}
			EX=0;	
		}
	}
}

void setup(void){	
	
	RCC_PLL_init();
	//RCC_HSI_init(); 
	SysTick_init(1);
	
	EXTI_init(GPIOB, 5, FALL, 0);
  //GPIO_init(GPIOB, 5, INPUT);
	
	//EXTI_init(GPIOC, BUTTON_PIN, RISE, 0);
  //GPIO_init(GPIOC, BUTTON_PIN, INPUT);
  //GPIO_pupdr(GPIOC, BUTTON_PIN, PD);
	
	PWM_init(&pwm1,GPIOA,6);
	PWM_init(&pwm2,GPIOB,3);
	
	TIM_init(TIM3,1000);
	PWM_pulsewidth_ms(&pwm1,500);
	
	TIM_init(TIM2,1);
	PWM_pulsewidth_ms(&pwm2,0.5);
  
}

void EXTI9_5_IRQHandler(void){
   if(is_pending_EXTI(5)){
		 EX=1;
     clear_pending_EXTI(5);
	 }
}

