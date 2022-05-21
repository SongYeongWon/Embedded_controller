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
PWM_t pwm;
void EXTI15_10_IRQHandler(void);

//Initialization
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	GPIO_init(GPIOA, 5, OUTPUT);    // calls RCC_GPIOA_enable()
	GPIO_init(GPIOB, 3, OUTPUT);    // calls RCC_GPIOA_enable()	
	GPIO_init(GPIOB, 10, OUTPUT);    // calls RCC_GPIOA_enable()

	TIM_init(TIM2, 20);
	PWM_init(&pwm,GPIOA,PA1);
	PWM_period_ms(&pwm,20);
}

int main (void){
	setup();	
	PWM_duty(&pwm,0.5/20);
	while(1){
		if(EX == 1){
			count ++;
			PWM_duty(&pwm,(0.5+(count*0.2))/20);
			delay_ms(50);
			if(count==10){
				count =0;
				delay_ms(1000);
				PWM_duty(&pwm,(0.5+(count*0.2))/20);
			}
			EX = 0;		
			TIM_INT_CCIE_enable(TIM1);
		}
	}
}
void TIM2_IRQHandler(void);
void TIM2_IRQHandler(void){
	if(is_UIF(TIM1)){// update interrupt flag
		EX=1;
		// clear by writing 0
	}
}
