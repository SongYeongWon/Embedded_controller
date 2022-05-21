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
#include "string.h"

// USART2 : MCU to PC via usb 
// USART1 : MCU to MCU2 via zigbee

uint8_t mcu2Data = 0;
uint8_t pcData = 0;
int indx =0;
int maxBuf=10;
uint8_t buffer[100]={0,};
int bReceive=0;
int ledOn = 0;
int endChar = 13;

int i =0;

void setup(void);
void LED_init(void);
int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");

	// Inifinite Loop ----------------------------------------------------------
	while (1){
		
		//printf("buffer[0] = %c\r\n",buffer[1]);
		if(bReceive==1 && buffer[0]=='L'){
			printf("buffer : %s\r\n",buffer);
			if (buffer[1]=='0') ledOn = 0;		
			else if (buffer[1]=='1') ledOn = 1;
			else printf("ERROR : Wrong command\r\n");
			bReceive = 0;
		}

		GPIO_write(GPIOA,5,ledOn);
		delay_ms(1000);
	}
}


// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	
	// USART congfiguration
	USART_init(USART2, 9600);
	USART_begin(USART6, GPIOA,11,GPIOA,12, 9600); 	// PA11 - RXD , PA12 - TXDD
	
	// GPIO configuration
	LED_init();
}


void USART6_IRQHandler(){		//USART1 INT 
	if(is_USART_RXNE(USART6)){
		mcu2Data = USART_getc(USART6);
		printf("%c",pcData); 						// echo to sender(pc)		
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
		USART_write(USART6,&pcData,1);	// transmit char to USART1
		printf("%c",pcData); 						// echo to sender(pc)
		
		if(pcData==endChar){
			printf("\r\n"); 							// to change lb   4ine on PC display
		}
	}
}

void LED_init(void){
	GPIO_init(GPIOA,5,OUTPUT);
	GPIO_ospeed(GPIOA, 5,MEDIUM_SPEED);  
	GPIO_otype(GPIOA, 5, OUTPUT_PP);
	GPIO_pupdr(GPIOA, 5, NOPUPD);
}


