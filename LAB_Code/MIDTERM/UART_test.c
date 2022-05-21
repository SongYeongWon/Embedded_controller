/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-10-28 by YKKIM  	
* @brief   Embedded Controller:  Simple code for UART Print ___
*					 - _________________________________
* 
******************************************************************************
*/

#include "stm32f411xe.h"
#include "ecRCC.h"
#include "ecSysTick.h"
#include "ecUART.h"  // should be in the same folder with your HAL lib
#include "ecGPIO.h"
#include "ecEXTI.h"
#include "ecTIM.h"
#include "ecPWM.h"

void setup(void);
static int count= 0;
static volatile uint32_t PLL_CLK = 84000000;
static  uint32_t val =0;


static	uint32_t button_press =0;
static	int button_First =0;
static	int button_Second =0;
static	int sec =0;

void EXTI15_10_IRQHandler(void);
void TIM2_IRQHandler(void);

int main(void){
	
 
	setup();
	
	while(1){
		
		printf("count value is  %d ms\r\n", count );
		if(EX==1){
			sec = button_Second - button_First;
			printf("sec is  %d ms\r\n", sec);
			EX=0;
		}
		delay_ms(1000);
	}
}


void setup(void){
	
	RCC_PLL_init(); 
	UART2_init();  //  BAUD:  9600
	SysTick_init(1);

	GPIO_init(GPIOC, BUTTON_PIN, INPUT);
	EXTI_init(GPIOC,BUTTON_PIN,FALL_EXTI,0);
	GPIO_pupdr(GPIOC, BUTTON_PIN, PU);
	
	TIM_INT_init(TIM2,1);
	// GPIO configuration ---------------------------------------------------------------------
	//printf("Setup complete\r\n");
}

void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){// update interrupt flag
		count++;
		clear_UIF(TIM2);// clear by writing 0
		}
}


void EXTI15_10_IRQHandler(void){
   if(is_pending_EXTI(BUTTON_PIN)){
		 button_press ++;
		 if(button_press == 1){
			 button_First= count;
		 }
		 else if(button_press == 2){
			 button_Second = count;
			 EX=1;
			 button_press=0;
		 }
		 clear_pending_EXTI(BUTTON_PIN);
   }
}
