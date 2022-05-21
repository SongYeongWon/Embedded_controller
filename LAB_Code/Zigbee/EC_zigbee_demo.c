/**
******************************************************************************
* @author  SONG YEONG WON
* @Mod		 2021-12-07 by YWSONG
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

// USART2 : MCU to PC via usb 
// USART1 : MCU to MCU2 via zigbee

// USART2 : MCU to PC via usb 

uint8_t buf1[4];
uint8_t buf2[4];
uint8_t buf3[4];
uint8_t buf4[4];
uint8_t buf5[4];
uint8_t buf6[4];
uint8_t buf7[4];


uint8_t mcu2Data = 0;
uint8_t pcData = 0;
int indx =0;
int maxBuf=50;
uint8_t buffer[100]={0,};
int bReceive=0;
int ledOn = 0;
int endChar = 13;

//-------Ultrasonic variable------------//
static int overflow = 0;
float distance = 0;
static float timeSt = 0;
static float timeEnd= 0;
static float sec = 0;
static int PERSON_OUTSIDE =0;			// Door front Flag

//--------IR sensor parameter------------------//
static int flag =0 ;
static uint32_t IR1=0;
static uint32_t IR2=0;
static int seqCHn[2] = {8,9};
static int WIN_OPEN =0;							// Window open detect Flag

//--------PIR motion sensor parameter------------//
static int PERSON_INSIDE =0;

//--------Light Intensity Sensor parameter-------//
static uint32_t CURTAIN_OPEN =0;
uint32_t Light_val = 0;

//--------Button mode parameter------------------//
static int button_pressed = 0;
static int Button_MODE= 0;

//---------TIM5 DAQ parameter--------------------//
static int DAQ_FLAG= 0;

static uint32_t light= 0;

//MODE, Ultrasonic, PIR, Window, Curtain, Sound, end of char 
unsigned char SENSOR[7] = {'0','0','0','0','0','0','E'};


void setup(void);
void TIM2_IRQHandler(void);			//Front door -> Ultrasonic distance sensor
void ADC_IRQHandler(void);			//Window open detect and sound sensor
void EXTI9_5_IRQHandler(void); 	//Inside door -> PIR motion sensor
void EXTI4_IRQHandler(void);		//Light Intensity Sensor
void USART6_IRQHandler(void);   //Zigbee comunication
void USART2_IRQHandler(void);
void USART1_IRQHandler(void);
void EXTI15_10_IRQHandler(void); //Button mode
void TIM5_IRQHandler(void); 		//DAQ log

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	ADC_start();
	printf("Hello Nucleo\r\n");
	
	//USART1 excel_DAQ initialize 
	//USART_write(USART1,(unsigned char*) "CLEARSHEET\r\n",12);	
	//USART_write(USART1,(unsigned char*) "LABEL,Date,Time,Timer,MODE,Dist,PIR,IR,Light,SOUND\r\n",55);	
	
	// Inifinite Loop ----------------------------------------------------------
	while (1){
		
		//sprintf(buf1, "%d", SENSOR[0]-48);
		//sprintf(buf2, "%d", SENSOR[1]-48);
		//sprintf(buf3, "%d", SENSOR[2]-48);
		//sprintf(buf4, "%d", SENSOR[3]-48);
		//sprintf(buf5, "%d", SENSOR[4]-48);
		//sprintf(buf6, "%d", SENSOR[5]-48);
		//printf(" overflow = %d\r\n ",DAQ_overflow);
		
		if(DAQ_FLAG==1){
	
			//USART1 Trasnmit sensor value to server
		/*USART_write(USART1,(unsigned char*) "DATA,DATE,TIME,TIMER,",21);	// transmit char to USART6
		USART_write(USART1,&buf1,4);
		USART_write(USART1,(unsigned char*) ",",1);	// transmit char to USART6
		USART_write(USART1,&buf2,4);
		USART_write(USART1,(unsigned char*) ",",1);	// transmit char to USART6
		USART_write(USART1,&buf3,4);
		USART_write(USART1,(unsigned char*) ",",1);	// transmit char to USART6
		USART_write(USART1,&buf4,4);
		USART_write(USART1,(unsigned char*) ",",1);	// transmit char to USART6
		USART_write(USART1,&buf5,4);
		USART_write(USART1,(unsigned char*) ",",1);	// transmit char to USART6
		USART_write(USART1,&buf6,4);
		USART_write(USART1,(unsigned char*) ",AUTOSCROLL_20\r\n",16);	// transmit char to USART6
		*/
		
		printf("BUTTON = %c\r\n",SENSOR[0]);
		printf("ULTRASONIC = %c\r\n",SENSOR[1]);
		printf("PIR = %c\r\n",SENSOR[2]);
		printf("WINDOW = %c\r\n",SENSOR[3]);
		printf("CURTAIN = %c\r\n",SENSOR[4]);
		printf("SOUND	= %c\r\n",SENSOR[5]);
		//printf("Light_val	= %d\r\n", Light_val);
		//printf("CURTAIN_OPEN = %d\r\n", CURTAIN_OPEN);
		printf("\r\n");
		
		printf("SOUND	= %f\r\n",distance);
		//printf("LIGHT SENSOR	= %d\r\n",GPIO_read(GPIOB,4));
		//printf("light = %d\r\n",light);
		DAQ_FLAG=0;
		}

		USART_write(USART6,&SENSOR,7);

		delay_ms(1000);
		//SENSOR[4] = '0';
	}
}


// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	
	//----------------Button Mode configuration-------------------//
  EXTI_init(GPIOC, BUTTON_PIN, FALL_EXTI, 1);
  GPIO_init(GPIOC, BUTTON_PIN, INPUT);
  GPIO_pupdr(GPIOC, BUTTON_PIN, PD);	
	
	//----------------PIR motion sensor(inside door)--------------//
	GPIO_init(GPIOB, 8,INPUT);
	EXTI_init(GPIOB,8,BOTH_EXTI,2);
	GPIO_pupdr(GPIOB, 8, PU);
	
	//----------------Light intensity sensor(Curtain)--------------//
	GPIO_init(GPIOB, 4, INPUT);
	EXTI_init(GPIOB,4,BOTH_EXTI,3);
	GPIO_pupdr(GPIOB, 4, PU);
	
	
	// USART congfiguration
	USART_init(USART2, 9600);
	USART_begin(USART1, GPIOA,9,GPIOA,10, 9600); 	// PA9 - RXD , PA10 - TXD
	USART_begin(USART6, GPIOA,11,GPIOA,12, 9600); 	// PA11 - RXD , PA12 - TXD
	
	
	//------------Ultrasonic distance sensor(Front door)--------//
	
	// PWM configuration -----------------------------------------------------------------	
	PWM_t trig;											// PWM1 for trig
	PWM_init(&trig,GPIOB,6);			 	// PWM init as PA_1: Ultrasonic trig pulse TIM2_CH2
	PWM_period_us(&trig,50000);    	// PWM of 50ms period. Use period_us()
	PWM_pulsewidth_us(&trig,10);   	// PWM pulse width of 10us
	
	// Input Capture configuration -------------------------------------------------------
	IC_t echo;											// Input Capture for echo
	ICAP_init(&echo,GPIOB,10);    	// ICAP init as PB10 as input caputre
 	ICAP_counter_us(&echo, 10);   	// ICAP counter step time as 10us
	ICAP_setup(&echo, 3, RISE_TIM);   	// TIM2_CH3 as IC3 , rising edge detect
	ICAP_setup(&echo, 4, FALL_TIM);   	// TIM2_CH3 as IC4 , falling edge detect

	// Enable TIMx interrupt			--------------------------------------------------------
	TIM_INT_enable(TIM2);						// TIM2 Interrupt Enable
	
	//------------Sound sensor and IR reflective sensor--------//
	ADC_init(GPIOB,0,TRGO);		//Glass breaking detect
	ADC_init(GPIOB,1,TRGO);		//window open detect
	ADC_sequence(2,seqCHn);
	
	//------------TIM5 configuration---------------------------//
	TIM_INT_init(TIM5,1000);
}


