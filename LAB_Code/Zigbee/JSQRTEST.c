/**
******************************************************************************
* @author  SONG YEONG WON
* @Mod		 2021-12-17 by YWSONG
* @brief   Embedded Controller:  FINA-LAB
*
******************************************************************************
*/

#include "stm32f411xe.h"
#include "math.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecUART.h"
#include "ecSysTick.h"
#include "ecADC.h"
#include "ecEXTI.h"
#include "string.h"

// USART2 : MCU to PC via usb 
// USART1 : MCU to MCU2 via zigbee

uint8_t mcu2Data = 0;
uint8_t pcData = 0;
int indx =0;
int maxBuf=10;
uint8_t buffer[100]={0,};
int endChar = 13;
static uint8_t endBuf[2] = {'\r\n', '\r\n'};

//--------IR sensor parameter------------------//
static uint32_t IR1=0;
static uint32_t IR2=0;
static int seqCHn[2] = {8,9};

static int DIR = 0;
//--------------Final LAB---------------//
static int LED1_Flag = 0;
static int LED1_COND = 0;
//----------Button----------//
static int Button_Flag = 0;
static int bReceive=0;
//----------PWM------------//
PWM_t pwm1;
static int DC_speed= 2;

void State_input(void);
void setup(void);
void ADC_IRQHandler(void);			//Window open detect and sound sensor
void USART2_IRQHandler(void);
void EXTI15_10_IRQHandler(void); //Button mode
void TIM4_IRQHandler(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	ADC_J_start();
	printf("Hello Nucleo\r\n");

	// Inifinite Loop ----------------------------------------------------------
	while (1){
		
			printf("BLink = %d\r\n",Button_Flag);
			printf("ADC = %d\r\n",IR1);
			printf("\r\n");
			delay_ms(2000000);
	}
}


// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	
	//----------------Button Mode configuration-------------------//
  EXTI_init(GPIOC, BUTTON_PIN, RISE_EXTI, 1);
  GPIO_init(GPIOC, BUTTON_PIN, INPUT);
  GPIO_pupdr(GPIOC, BUTTON_PIN, PD);	
	
	//---------LED1-------------//
	GPIO_init(GPIOA,5,OUTPUT);
	GPIO_ospeed(GPIOA, 5,MEDIUM_SPEED);  
	GPIO_otype(GPIOA, 5, OUTPUT_PP);
	GPIO_pupdr(GPIOA, 5, NOPUPD);
	

	// USART congfiguration
	USART_init(USART2, 9600);
	
	//-----TIMER interrupt--------//
	TIM_INT_init(TIM4,500);
	
	//--------DIrection--------//
	GPIO_init(GPIOA,9,OUTPUT);
	GPIO_ospeed(GPIOA, 9,MEDIUM_SPEED);  
	GPIO_otype(GPIOA, 9, OUTPUT_PP);
	GPIO_pupdr(GPIOA, 9, NOPUPD);
	
	
	TIM_usec_init(TIM3,200);
	PWM_init(&pwm1,GPIOA,6);
	PWM_period_us(&pwm1,200);
	
	//------------IR reflective sensor--------//
	ADC_J_init(GPIOB,0,TRGO);		//window open detect
	ADC_J_init(GPIOB,1,TRGO);		//Glass breaking detect
	ADC_continue(SINGLE);	
	ADC_J_sequence(2,seqCHn);
	

}




//Window Open detect
void ADC_IRQHandler(void){
	if(is_ADC_OVR()){
		clear_ADC_OVR();
	}
	if(is_ADC_JEOC()){       //after finishing sequence
		IR1 = ADC1->JDR2;
		if(IR1<1000){
				PWM_pulsewidth_us(&pwm1,200-50*DC_speed);
		}if(IR1>=1000){
			PWM_pulsewidth_us(&pwm1,0);
		}
		clear_ADC_JEOC();
	}
}


void USART2_IRQHandler(){		//USART1 INT 
	if(is_USART_RXNE(USART2)){
		mcu2Data = USART_getc(USART2);
		USART_write(USART2,&mcu2Data,1);		
		if(mcu2Data==endChar){
			bReceive=1;
			indx = 0;
			USART_write(USART2, &endBuf, 1);
			State_input();
		}
		else{
			if(indx>maxBuf){
				indx =0;
				memset(buffer, 0, sizeof(char) * maxBuf);
				printf("ERROR : Too long string\r\n");
			}
			buffer[indx] = mcu2Data;
			indx++;
		}
	}
} 


void State_input(void){	

			printf("buffer : %s\r\n",buffer);
	if(bReceive==1 && buffer[0]=='F'){	
		switch(buffer[1]){
				case '1' : GPIO_write(GPIOA,9,LOW);  DIR=0;				break;		//FORWARD
				case '0' : GPIO_write(GPIOA,9,HIGH); DIR=1;				break;		//BACKWARD	
        default: printf("ERROR : TYPE U,D,L,R,F,B,S OLNY!!\r\n"); break;
      }
			buffer[0]=NULL;
			buffer[1]=NULL;
		}
	
		if(bReceive==1 && buffer[0]=='B'){	
		switch(buffer[1]){
				case '1' : LED1_COND=1; LED1_Flag=0;				break;		//FORWARD
				case '0' : LED1_COND=0;	LED1_Flag=0;			break;		//BACKWARD	
        default: printf("ERROR : TYPE U,D,L,R,F,B,S OLNY!!\r\n"); break;
      }
			buffer[0]=NULL;
			buffer[1]=NULL;
		}
		
		if(DIR==1){																		//DC Motor control
		PWM_pulsewidth_ms(&pwm1,200-50*DC_speed);
		}else PWM_pulsewidth_ms(&pwm1,50*DC_speed);
}



void EXTI15_10_IRQHandler(void){
   if(is_pending_EXTI(BUTTON_PIN)){
		 Button_Flag++;
		 if(Button_Flag==1){
			 Button_Flag=1;
		 }
		 if(Button_Flag ==2){
			 Button_Flag=0;
		 }
     clear_pending_EXTI(BUTTON_PIN);
   }
}

void TIM4_IRQHandler(void){
	if(is_UIF(TIM4)){
		if(LED1_COND ==0){
			if(Button_Flag==0){
				LED1_Flag ++ ;
			if(LED1_Flag==1){
			GPIO_write(GPIOA,5,HIGH);
		}if(LED1_Flag==2){
			GPIO_write(GPIOA,5,LOW);
			LED1_Flag=0;
			}
		}
	}
		if(LED1_COND ==1){
			if(Button_Flag==0){
				LED1_Flag ++ ;
			if(LED1_Flag==4){
			GPIO_write(GPIOA,5,HIGH);
		}if(LED1_Flag==8){
			GPIO_write(GPIOA,5,LOW);
			LED1_Flag=0;
			}
		}
	}
	if(Button_Flag==1){
		GPIO_write(GPIOA,5,HIGH);
	}
	clear_UIF(TIM4);  							    
	}
}

