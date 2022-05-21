/**
******************************************************************************
* @author  SONG YEONG WON
* @Mod		 2021-11-03 by YWSONG  	
* @brief   Embedded Controller: Stepper Motor
* 
******************************************************************************
*/
#include "ecUART.h"
#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecEXTI.h"
#include "ecSysTick.h"
#include "ecStepper.h"

void setup(void);
void TIM2_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
static int count= 0;
static int Start= 0;
static int Stop= 0;
static int sec =0;

#define EC_DIN 13

int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	Start=count;
	Stepper_step(2048*2, 1, HALF);  // (Full Step : 2048, Half Step : 4096 Direction : 0 -> ++ or 1 -> --, Mode : FULL or HALF)
	Stop=count;
	
	sec = Stop - Start;
	printf("Stepper motor seconds %d ms\r\n", sec);
	// Inifinite Loop ----------------------------------------------------------
	while(1){;}
}


// Initialiization 
void setup(void)
{	                                 // System Clock = 84MHz
	RCC_PLL_init();
	UART2_init();
	SysTick_init(1);                                 // Systick init usec

	TIM_INT_init(TIM2,1);														// 1ms pulse
	
	EXTI_init(GPIOC,BUTTON_PIN,FALL,0);             // External Interrupt Setting
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);           // GPIOC pin13 initialization

	Stepper_init(GPIOB,10,GPIOB,4,GPIOB,5,GPIOB,3); // Stepper GPIO pin initialization
	Stepper_setSpeed(15,HALF);                          //  set stepper motor speed
}


void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){// update interrupt flag
		count++;
		}
		clear_UIF(TIM2);// clear by writing 0
}

void EXTI15_10_IRQHandler(void) {  
	if (is_pending_EXTI(BUTTON_PIN)) {
		Stepper_stop();
		clear_pending_EXTI(BUTTON_PIN); // cleared by writing '1'
	}
}
