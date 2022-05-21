/**
******************************************************************************
* @author  SONG YEONG WON
* @Mod		 2021-11-19 by YWSONG  	
* @brief   Embedded Controller: LAB : ADC-IR Reflective Sensor
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
#include "math.h"
#include "ecPWM.h"

//IR parameter//
static int flag =0 ;
static uint32_t IR1=0;
static uint32_t IR2=0;
static uint32_t Threshold = 1500;
static int seqCHn[2] = {8,9};

void setup(void);
void ADC_IRQHandler(void);

int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	ADC_start();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){

			printf("IR1 = %d\r\n",IR1);	
			printf("IR2 = %d\r\n",IR2);
			if(IR1<Threshold && IR2<Threshold){
				printf("Go Straight\r\n");
			}
			if(IR1>Threshold && IR2<Threshold){
				printf("Go LEFT\r\n");
			}
			if(IR1<Threshold && IR2>Threshold){
				printf("Go RIGHT\r\n");
			}
			printf("\r\n");
			delay_ms(1000);
		}
}

// InitialiizationW 
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	UART2_init();
	SysTick_init(1);
	
	ADC_init(GPIOB,0,TRGO);
	ADC_init(GPIOB,1,TRGO);
		
	ADC_sequence(2,seqCHn);
}


void ADC_IRQHandler(void){
	if(is_ADC_OVR()){
		clear_ADC_OVR();
	}
	if(is_ADC_EOC()){       //after finishing sequence
	  if(flag==0){
			IR1 = ADC1->DR;			
		}
		else if(flag==1){
			IR2 = ADC1->DR;
		}
		flag =!flag;
	}
}
