/**
******************************************************************************
* @author  Song Yeong Won
* @Mod		 2021-11-12 by YWSong
* @brief   Embedded Controller:  LAB : Input Capture - Ultrasonic Distance Sensor
* ******************************************************************************
*/


#include "stm32f411xe.h"
#include "math.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecUART.h"
#include "ecPWM.h"
#include "ecSysTick.h"


static int overflow = 0;
static int Start_ov = 0;
static int End_ov = 0;
static float period = 0;
static uint32_t Start_rising = 0;
static uint32_t End_rising = 0;

void TIM2_IRQHandler(void);

int main(void){
	
	RCC_PLL_init(); 
	UART2_init();
  SysTick_init(1);
	// GPIO configuration ---------------------------------------------------------------------
	GPIO_init(GPIOA, 0, ALTERNATE);	// PB10: Alternate Function Mode
	// TIM configuration -----------------------------------------------------------------------
	GPIOA->AFR[0] |= 1 << (4*0);					 			 //AF1(TIM2)
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;          //TIM2 Clock enabled

	TIM2->PSC = 84-1;															// Timer counter clock: 1M`Hz(1us)
	TIM2->ARR = 65535;														// Set auto reload register to maximum (count up to 65535)
	TIM2->CR1 &= ~(1UL<<4);										    // CR1 DIR enabled
	
	TIM2->CCMR1 &= ~(3UL<<0);
	TIM2->CCMR1 |= 1<<0;													// Capture/Compare Selection: CC1 is mapped on TI1 
	
	TIM2->CCMR1 &=  ~(15UL<<4);                 	// Clear IC1F
	TIM2->CCMR1 |=  2<<4	;                       // Set filter N=4
	
	TIM2->CCER  &= ~(1<<0);												// Clear CCER
	TIM2->CCER  &= ~(1<<1);                       
	TIM2->CCER  &= ~(1<<3);			                  // Capture rising edge

	TIM2->CCER |= 1<<0; 									        // Capture enabled
	TIM2->DIER |= 1<<1;												    // Interrupt enabled
	TIM2->DIER |= 1<<0;									          // Update interrupt enable	
	TIM2->CR1  |= 1<<0;										        // Counter enable


	NVIC_SetPriority(TIM2_IRQn, 2);						     // Set the priority of TIM2 interrupt request
	NVIC_EnableIRQ(TIM2_IRQn);									   // TIM2 interrupt request enable

	while(1)
	{
		printf("%f[sec]\r\n", period); 							// print out the period on TeraTerm
		delay_ms(1000);
	}
}

void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                  					 // Update interrupt
		overflow ++;
		clear_UIF(TIM2);  							           // clear update interrupt flag
	}
	if(is_CCIF(TIM2)){
		EX ++;
		if(EX==1){
			Start_rising = TIM2->CCR1;
		}
		if(EX==2){
			End_rising = TIM2->CCR1;
			if(End_rising>=Start_rising){
			period =((End_rising-Start_rising)+(overflow*(0xFFFF+1)))*0.000001;
			}
			else if(End_rising<Start_rising){
			period =((0xFFFF-Start_rising+End_rising)+(overflow-1)*(0xFFFF+1))*0.000001;
			}
			Start_rising = End_rising;
			EX=1;
			overflow=0;
		}
		clear_CCIF(TIM2); 							         		// clear capture/compare interrupt flag ( it is also cleared by reading TIM2_CCR1)
	}
}
