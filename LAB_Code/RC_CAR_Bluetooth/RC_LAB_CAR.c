/**
******************************************************************************
* @author  SONG YEONG WON
* @Mod		 2021-12-3 YWSONG
* @brief   Embedded Controller:  USART communication
*
******************************************************************************
*/

#include "ecHAL.h"

// USART2 : MCU to PC via usb 
// USART1 : MCU to MCU2

uint8_t mcu2Data = 0;
uint8_t pcData = 0;
int indx =0;
int maxBuf=10;
uint8_t buffer[100]={0,};
int endChar = 13;

PWM_t pwm1;
PWM_t pwm2;
//Servo angle
static int count= 2;

//DC speed
static int DC_speed= 0;
static int DIR = 0;

static int Angle[5] = {0,45,90,135,180}; 				//Angle state
static int VEL[5] = {0,25,50,75,100};						//Dc speed state
static char *Direction[2] = {"FWD","BWD"};			//Direction state Default FWD

static uint8_t endBuf[2] = {'\r\n', '\r\n'};

void setup(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void State_input(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello RC car\r\n");
	PWM_pulsewidth_ms(&pwm2,2);
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		printf("RC car -> DIR : %d[deg] VEL : %d[per] %s \r\n", Angle[count],VEL[DC_speed],Direction[DIR]);
		delay_ms(1000000);
	}
}

// Initialiization 
void setup(void){
	RCC_PLL_init();
	SysTick_init(1);
	PWM_pulsewidth_ms(&pwm1,1.5); //Initiall Servo motor angle 90
	//Motor direction Pin
	GPIO_init(GPIOA,5,OUTPUT);
	GPIO_ospeed(GPIOA, 5,MEDIUM_SPEED);  
	GPIO_otype(GPIOA, 5, OUTPUT_PP);
	GPIO_pupdr(GPIOA, 5, NOPUPD);
	
	//TIM 2 -> RC Servo angle
	TIM_init(TIM2, 20);
	PWM_init(&pwm1,GPIOA,1);
	PWM_period_ms(&pwm1,20);
	
	//TIM 3 -> DC Motor Speed
	TIM_init(TIM3,2);
	PWM_init(&pwm2,GPIOA,6);
	PWM_period_ms(&pwm2,2);
	
	// USART congfiguration
	USART_init(USART2, 38400);
	USART_begin(USART1, GPIOA,9,GPIOA,10, 9600); 	// PA9 - RXD , PA10 - TXD
}


void USART1_IRQHandler(){		//USART1 INT 
	if(is_USART_RXNE(USART1)){
		mcu2Data = USART_getc(USART1);
		USART_write(USART1,&mcu2Data,1);		
		if(mcu2Data==endChar){
			//bReceive=1;
			indx = 0;
			USART_write(USART1, &endBuf, 1);
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

void USART2_IRQHandler(){		//USART2 INT 
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		USART_write(USART1,&pcData,1);	// transmit char to USART1
		printf("%c",pcData); 						// echo to sender(pc)
		
		if(pcData==endChar){
			printf("\r\n"); 							// to change line on PC display
		}
	}
}

void State_input(void){	
	//Forward, Backward, Stop
			printf("buffer : %s\r\n",buffer);
			switch(buffer[0]){
				case 'U' : DC_speed++;	if(DC_speed>4)DC_speed=4;	break;  	//UP
				case 'D' : DC_speed--;	if(DC_speed<0)DC_speed=0;	break; 		//DOWN
				case 'L' : count--;  		if(count<0)count=0;				break;		//LEFT
				case 'R' : count++; 		if(count>4)count=4;				break;		//RIGHT
				case 'F' : GPIO_write(GPIOA,5,LOW);  DIR=0;				break;		//FORWARD
				case 'B' : GPIO_write(GPIOA,5,HIGH); DIR=1;				break;		//BACKWARD
				case 'S' : count=2; DC_speed=0;										break;		//STOP		
        default: printf("ERROR : TYPE U,D,L,R,F,B,S OLNY!!\r\n"); break;
      }
			buffer[0]=NULL;

		PWM_pulsewidth_ms(&pwm1,0.5*count+0.5);       //Servo Motor angle (LEFT/RIGHT)
		if(DIR==1){																		//DC Motor control
		PWM_pulsewidth_ms(&pwm2,2-0.5*DC_speed);
		}else PWM_pulsewidth_ms(&pwm2,0.5*DC_speed);
}

