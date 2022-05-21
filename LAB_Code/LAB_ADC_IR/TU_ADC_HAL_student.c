/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  Tutorial ___
*					 - _________________________________
* 
******************************************************************************
*/
#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecSysTick.h"
#include "ecUART.h"
#include "ecADC.h"

//IR parameter//
float result_v =0;

void setup(void);
void ADC_IRQHandler(void);

int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	SysTick_init(1);
	ADC_start();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
	printf("voltage = %.3f\r\n",result_v*3.3/4095);	
	delay_ms(1000);
	}
}

// InitialiizationW 
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	UART2_init();
	ADC_init(GPIOA,1,SW);
	
}


void ADC_IRQHandler(void){
	if(is_ADC_EOC(ADC1)){       //after finishing sequence
	  result_v = ADC_read();
 }
}
