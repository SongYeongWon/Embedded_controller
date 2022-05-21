/**
******************************************************************************
* @author  SONG YEONG WON
* @Mod		 2021-11-19 by YWSONG  	
* @brief   Embedded Controller: LAB : ADC-IR Reflective Sensor
* 
******************************************************************************
*/


#include "ecHAL.h"
//IR parameter//
static int flag =0 ;
static uint32_t IR1=0;
static uint32_t IR2=0;
static uint32_t Threshold = 1500;
static int seqCHn[2] = {8,9};
static int day =0;

void setup(void);
void ADC_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI4_IRQHandler(void);
int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	ADC_start();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){

			/*printf("IR1 = %d\r\n",IR1);	
		if(IR1<3000){
			printf("Window Flag on\r\n");	
		}*/
			//printf("Sound = %d\r\n",IR2);
			/*if(IR1<Threshold && IR2<Threshold){
				printf("Go Straight\r\n");
			}
			if(IR1>Threshold && IR2<Threshold){
				printf("Go LEFT\r\n");
			}
			if(IR1<Threshold && IR2>Threshold){
				printf("Go RIGHT\r\n");
			}*/
			//printf("UMM\r\n");
		if(EX==0){
			printf("daytime\r\n");
		}
		if(EX==1){
			printf("night time\r\n");
		}
			delay_ms(1000);
		}
}

// InitialiizationW 
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	UART2_init();
	SysTick_init(1);
	
	GPIO_init(GPIOB, 8, INPUT);
	EXTI_init(GPIOB,8,FALL_EXTI,0);
	GPIO_pupdr(GPIOB, 8, PU);
	
	GPIO_init(GPIOB, 4, INPUT);
	EXTI_init(GPIOB,4,BOTH_EXTI,0);
	GPIO_pupdr(GPIOB, 4, PU);
	
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

void EXTI9_5_IRQHandler(void){
   if(is_pending_EXTI(8)){
		 //EX = 1;
		 clear_pending_EXTI(8);
   }
}

void EXTI4_IRQHandler(void){
   if(is_pending_EXTI(4)){
		 day++;
		 if(day==1){
			 EX = 1;
		 }
		 if(day==2){
			 EX = 0;
			 day= 0;
		 }
		 clear_pending_EXTI(4);
   }
}
