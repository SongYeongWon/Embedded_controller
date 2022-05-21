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
PWM_t pwm;
void EXTI15_10_IRQHandler(void);

//Initialization
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);

	GPIO_init(GPIOC, BUTTON_PIN, INPUT);
	EXTI_init(GPIOC,BUTTON_PIN,FALL,0);
	GPIO_pupdr(GPIOC, BUTTON_PIN, PU);
	
	TIM_init(TIM2, 20);
	PWM_init(&pwm,GPIOA,PA1);
	PWM_period_ms(&pwm,20);
}

int main (void){
	
	setup();	
	PWM_duty(&pwm,0.5/20);
	while(1){
		if(EX == 1){
			if(count <3){
				count ++;
			PWM_duty(&pwm,(1.5+(count*0.5))/20);
			delay_ms(50);
			}else count=2;
			EX = 0;
		}
	}
}

void EXTI15_10_IRQHandler(void){
   if(is_pending_EXTI(BUTTON_PIN)){		 
		 EX = 1;
		 clear_pending_EXTI(BUTTON_PIN);
   }
}
