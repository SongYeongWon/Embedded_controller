/**
******************************************************************************
* @author  SONG YEONG WON
* @Mod		 2021-12-23 by YWSONG
* @brief   Embedded Controller:  USART communication
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
#include "ecStepper.h"


uint8_t mcu2Data = 0;
uint8_t pcData = 0;
int indx =0;
int maxBuf=50;
uint8_t buffer[100]={0,};
int bReceive=0;
int ledOn = 0;
//int endChar = 13;
int endChar = 69;


//SIREN MODE count number
static uint32_t SRFG_count = 6;
static uint32_t SRFG_toggle = 0;

static uint32_t mode =0;
static uint32_t siren =1;
static uint32_t siren_MODE =1;

//------------TIM5 Stepper Motor------------//
static int stepper_Toggle =0;
static int Stepper_Flag = 0;
static int Stepper_day = 1;
static int Stepper_night = 1;
static int Stepper_MODE = 1;
static int Stepper_print = 0;

//----------SIREN TRIGER----------------//
static int SIREN_sevenprint = 0;
static int SIREN_PRINTMODE = 1;
static int Curtain_siren =0;
static int Stepper_siren =1;

//-----------INSIDE 5sec maintain-------------//
static int INSIDE_cnt = 0;
static int INSIDE_FLAG = 0;
static int INSIDE_FLAG_SECURE =0;
//-----------SECURE MODE--------------//

unsigned char BUTTON[3] = {'B','0','E'};
unsigned char Ultra[3] = {'U','0','E'};
unsigned char PIR[3] = {'P','0','E'};
unsigned char WINDOW[3] = {'I','0','E'};
unsigned char CURTAIN[3] = {'C','0','E'};
unsigned char SOUND[3] = {'S','0','E'};


unsigned char STOP_buffer[3] = {'T','1','E'};

static	uint8_t Button_v = 48;
static	uint8_t Ultrasonic_v = 48;
static	uint8_t PIR_v = 48;
static	uint8_t Sound_v = 48;
static	uint8_t IR_v = 0;
static	uint8_t Light_v = 48;
static 	uint8_t Siren_v = 48;

void setup(void);
void TIM2_IRQHandler(void);			//Front door -> Ultrasonic distance sensor
void USART6_IRQHandler(void);
void EXTI15_10_IRQHandler(void); //Button mode
void TIM5_IRQHandler(void);
void TIM2_IRQHandler(void);
void SMART_HOME(void);


int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");
	// Inifinite Loop ----------------------------------------------------------
	while (1){
		
			//end of Siren
			if(SIREN_sevenprint==1){
				Stepper_step(2048*2, 1, HALF);
				Stepper_siren=0;
				SIREN_sevenprint=0;
			}
			
			//NORMAL MODE
			if(Stepper_print==1){
				Stepper_step(2048*2, 1, HALF);
				//Stepper_day=0;
				stepper_Toggle=1;
				Stepper_print=0;
				//Stepper_Flag =0;
			}	if(Stepper_print==2){
				Stepper_step(2048*2, 0, HALF);
				//Stepper_night =0;
				stepper_Toggle=0;
				Stepper_print=0;
		}
	}
}


// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	
	//----------SIREN MODE 7-segement configuration-----------//
	sevensegment_init();

	//OUTSIDE LED
  GPIO_init(GPIOB, 8, OUTPUT);
	GPIO_ospeed(GPIOB, 8, MEDIUM_SPEED);
	GPIO_otype(GPIOB, 8, OUTPUT_PP);
	GPIO_pupdr(GPIOB, 8, NOPUPD);
	
	//INSIDE LED
	GPIO_init(GPIOB, 9, OUTPUT);
	GPIO_ospeed(GPIOB, 9, MEDIUM_SPEED);
	GPIO_otype(GPIOB, 9, OUTPUT_PP);
	GPIO_pupdr(GPIOB, 9, NOPUPD);

		//----------------Button Mode configuration-------------------//
  EXTI_init(GPIOC, BUTTON_PIN, FALL_EXTI, 1);
  GPIO_init(GPIOC, BUTTON_PIN, INPUT);
  GPIO_pupdr(GPIOC, BUTTON_PIN, PD);
	
	//------------TIM5 configuration---------------------------//
	//TIM_INT_usec_init(TIM5,100);
	
	//-------------Stepper Motor-------------------------------//
	Stepper_init(GPIOB,10,GPIOB,4,GPIOB,5,GPIOB,3); // Stepper GPIO pin initialization
	Stepper_setSpeed(10,HALF);                          //  set stepper motor speed
	
	
	//------------TIM2 configuration---------------------------//
	TIM_INT_init(TIM2,1000);
	
	// USART congfiguration
	USART_init(USART2, 9600);
	USART_begin(USART6, GPIOA,11,GPIOA,12, 9600); 	// PA11 - RXD , PA12 - TXD
}

void USART6_IRQHandler(){		//USART1 INT 
	if(is_USART_RXNE(USART6)){
		mcu2Data = USART_getc(USART6);	
		//printf("%c",mcu2Data);
		buffer[indx] = mcu2Data;
		indx++;
		
		if(mcu2Data=='E'){
			indx = 0;
			switch(buffer[0]){
				case 'B': Button_v = buffer[1]; break;
				case 'U': Ultrasonic_v = buffer[1]; break;
				case 'P': PIR_v = buffer[1]; break;
				case 'I': IR_v = buffer[1]; break;
				case 'C': Light_v = buffer[1]; break;
				case 'S': Sound_v = buffer[1]; break;
				case 'X': Siren_v = buffer[1]; break;			
			default: ;
		}
			printf("%c %c\r\n",buffer[0],buffer[1]);
	}
		else{
			if(indx>maxBuf){
				indx =0;
				memset(buffer, 0, sizeof(char) * maxBuf);
				printf("ERROR : Too long string\r\n");
			}
		}
	}
}

//STOP BUTTON
void EXTI15_10_IRQHandler(void){
   if(is_pending_EXTI(BUTTON_PIN)){
		 siren=0;
		 sevensegment_decoder(5);
		 Siren_v =0;
		 INSIDE_FLAG_SECURE=0;
		 GPIO_write(GPIOB,9,LOW);
		 stepper_Toggle=1;
		 USART_write(USART6,STOP_buffer,3);
     clear_pending_EXTI(BUTTON_PIN);
   }
}


void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                     // Update interrupt
		if(INSIDE_FLAG==1){
				GPIO_write(GPIOB,9,HIGH);
				INSIDE_cnt++;
			if(INSIDE_cnt==3){
				GPIO_write(GPIOB,9,LOW);
				INSIDE_cnt=0;
				INSIDE_FLAG=0;
			}
		}
		SMART_HOME();
		clear_UIF(TIM2);  							    // clear update interrupt flag
	}
}


void SMART_HOME(void){
	//-----------------------------------------------------------------------//
	//------------------------------NORMAL MODE------------------------------//
	//-----------------------------------------------------------------------//
	
	if(Button_v == 48){
		//Normal mode steup
		sevensegment_decoder(1);
		SRFG_count=6;
		siren=1;
		Stepper_siren=1;
		
		//OUTSIDE PEOPEL DETECT
		if(Ultrasonic_v == 49){
			GPIO_write(GPIOB,8,HIGH);
		}if(Ultrasonic_v == 48){
			GPIO_write(GPIOB,8,LOW);   //5 sec on and off
		}
		
		//INSIDE PEOPEL DETECT
		if(PIR_v == 49){
			INSIDE_FLAG=1;
		}
		
		//Curtain Daytime Lighttime 
		if(Light_v == 48 &&  stepper_Toggle==0){
			Stepper_Flag = Stepper_day;
			if(Stepper_Flag == Stepper_MODE){
				Stepper_print=1;
			}
		}if(Light_v == 49 && stepper_Toggle==1){
			Stepper_Flag = Stepper_night;
			if(Stepper_Flag == Stepper_MODE){
				Stepper_print=2;
			}
		}
		
	}
	
	//-----------------------------------------------------------------------//
	//------------------------------SECURE MODE------------------------------//
	//-----------------------------------------------------------------------//
	else if(Button_v == 49){
		sevensegment_decoder(5);
		Stepper_day=1;
		Stepper_night=1;
		
		//OUTSIDE PEOPEL DETECT
		if(Ultrasonic_v == 49){
			GPIO_write(GPIOB,8,HIGH);
			//VSITOR_LOG MESSAGE TO the server
		}else GPIO_write(GPIOB,8,LOW);
		
		//INSIDE PEOPEL DETECT
		if(PIR_v == 49){
			INSIDE_FLAG=1;
		}
		
		if(INSIDE_FLAG==1){
			GPIO_write(GPIOB,9,HIGH);
		}
		if(INSIDE_FLAG==0){
			GPIO_write(GPIOB,9,LOW);
		}
			if(Siren_v ==49){				
				mode = siren ;
		
		if(mode == siren_MODE){
			if(SRFG_count>=1){	
					SRFG_count--;
				sevensegment_decoder(SRFG_count);
				}
				if(SRFG_count<1){
						sevensegment_decoder(SRFG_toggle);
						Curtain_siren=Stepper_siren;
					if(Curtain_siren==SIREN_PRINTMODE){
						SIREN_sevenprint=1;
					}
						if(SRFG_toggle==0){
							SRFG_toggle=10;
						}else SRFG_toggle=0;
				}
			}
		}
	}
}