//Front Door
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
			distance = (float) sec*340/(2*10000); // Ultrasonic speed[m/s] * echo pulse duration[us]
			if(distance<30){
				SENSOR[1] = '1';
			}else SENSOR[1] = '0';
			overflow=0;
			clear_CCIF(TIM2,4);								 // clear capture/compare interrupt flag 
	}
}


//Window Open detect
void ADC_IRQHandler(void){
	if(is_ADC_OVR()){
		clear_ADC_OVR();
	}
	if(is_ADC_EOC()){       //after finishing sequence
	  if(flag==0){
			IR1 = ADC1->DR;
			if(IR1>3000){
				SENSOR[3] = '0';
			}else SENSOR[3] = '1';
		}
		else if(flag==1){
			IR2 = ADC1->DR;
		}
		flag =!flag;
	}
}

//Door person detect (inside)
void EXTI9_5_IRQHandler(void){
   if(is_pending_EXTI(8)){
		 PERSON_INSIDE++;
		 if(PERSON_INSIDE==1){
		 SENSOR[2] = '1';
		 }else if(PERSON_INSIDE==2){
		 SENSOR[2] = '0';
		 PERSON_INSIDE=0;
		 }
		 clear_pending_EXTI(8);
   }
}

//Daylight intensity
void EXTI4_IRQHandler(void){
   if(is_pending_EXTI(4)){
		/* CURTAIN_OPEN++;
		 //printf("LIGHT SENSOR	= %d\r\n",GPIO_read(GPIOB,4));
		  if(GPIO_read(GPIOA,4) == 0){
		 SENSOR[4] = '1';
		 }else if(CURTAIN_OPEN==2){
		 SENSOR[4] = '0';
		 CURTAIN_OPEN=0;
		 }*/
		
		 if(GPIO_read(GPIOB,4) == 16){
				SENSOR[4] = '1';
		 }else SENSOR[4] = '0';
		 
		 /*if(GPIO_read(GPIOA,4) != 0){
			 SENSOR[4] = '0';		 
		 }*/
		 //SENSOR[4] = '1';
//		 SENSOR[4] = 49;
		// Light_val = GPIO_read(GPIOB, 4);
		 //if(Light_val == 16) CURTAIN_OPEN = 0;
		// else if(Light_val == 0) CURTAIN_OPEN = 1;
		 /*
		 CURTAIN_OPEN++;
		 if(CURTAIN_OPEN==1){
		 SENSOR[4] = '1';
		 }else if(CURTAIN_OPEN==2){
		 SENSOR[4] = '0';
		 CURTAIN_OPEN=0;
		 }*/
		 clear_pending_EXTI(4);
   }
}


//Comunication Zigbee
void USART6_IRQHandler(){		//USART1 INT 
	if(is_USART_RXNE(USART6)){
		mcu2Data = USART_getc(USART6);
		//printf("%c",mcu2Data); 						// echo to sender(pc)		
		if(mcu2Data=='E'){
			bReceive=1;
			indx = 0;
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

//Comunication muc1
void USART2_IRQHandler(){		//USART2 INT 
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		USART_write(USART6,&pcData,1);	// transmit char to USART1
		printf("%c",pcData); 						// echo to sender(pc)
		
		if(pcData==endChar){
			printf("\r\n"); 							// to change line on PC display
		}
	}
}

//Comunication DAQ
void USART1_IRQHandler(){		//USART1 INT 
	if(is_USART_RXNE(USART1)){
		mcu2Data = USART_getc(USART1);						
		if(mcu2Data==endChar) {
			bReceive=1;
			indx = 0;
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


void EXTI15_10_IRQHandler(void){
   if(is_pending_EXTI(BUTTON_PIN)){
     button_pressed++;
		 if(button_pressed==1){
			 Button_MODE=!Button_MODE;
			 if(Button_MODE==1){
				 SENSOR[0] = '1';
			 }else SENSOR[0] = '0';
			 button_pressed=0;
		 }
      clear_pending_EXTI(BUTTON_PIN);
   }
}

void TIM5_IRQHandler(void){
	if(is_UIF(TIM5)){                     // Update interrupt
			DAQ_FLAG=1;
		clear_UIF(TIM5);  							    // clear update interrupt flag
	}
}
