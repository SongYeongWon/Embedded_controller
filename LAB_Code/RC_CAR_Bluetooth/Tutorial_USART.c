/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM
* @brief   Embedded Controller:  USART communication
*
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecUART.h"
#include "ecSysTick.h"
#include "String.h"
#include "stdlib.h"

// USART2 : MCU to PC via usb 
// USART1 : MCU to MCU2

uint8_t mcu2Data = 0;
uint8_t pcData = 0;
int indx =0;
int maxBuf=10;
uint8_t buffer[100]={0,};
int bReceive=0;
uint32_t led1On = 0;
uint32_t led2On = 0;
uint32_t led3On = 0;
int endChar = 13;

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void LED_init(void);
void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");
	// Inifinite Loop ----------------------------------------------------------
	while (1){
		
		//printf USART 2
		if(bReceive==1 && buffer[0]=='L'){
			printf("buffer : %s\r\n",buffer);
			switch(buffer[1]){  
				case '0' : led1On =0;led2On =0;led3On =0 ; break;
				case '1' : led1On =0;led2On =0;led3On =1 ; break;
				case '2' : led1On =0;led2On =1;led3On =0 ; break;
				case '3' : led1On =0;led2On =1;led3On =1 ; break;
				case '4' : led1On =1;led2On =0;led3On =0 ; break;
				case '5' : led1On =1;led2On =0;led3On =1 ; break;
				case '6' : led1On =1;led2On =1;led3On =0 ; break;
				case '7' : led1On =1;led2On =1;led3On =1 ; break;				
         default: printf("ERROR : TYPE from L0 to L7 OLNY!!\r\n"); break;
      }
			bReceive = 0;
		}
		GPIO_write(GPIOA,5,led1On);
		GPIO_write(GPIOA,6,led2On);
		GPIO_write(GPIOA,7,led3On);
		delay_ms(500);
	}
}

// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	
	// USART congfiguration
	USART_init(USART2, 38400);
	USART_begin(USART1, GPIOA,9,GPIOA,10, 9600); 	// PA9 - RXD , PA10 - TXD
	
	// GPIO configuration
	LED_init();
}

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

void LED_init(void){
	GPIO_init(GPIOA, 5, OUTPUT);  
	GPIO_ospeed(GPIOA, 5,MEDIUM_SPEED);  
	GPIO_otype(GPIOA, 5, OUTPUT_PP);
	GPIO_pupdr(GPIOA, 5, NOPUPD);	
	
	GPIO_init(GPIOA, 6, OUTPUT);  
	GPIO_ospeed(GPIOA, 6,MEDIUM_SPEED);  
	GPIO_otype(GPIOA, 6, OUTPUT_PP);
	GPIO_pupdr(GPIOA, 6, NOPUPD);	
	
	GPIO_init(GPIOA, 7, OUTPUT);  
	GPIO_ospeed(GPIOA, 7,MEDIUM_SPEED);  
	GPIO_otype(GPIOA, 7, OUTPUT_PP);
	GPIO_pupdr(GPIOA, 7, NOPUPD);	
}
