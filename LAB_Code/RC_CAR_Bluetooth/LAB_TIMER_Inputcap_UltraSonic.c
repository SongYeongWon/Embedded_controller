/**
******************************************************************************
* @author  SONG YEONG WON
* @Mod		 2021-11-12 by YWSONG  	
* @brief   Embedded Controller LAB: Input Capture - Ultrasonic Distance Sensor
* ******************************************************************************
*/


#include "stm32f411xe.h"
#include "math.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecUART.h"
#include "ecSysTick.h"

static int overflow = 0;
float distance = 0;
static float timeSt = 0;
static float timeEnd= 0;
static float sec = 0;


void TIM2_IRQHandler(void);
void setup(void);

int main(void){
	
	setup();
	
	
	while(1){
	  distance = (float) sec*340/(2*10000); // Ultrasonic speed[m/s] * echo pulse duration[us]
		printf("%f [cm]\r\n",distance);
		delay_ms(500);
	}
}

void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                     // Update interrupt
		overflow++;													// overflow count
		clear_UIF(TIM2);  							    // clear update interrupt flag
	}
	if(is_CCIF(TIM2,3)){ 									// TIM2_Ch3 (IC3) Capture Flag. Rising Edge Detect
			timeSt = TIM2->CCR3;								// Capture TimeStart from CC3
		clear_CCIF(TIM2,3);                 // clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM2,4)){ 						// TIM2_Ch3 (IC4) Capture Flag. Falling Edge Detect
			timeEnd = TIM2->CCR4;
			sec =((timeEnd-timeSt)+(overflow*(0xFFFF+1)))*10;
			overflow=0;
			clear_CCIF(TIM2,4);								  // clear capture/compare interrupt flag 
	}
}

void setup(){

	RCC_PLL_init(); 
	SysTick_init(1);
	UART2_init();
 
// PWM configuration ---------------------------------------------------------------------	
	PWM_t trig;											// PWM1 for trig
	PWM_init(&trig,GPIOA,6);			 	// PWM init as PA_6: Ultrasonic trig pulse
	PWM_period_us(&trig,50000);    	// PWM of 50ms period. Use period_us()
	PWM_pulsewidth_us(&trig,10);   	// PWM pulse width of 10us
	
// Input Capture configuration -----------------------------------------------------------------------	
	IC_t echo;											// Input Capture for echo
	ICAP_init(&echo,GPIOB,10);    	// ICAP init as PB10 as input caputre
 	ICAP_counter_us(&echo, 10);   	// ICAP counter step time as 10us
	ICAP_setup(&echo, 3, RISE);   	// TIM2_CH3 as IC3 , rising edge detect
	ICAP_setup(&echo, 4, FALL);   	// TIM2_CH3 as IC4 , falling edge detect

// Enable TIMx interrupt			-----------------------------------------------------------------------
	TIM_INT_enable(TIM2);						// TIM2 Interrupt Enable
}

