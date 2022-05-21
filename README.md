---
  description: EC_HAL API
---

# Embedded Controller HAL

Written by:   송영원

Course:  임베디드컨트롤러



Program: 		C/C++

IDE/Compiler: Keil uVision 5

OS: 					WIn10

MCU:  				STM32F411RE, Nucleo-64





## GPIO Digital In/Out 

### Header File

 `#include "ecGPIO.h"`


```c++
// Distributed for LAB: GPIO

#include "stm32f411xe.h"
#include "ecRCC.h"
#ifndef __ECGPIO_H
#define __ECGPIO_H

#define LOW  			0
#define HIGH 			1

#define RESET 3UL
	
//MODER
#define INPUT 			0
#define OUTPUT  		1
#define ALTERNATE 		2
#define ANALOG  		3

//OSPEED
#define LOW_SPEED 		0
#define MEDIUM_SPEED  	1
#define FAST_SPEED 		2
#define HIGH_SPEED  	3

//PUPDR
#define NOPUPD 			0
#define PU  			1 //pull-up
#define PD 				2 //pull-down
#define RESERVED  		3


//OTYPE

#define OUTPUT_PP		0
#define OUTPUT_OD 		1

//7-Segment LED PINs
#define LED_PA5   		5
#define LED_PA6    		6
#define LED_PA7    		7
#define LED_PB6    		6
#define LED_PC7    		7
#define LED_PA9    		9	
#define LED_PA8    		8
#define LED_PB10   		10
#define BUTTON_PIN 		13

//PWM
#define PA1   			1

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void setup(void);
void GPIO_init(GPIO_TypeDef *Port, int pin, uint32_t mode);
void GPIO_mode(GPIO_TypeDef* Port, int pin, uint32_t mode);

void GPIO_write(GPIO_TypeDef *Port, int pin, uint32_t Output);
uint32_t  GPIO_read(GPIO_TypeDef *Port, int pin);
void GPIO_ospeed(GPIO_TypeDef* Port, int pin, uint32_t speed);
void GPIO_otype(GPIO_TypeDef* Port, int pin, uint32_t type);
void GPIO_pupdr(GPIO_TypeDef* Port, int pin, uint32_t pudr);


uint32_t ButtonST(uint32_t value);
void sevensegment_init(void);
void sevensegment_decoder(uint32_t num);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

```




### GPIO_init\(\)

Initializes GPIO pins with default setting and Enables GPIO Clock. Mode: In/Out/AF/Analog

![image](https://user-images.githubusercontent.com/84532205/142417064-06831232-f63c-4e67-a06c-b10248be6879.png)

```c++
void GPIO_init(GPIO_TypeDef *Port, int pin, uint32_t mode);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH

* **pin**:  pin number (int) 0~15

* **mode**: INPUT (0), OUTPUT (1),  ALTERNATE(2), ANALOG (3)

  

**Example code**

```c++
GPIO_init(GPIOA, 5, OUTPUT);
GPIO_init(GPIOC, 13, INPUT); //GPIO_init(GPIOC, 13, 0);
```



### GPIO_mode\(\)

Configures  GPIO pin modes: In/Out/AF/Analog

![image](https://user-images.githubusercontent.com/84532205/142417251-d0d066b5-a8b5-4705-bd15-af4c3dd92185.png)

```c++
void GPIO_mode(GPIO_TypeDef* Port, int pin, uint32_t mode);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH

* **pin**:  pin number (int) 0~15

* **mode**:   INPUT (0), OUTPUT (1),  ALTERNATE(2), ANALOG (3)

  

**Example code**

```c++
GPIO_mode(GPIOA, 5, OUTPUT);
```



### GPIO_pupdr\(\)

Configures  GPIO pin modes: NOPUPD/PU/PD/RESERVED

![image](https://user-images.githubusercontent.com/84532205/142417415-b8752252-bb29-4c55-ba05-977f2f7307ca.png)

```c++
void GPIO_pupdr(GPIO_TypeDef* Port, int pin, uint32_t pudr);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH

* **pin**:  pin number (int) 0~15

* **pupd**:   NOPUPD (0),  PU(1), PD(2), RESERVED(3)

  

**Example code**

```c++
GPIO_pupdr(GPIOA, 5, PU)
```



### GPIO_ospeed\(\)

Configures  GPIO pin modes: Low speed/Medium speed/Fast speed/High speed

![image](https://user-images.githubusercontent.com/84532205/142417455-1deafb31-2e1c-4c84-a2b4-4d8514178a22.png)

```c++
void GPIO_ospeed(GPIO_TypeDef* Port, int pin, uint32_t speed);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15
* **ospeed**:   LOW_SPEED(0), MEDIUM_SPEED(1), FAST_SPEED(2), HIGH_SPEED(3)



**Example code**

```c++
GPIO_ospeed(GPIOA, 5, 1);// 1 : Medium speed
```



### GPIO_otype\(\)

Configures  GPIO pin modes: Output push-pull/Output open-drain

![image](https://user-images.githubusercontent.com/84532205/142417487-50bede66-5a24-4033-babb-a42fb355b582.png)

```c++
void GPIO_otype(GPIO_TypeDef* Port, int pin, uint32_t type);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15
* **otype**:   OUTPUT_PP(0),  OUTPUT_OD(1)



**Example code**

```c++
GPIO_otype(GPIOA, 5, 0); // 0 : Output push-pull
```



### GPIO_write\(\)

Configures  GPIO pin modes: Low/High

![image](https://user-images.githubusercontent.com/84532205/142417514-fb74078c-0da7-49cc-94f0-b3e887e8bce6.png)

```c++
void GPIO_write(GPIO_TypeDef *Port, int pin, uint32_t Output);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15



**Example code**

```c++
GPIO_write(GPIOA, 5, LOW);
```



### GPIO_read(\)

Read the value of the pin

![image](https://user-images.githubusercontent.com/84532205/142417543-6b958237-c40a-4a9a-b0fd-6f3677dce56b.png)

```c++
uint32_t  GPIO_read(GPIO_TypeDef *Port, int pin);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15



**Example code**

```c++
if(GPIO_read(GPIOC, BUTTON_PIN) == 0){
    GPIO_write(GPIOA, LED_PA5, HIGH);
}
```



### ButtonST(\)

count how many times the button is pressed

```c++
uint32_t ButtonST(uint32_t value);
```

**Parameters**

* **value**:  button pressed times

  

**Example code**

```c++
button_nextstate = ButtonST(button_nextstate); //update variable button_nextstate
```



### sevensegment_init(\)

Initializes 7-segment GPIO pins with default setting and Enables GPIO Clock.

```c++
void sevensegment_init(void);
```

**Parameters**

* **void**

**Example code**

```c++
sevensegment_init();
```



### sevensegment_decoder(\)

Control each LED to display a number on the 7-segment LED

```c++
void sevensegment_decoder(uint32_t num);
```

**Parameters**

* **int:**  unsigned integer number (7-segment can represent a number 0 to 9)

    

**Example code**

```c++
sevensegment_decoder(0); //Display number '0' on 7-segment
```



## EXTI 

### Header File

 `#include "ecEXTI.h"`


```c++
#ifndef __EC_EXTI_H
#define __EC_EXTI_H

#include "stm32f411xe.h"

//EXTI interrupt
#define PA_PIN 0
#define PB_PIN 1
#define PC_PIN 2
#define PD_PIN 3
#define PE_PIN 4
#define PH_PIN 7

#define FALL 0
#define RISE 1
#define BOTH 2

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void EXTI_init(GPIO_TypeDef *Port, uint32_t pin, int trig,int priority);
void EXTI_clearpending(uint32_t pin);
void EXTI_enable(uint32_t pin);
void EXTI_disable(uint32_t pin);
uint32_t  is_pending_EXTI(uint32_t pin);
void clear_pending_EXTI(uint32_t pin);

#ifdef __cplusplus
}
#endif /* __cplusplus */
	 
#endif
```



### EXTI_init()

Initializes external interrupt pins  with default setting and Enables SYSCFG peripheral Clock.

![image](https://user-images.githubusercontent.com/84532205/142417623-c3077c89-ebb5-4e90-bc86-a284c91d1171.png)
![image](https://user-images.githubusercontent.com/84532205/142417640-a86228c5-eec8-4320-ac2d-d4a8ace171d5.png)
![image](https://user-images.githubusercontent.com/84532205/142417665-c471c289-7606-424d-94e8-5b6700a7d763.png)

```c++
void EXTI_init(GPIO_TypeDef *port, uint32_t pin, int trig_type, int priority);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15
* **Trig_type** : FALL(0), RISE(1), BOTH(2) 
* **priority** : interrupt priority

**Example code**

```c++
EXTI_init(GPIOC, BUTTON_PIN, FALL,0) //BUTTON_PIN = 13 , priority = 0
```



### EXTI_enable()

Enable external interrupt. unmasking corresponding bit in IMR register.

![image](https://user-images.githubusercontent.com/84532205/142417759-462e7f8d-9c0f-4b75-825b-00cae468e7bd.png)

```c++
void EXTI_enable(uint32_t pin);
```

**Parameters**

* **pin**:  pin number (int) 0~15

**Example code**

```c++
EXTI_enable(BUTTON_PIN) //BUTTON_PIN = 13
```



### EXTI_disable()

Disable external interrupt. masking corresponding bit in IMR register.

![image](https://user-images.githubusercontent.com/84532205/142417787-bca94a30-6a45-4ee6-9d43-1d72c97cce87.png)

```c++
void EXTI_disable(uint32_t pin);
```

**Parameters**

* **pin**:  pin number (int) 0~15

**Example code**

```c++
EXTI_disable(BUTTON_PIN) //BUTTON_PIN = 13
```



### is_pending_EXTI()

check the interrupt pending bit  to excute interrupt

![image](https://user-images.githubusercontent.com/84532205/142417815-a9f5991a-bcc3-4f8b-8987-c8df64dbb823.png)

```c++
uint32_t is_pending_EXTI(uint32_t pin);
```

**Parameters**

* **pin**:  pin number (int) 0~15

**Example code**

```c++
is_pending_EXTI(BUTTON_PIN) //BUTTON_PIN = 13
```



### clear_pending_EXTI()

clear pending bit when external interrupt is finished.

```c++
void clear_pending_EXTI(uint32_t pin);
```

**Parameters**

* **pin**:  pin number (int) 0~15

**Example code**

```c++
clear_pending_EXTI(BUTTON_PIN) //BUTTON_PIN = 13
```



## SysTick

### Header File

 `#include "ecSysTick.h"`


```c++
#ifndef __EC_ECSYSTICK_H
#define __EC_ECSYSTICK_H

#include "stm32f411xe.h"
#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void SysTick_init(uint32_t msec);
void delay_ms(uint32_t msec);
uint32_t SysTick_val(void);
void SysTick_reset (void);
void SysTick_enable(void);
void SysTick_disable (void);
void SysTick_Handler(void);
static volatile uint32_t TimeDelay = 0;
static volatile uint32_t EX = 0;
static volatile uint32_t EX1 = 0;
#ifdef __cplusplus
}
#endif /* __cplusplus */
	 
#endif
```



### SysTick_init()

Initializes  SysTick with default setting including reload counting value, interrupt priority, processor clock and so on.

![image](https://user-images.githubusercontent.com/84532205/142417971-f9eab263-14eb-408c-916b-a9075f69c734.png)
![image](https://user-images.githubusercontent.com/84532205/142417989-39c30efb-616b-472e-9941-bcc1d6db5a83.png)

```c++
void SysTick_init(uint32_t msec);
```

**Parameters**

* **msec**:  Clock frequency

**Example code**

```c++
SysTick_init(PLL_CLK) //PLL_CLK = 84000000 (84MHz)
```



### delay_ms()

Give a delay milliseconds.

```c++
void delay_ms(uint32_t msec);
```

**Parameters**

* **msec**:  delay time

**Example code**

```c++
delay_ms(1000) // down count 999 to 0
```



### SysTick_val()

Read and return the current value of the SysTick counter.

```c++
uint32_t SysTick_val(void);
```

**Parameters**

* **void**

**Example code**

```c++
Start_time = SysTick_val(); // read start count value
Stop_time = SysTick_val(); // read stop count value
```



### SysTick_reset()

Reset the count value to be zero.

```c++
void SysTick_reset (void);
```

**Parameters**

* **void**

**Example code**

```c++
SysTick_reset();
```



### SysTick_enable()

Enable the counter.

```c++
void SysTick_enable(void);
```

**Parameters**

* **void**

**Example code**

```c++
SysTick_enable();
```



### SysTick_disable()

Disable the counter.

```c++
void SysTick_disable(void);
```

**Parameters**

* **void**

**Example code**

```c++
SysTick_disable();
```



## API

### Header File

 `#include "EC_API.h"`


```c++
#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecSysTick.h"
#include "ecEXTI.h"

#ifndef __EC_GPIO_API_H
#define __EC_GPIO_API_H

#define EC_DOUT  	1
#define EC_DIN 		0

#define EC_PU 1
#define EC_PD 0
#define EC_NONE 0

#define EC_LOW 		0
#define EC_MEDIUM 1
#define EC_FAST 	2
#define EC_HIGH 	3

/* System CLOCK is HSI by default */
class EC_DigitalIn
{
public:
    EC_DigitalIn(GPIO_TypeDef *Port, int pin) 
    {
			uint8_t mode=EC_DIN; // mode=0
			GPIO_init(Port, pin, mode);
			Port_t=Port;
			pin_t=pin;
			mode_t=mode;	
    }

    ~EC_DigitalIn()
    {
			 delete[] Port_t;
    }

    int read();
		
		void pupdr(int _pupd);
    
    operator int()
    {
        return read();
    }

	private:
			GPIO_TypeDef *Port_t;
			int	pin_t;
			int mode_t;	
			int val_t;	
};

class EC_DigitalOut
{
public:
		EC_DigitalOut(GPIO_TypeDef *Port, int pin);
    ~EC_DigitalOut()
    {
			 delete[] Port_t;
    }
    void write(int _outVal);
		void pupdr(int _pupd);
		void otype(int _type);
		void ospeed(int _speed);
		
		EC_DigitalOut &operator= (int value)
    {
				 write(value);
				 return *this;
    }
		int read()
    {
				return GPIO_read(Port_t, pin_t);
    }
		operator int()
		{
		// Underlying call is thread safe
			return read();
		}

	private:  //only access in class 
			GPIO_TypeDef *Port_t;
			int	pin_t;  // _t -> internal 
			int mode_t;	
};


class EC_Ticker
{
public:
    EC_Ticker(uint32_t freq) 
    {
			frequency = freq * 84000000;
	  	SysTick_init(frequency);
		}
		void Delay_ms(uint32_t msec);
		uint32_t read_ms(void);
		void reset(void);
		
	private:
			uint32_t frequency ;
};

#endif
```



### EC_Ticker::Delay_ms()

Give a delay milliseconds.

```c++
void EC_Ticker::Delay_ms(uint32_t msec);
```

**Parameters**

* **msec** : delay time

**Example code**

```c++
EC_Ticker tick(1);
volatile uint32_t count = 0;

//Initialization	
void setup(void)
{
	RCC_PLL_init();
	sevensegment_init();
}

int main (void){
	setup();	
	while(1){
		sevensegment_decoder(count);
		tick.Delay_ms(1000);
		count++;
		if(count == 10)count =0;
		tick.reset();
	}
}
```



### EC_Ticker::read_ms()

Read and return the current value of the SysTick counter.

```c++
uint32_t EC_Ticker::read_ms(void);
```

**Parameters**

* **void**

**Example code**

```c++
Start_time = tick.read_ms(); // read start count value
Stop_time = tick.read_ms(); // read stop count value
```



### EC_Ticker::reset()

Reset the count value to be zero.

```c++
void EC_Ticker::reset(void);
```

**Parameters**

* **void**

**Example code**

```c++
tick.reset();
```



## TIMER

### Header File

 `#include "ecTIM.h"`


```c++
#ifndef __EC_TIM_H 
#define __EC_TIM_H
#include "stm32f411xe.h"
// Edge Type
#define RISE 0
#define FALL 1
#define BOTH 2

//Input Capture

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* Timer Configuration */
void TIM_init(TIM_TypeDef *timerx, uint32_t msec);  
void TIM_period_us(TIM_TypeDef* timx, uint32_t usec);  
void TIM_period_ms(TIM_TypeDef* timx, uint32_t msec);

void TIM_INT_init(TIM_TypeDef* timerx, uint32_t msec); 
void TIM_INT_enable(TIM_TypeDef* timx);
void TIM_INT_disable(TIM_TypeDef* timx);

uint32_t is_UIF(TIM_TypeDef *TIMx);
void clear_UIF(TIM_TypeDef *TIMx);

void TIM_INT_CCIE_enable(TIM_TypeDef* timerx);

/* Input Capture*/
typedef struct{
	GPIO_TypeDef *port;
	int pin;   
	TIM_TypeDef *timer;
	int ch;  		//int Timer Channel
	int ICnum;  //int IC number
} IC_t;

void ICAP_init(IC_t *ICx, GPIO_TypeDef *port, int pin);
void ICAP_setup(IC_t *ICx, int IC_number, int edge_type);
void ICAP_counter_us(IC_t *ICx, int usec);
void ICAP_pinmap(IC_t *timer_pin);

uint32_t is_CCIF(TIM_TypeDef *TIMx, uint32_t ccNum);
void clear_CCIF(TIM_TypeDef *TIMx, uint32_t ccNum);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
```



### TIM_init()

![image](https://user-images.githubusercontent.com/84532205/142418036-e94d13ff-0d57-4df2-9467-18bf3c4d821c.png)

```c++
void TIM_init(TIM_TypeDef *timerx, uint32_t msec);
```

**Parameters**

* timex : Timer number 
* integer : msec 

**Example code**

```c++
TIM_init(TIM2, 20); //Initialize Timer2 20ms
```



### TIM_period_us()

![image](https://user-images.githubusercontent.com/84532205/142418071-c2bbe06c-4ae3-4aa2-8e55-ee9abc918936.png)

```c++
void TIM_period_us(TIM_TypeDef* timx, uint32_t usec);
```

**Parameters**

* timex : Timer number 
* integer : usec 

**Example code**

```c++
TIM_TIM_period_us(TIM2, 1); //Initialize Timer2 1us
```



### TIM_period_ms()

![image](https://user-images.githubusercontent.com/84532205/142418095-75023c47-fd4f-4f14-bdb0-0db26e495d17.png)

```c++
void TIM_period_ms(TIM_TypeDef* timx, uint32_t msec);
```

**Parameters**

* timex : Timer number 
* integer : msec 

**Example code**

```c++
TIM_period_ms(TIM2, 20); //Initialize Timer2 20ms
```



### TIM_INT_init()

Initialize timerx and enable Update Event Interrupt.

```c++
void TIM_INT_init(TIM_TypeDef* timerx, uint32_t msec); 
```

**Parameters**

* timex : Timer number 
* integer : msec 

**Example code**

```c++
TIM_INT_init(TIM2, 20); //Initialize Timer2 20ms
```



### TIM_INT_enable()

Enable Timer Update Interrupt

![image](https://user-images.githubusercontent.com/84532205/142418135-62ed83a8-cfa5-4647-9dfa-ab62202ec1a4.png)

```c++
void TIM_INT_enable(TIM_TypeDef* timx);
```

**Parameters**

* timex : Timer number 

**Example code**

```c++
TIM_INT_init(TIM2); //enable Timer2 
```



### TIM_INT_disable()

disable Timer Update Interrupt

![image](https://user-images.githubusercontent.com/84532205/142418169-6ce5e73a-5718-489f-9c12-60062bc95220.png)

```c++
void TIM_INT_disable(TIM_TypeDef* timx);
```

**Parameters**

* timex : Timer number 

**Example code**

```c++
TIM_INT_disablet(TIM2, 20); //disable Timer2
```



### is_UIF()

check the update interrupt flag. No update occured or update interrupt pending.

![image](https://user-images.githubusercontent.com/84532205/142418194-222aea1d-cf11-4116-a171-77e5b6c82cf1.png)

```c++
uint32_t is_UIF(TIM_TypeDef *TIMx);
```

**Parameters**

* timex : Timer number

**Example code**

```c++
void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){// update interrupt flag
		_count++;
		if (_count >1000) {
			LED_toggle();
			_count=0;}
		clear_UIF(TIM2);// clear by writing 0
    }
}
```



### clear_UIF()

clear the update interrupt flag. No update occured.

![image](https://user-images.githubusercontent.com/84532205/142418241-715ddc69-1a71-4483-9b43-9e2012c5f6a5.png)

```c++
void clear_UIF(TIM_TypeDef *TIMx);
```

**Parameters**

* timex : Timer number

**Example code**

```c++
void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){// update interrupt flag
		_count++;
		if (_count >1000) {
			LED_toggle();
			_count=0;}
		clear_UIF(TIM2);// clear by writing 0
    }
}
```



### is_CCIF()

check the Capture/Compare interrupt flag. No update capture occured or caputre the counter value.

```c++
uint32_t is_CCIF(TIM_TypeDef *TIMx, uint32_t ccNum);
```

**Parameters**

* **TIMx** : Timer number
* **ccNum** : CCRx channel number

**Example code**

```c++
void TIM2_IRQHandler(void){
	if(is_CCIF(TIM2,3)){ 								// TIM2_Ch3 (IC3) Capture Flag. Rising Edge 
			timeSt = TIM2->CCR3;						// Capture TimeStart from CC3
		clear_CCIF(TIM2,3);              				// clear capture/compare interrupt flag 
	}	
}
```



### clear_CCIF()

clear the Capture/Compare interrupt flag. No update occured.

```c++
void clear_CCIF(TIM_TypeDef *TIMx, uint32_t ccNum);
```

**Parameters**

* **TIMx** : Timer number
* **ccNum** : CCRx channel number

**Example code**

```c++
void TIM2_IRQHandler(void){
	if(is_CCIF(TIM2,3)){ 								// TIM2_Ch3 (IC3) Capture Flag. Rising Edge 
			timeSt = TIM2->CCR3;						// Capture TimeStart from CC3
		clear_CCIF(TIM2,3);              				// clear capture/compare interrupt flag 
	}	
}
```



### ICAP_init() 

Initializes input capture mode  with default setting(GPIO port, pin number) and enables each timer Clock.

```c++
void ICAP_init(IC_t *ICx, GPIO_TypeDef *port, int pin);
```

**Parameters**

* **port** : Port Number,  GPIOA~GPIOH
* **pin** : pin number (int) 0~15

**Example code**

```c++
ICAP_init(&echo,GPIOB,10);  // ICAP init as PB10 as input caputre
```



### ICAP_setup()

Selecting TIx-ICy and Edge Type.

```c++
void ICAP_setup(IC_t *ICx, int IC_number, int edge_type);
```

**Parameters**

* **ICx** : Input capture typedef structure 
* **IC_number** : Input channer number
* **edge_type** : Rising or Falling or Both

**Example code**

```c++
ICAP_setup(&echo, 3, RISE);   	// TIM2_CH3 as IC3 , rising edge detect
ICAP_setup(&echo, 4, FALL);   	// TIM2_CH3 as IC4 , falling edge detect
```



### ICAP_counter_us()

Setting Timer x counter clock (microseconds).

```c++
void ICAP_counter_us(IC_t *ICx, int usec);
```

**Parameters**

* **ICx** : Input capture typedef structure 
* **usec** : Input channer number

**Example code**

```c++
ICAP_counter_us(&echo, 10);   	// ICAP counter step time as 10us
```



## PWM

### Header File

 `#include "ecPWM.h"`


```c++
#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecTIM.h"  			

#ifndef __EC_PWM_H
#define __EC_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
typedef struct{
   GPIO_TypeDef *port;
   int pin;
   TIM_TypeDef *timer;
   int ch;
} PWM_t;

/* PWM Configuration */
void PWM_init(PWM_t *pwm, GPIO_TypeDef *port, int pin);  
void PWM_period_ms(PWM_t *pwm,  uint32_t msec);		
void PWM_period_us(PWM_t *PWM_pin, uint32_t usec);  
void PWM_pulsewidth_ms(PWM_t *pwm, float pulse_width_ms);
void PWM_pulsewidth_us(PWM_t *pwm, float pulse_width_ms);
void PWM_duty(PWM_t *pwm, float duty);
void PWM_pinmap(PWM_t *PWM_pin);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
```



### PWM_init()

![image](https://user-images.githubusercontent.com/84532205/142418296-390414f7-32c0-45eb-bc62-7ae3e3a3e5c2.png)

```c++
void PWM_init(PWM_t *pwm, GPIO_TypeDef *port, int pin);  
```

**Parameters**

* pwm : typedef pwm structure 
* port : GPIOx port 
* pin : Output pin number

**Example code**

```c++
PWM_init(&pwm,GPIOA,PA1);
```



### PWM_period_ms()

```c++
void PWM_period_ms(PWM_t *pwm,  uint32_t msec); 
```

**Parameters**

* pwm : typedef pwm structure 
* integer : msec

**Example code**

```c++
PWM_period_ms(&pwm,20);
```



### PWM_period_us()

```c++
void PWM_period_us(PWM_t *PWM_pin, uint32_t usec);
```

**Parameters**

* pwm : typedef pwm structure 
* integer : usec

**Example code**

```c++
PWM_period_us(&pwm,20000);
```



### PWM_pulsewidth_ms()

```c++
void PWM_pulsewidth_ms(PWM_t *pwm, float pulse_width_ms);
```

**Parameters**

* pwm : typedef pwm structure 
* float : puelse width msec

**Example code**

```c++
PWM_pulsewidth_ms(&pwm,0.5);
```



### PWM_duty()

```c++
void PWM_duty(PWM_t *pwm, float duty);
```

**Parameters**

* pwm : typedef pwm structure 
* float : duty ratio

**Example code**

```c++
PWM_duty(&pwm,0.5/20); ratio of 0.5ms/20ms 
```



### PWM_pinmap()

![image](https://user-images.githubusercontent.com/84532205/142418436-90c249c5-0f1d-4ba1-8a4e-18b6c9dcc19f.png)

```c++
void PWM_pinmap(PWM_t *PWM_pin);
```

**Parameters**

* pwm_pin : pwm_pin structure has port and pin number 

**Example code**

```c++
PWM_pinmap(&pwm); Timer and channel are determined according to port and pin number.
```



## Stepper Motor

### Header File

 `#include "ecStepper.h"`

``` c++
#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecSysTick.h"
			
#ifndef __EC_STEPPER_H
#define __EC_STEPPER_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

//State mode
#define HALF 0
#define FULL 1	 

/* Stepper Motor */
//stepper motor function

typedef struct{
   GPIO_TypeDef *port1;
   int pin1;
	 GPIO_TypeDef *port2;
   int pin2;
	 GPIO_TypeDef *port3;
   int pin3;
	 GPIO_TypeDef *port4;
   int pin4;
	 int _step_num;
} Stepper_t;

	 
void Stepper_init(GPIO_TypeDef* port1, int pin1, GPIO_TypeDef* port2, int pin2, GPIO_TypeDef* port3, int pin3, GPIO_TypeDef* port4, int pin4);
void Stepper_setSpeed (long whatSpeed, int mode);
void Stepper_step(int steps, int direction, int mode); 
void Stepper_run (int direction, int mode); 
void Stepper_stop (void);
void Stepper_pinOut(uint32_t state, int mode);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
```



### Stepper_init()

Initializes Stepper port and pins with default setting including otype, ospeed, pupdr.

```c++
void Stepper_init(GPIO_TypeDef* port1, int pin1, GPIO_TypeDef* port2, int pin2, GPIO_TypeDef* port3, int pin3, GPIO_TypeDef* port4, int pin4);
```

**Parameters**

* **Port1:**  Port Number,  GPIOA~GPIOH
* **Port2:**  Port Number,  GPIOA~GPIOH
* **Port3:**  Port Number,  GPIOA~GPIOH
* **Port4:**  Port Number,  GPIOA~GPIOH
* **pin**1:  pin number (int) 0~15
* **pin**:  pin number (int) 0~15
* **pin3**:  pin number (int) 0~15
* **pin4**:  pin number (int) 0~15

**Example code**

```c++
Stepper_init(GPIOB,10,GPIOB,4,GPIOB,5,GPIOB,3); // Stepper GPIO pin initialization
```



### Stepper_setSpeed()

it can control stepper motor's speed. 

```c++
void Stepper_setSpeed (long whatSpeed, int mode);
```

**Parameters**

* wahtSpeed : RPM
* mode : FULL or Half mode 

**Example code**

```c++
Stepper_setSpeed(18,FULL);     //  set stepper motor speed
```



### Stepper_step()

Control the stepper motor according to the number of steps, direction, and mode.

```c++
void Stepper_step(int steps, int direction, int mode); 
```

**Parameters**

* step : step size determined by the user according to the mode.
* direction : CW(clockwise) , CCW(counterclockwise)
* mode : FULL or Half

**Example code**

```c++
Stepper_step(2048, 0, FULL);  // (Full Step : 2048, Half Step : 4096 Direction : 0 -> ++ or 1 -> --, Mode : FULL or HALF)
```



### Stepper_stop()

When button is pressed, stepper motor stops immediately. 

```c++
void Stepper_stop (void);
```

**Parameters**

* void

**Example code**

```c++
void EXTI15_10_IRQHandler(void) {  
	if (is_pending_EXTI(BUTTON_PIN)) {
		Stepper_stop();
		clear_pending_EXTI(BUTTON_PIN);
	}
}
```



### Stepper_pinOut()

The next state is determined according to the current state and mode.

```c++
void Stepper_pinOut(uint32_t state, int mode);
```

**Parameters**

* state : it represents present state number
* mode : FULL or Half

**Example code**

```c++
Stepper_pinOut(state_number, mode);
```



## ADC

### Header File

 `#include "ecADC.h"`

``` c++
#ifndef __MY_ADC_H
#define __MY_ADC_H
#include "stm32f411xe.h"

// ADC trigmode
#define SW 0
#define TRGO 1

// ADC contmode
#define CONT 0
#define SINGLE 1

// Edge Type
#define RISE 1
#define FALL 2
#define BOTH 3

#define _DEFAULT 0

// ADC setting

void ADC_start(void);
void ADC_init(GPIO_TypeDef *port, int pin, int mode);	// trigmode : SW , TRGO
void ADC_continue(int contmode); 						// contmode : CONT, SINGLE / Operate both ADC,JADC
void ADC_TRGO(TIM_TypeDef* TIMx, int msec, int edge);
void ADC_sequence(int length, int *seq); 

uint32_t is_ADC_EOC(void);
uint32_t is_ADC_OVR(void);
void clear_ADC_OVR(void);
uint32_t ADC_read(void);
uint32_t ADC_pinmap(GPIO_TypeDef *port, int pin);

#endif
```



### ADC_init()

Initializes GPIO port and pin number, Trigger mode.

```c++
void ADC_init(GPIO_TypeDef *port, int pin, int mode);	
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15
* **mode**:  trigmode : SW , TRGO

**Example code**

```c++
ADC_init(GPIOB,0,TRGO);
ADC_init(GPIOB,1,TRGO);
```



### ADC_continue()

Set the Single mode or Continuous mode

```c++
void ADC_continue(int contmode); 
```

**Parameters**

* **contmode**:  contmode : CONT, SINGLE

**Example code**

```c++
ADC_continue(SINGLE)
```



### ADC_TRGO()

Initializes TRGO with default setting.

```c++
void ADC_TRGO(TIM_TypeDef* TIMx, int msec, int edge);
```

**Parameters**

* **TIM**:  Select Timer 
* **msec**: period of timer
* **edge**: Select Trigger Polarity

**Example code**

```c++
ADC_TRGO(TIM3, 1, RISE);
```



### ADC_sequence()

Set the Sequence length and channel sequence.

```c++
void ADC_sequence(int length, int *seq);
```

**Parameters**

* **length**:  length of conversions 
* **seq**: the number of conversion sequence

**Example code**

```c++
static int seqCHn[2] = {8,9};	
ADC_sequence(2,seqCHn);
```



### ADC_start()

Software Trigger.

```c++
void ADC_start(void);
```

**Parameters**

* **void**

**Example code**

```c++
ADC_start();
```



### is_ADC_EOC()

check the conversion ends and read the data of register ADC.

```c++
uint32_t is_ADC_EOC(void);
```

**Parameters**

* **void**

**Example code**

```c++
if(is_ADC_EOC()){       //after finishing sequence
	if(flag==0){
		IR1 = ADC1->DR;			
	}else if(flag==1){
		IR2 = ADC1->DR;
	}
	flag =!flag;
}
```



### is_ADC_OVR()

Check the overrun is ocurred.

```c++
uint32_t is_ADC_OVR(void);
```

**Parameters**

* **void**

**Example code**

```c++
if(is_ADC_OVR()){
		clear_ADC_OVR();
	}
```



### clear_ADC_OVR()

Clear the overrun flag.

```c++
void clear_ADC_OVR(void);
```

**Parameters**

* **void**

**Example code**

```c++
if(is_ADC_OVR()){
		clear_ADC_OVR();
	}
```



### ADC_read()

Read the data register of ADC.

```c++
uint32_t ADC_read(void);
```

**Parameters**

* **void**

**Example code**

```c++
IR1 = ADC_read();		
```



### ADC_pinmap()

Match the channel number according to the port and pin number.

```c++
uint32_t ADC_pinmap(GPIO_TypeDef *Port, int Pin);
```

**Parameters**

* **port** : Port Number,  GPIOA~C
* **pin **: pin number (int) 0~15

**Example code**

```c++
channel = ADC_pinmap(GPIOA, 4); //	channel is 4 when GPIOA port and pin 4.
```

## USART

### Header File

 `#include "ecUSART.h"`

``` c++
#ifndef __EC_USART_H
#define __EC_USART_H

#include "stm32f411xe.h"
#include <stdio.h>
#include "ecGPIO.h"
#include "ecRCC.h"

#define POL 0
#define INT 1

// You can modify this
#define BAUD_9600		0
#define BAUD_19200	1
#define BAUD_57600	2
#define BAUD_115200 3
#define BAUD_921600 4


// ********************** USART 2 (USB) ***************************
	// PA_2 = USART2_TX
	// PA_3 = USART2_RX
	// Alternate function(AF7), High Speed, Push pull, Pull up
	// APB1
// **********************************************************

// ********************** USART 1 ***************************
	// PB_6 = USART1_TX (default)	// PA_9  (option)
	// PB_3 = USART1_RX (default)	// PA_10 (option)
	// APB2
// **********************************************************

// ********************** USART 6 ***************************
	// PA_11 = USART6_TX (default)	// PC_6  (option)
	// PA_12 = USART6_RX (default)	// PC_7 (option)
	// APB2
// **********************************************************


/* Given to Students */ 
void USART_write(USART_TypeDef* USARTx, uint8_t* buffer, uint32_t nBytes);
void USART_delay(uint32_t us);  

/* Exercise*/
void USART_begin(USART_TypeDef* USARTx, GPIO_TypeDef* GPIO_TX, int pinTX, GPIO_TypeDef* GPIO_RX, int pinRX, int baud);
void USART_init(USART_TypeDef* USARTx, int baud);  															// default pins. 
uint8_t USART_getc(USART_TypeDef * USARTx);			
uint32_t is_USART_RXNE(USART_TypeDef * USARTx);

#endif
```



### USART_write()

Transmit the data.  User can send the  input data between different usart communications.

```c++
void USART_write(USART_TypeDef* USARTx, uint8_t* buffer, uint32_t nBytes);
```

**Parameters**

* **USARTx:**  select USART nubmer
* **buffer**:  user input buffer
* **nBytes**:  TXE (TX empty) bit

**Example code**

```c++
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
```



### USART_begin()

Select the USART number and decide the corresponding GPIO port and pinnubmer of RX and TX Pin.  The baud-rate of usart communication is determined.

```c++
void USART_begin(USART_TypeDef* USARTx, GPIO_TypeDef* GPIO_TX, int pinTX, GPIO_TypeDef* GPIO_RX, int pinRX, int baud);
```

**Parameters**

* **USARTx:**  select USART nubmer
* **GPIO_TX:** Select a designated TX port according to USART.
* **pinTX:** Select a designated TX pin number  according to USART and GPIO_TX port.
* **GPIO_RX:** Select a designated RX port according to USART.
* **pinRX:** Select a designated RX pin number  according to USART and GPIO_RX port.
* **baud**: Select USART baud-rate

**Example code**

```c++
USART_begin(USART1, GPIOA,9,GPIOA,10, 9600); 	// PA9 - RXD , PA10 - TXD
```



### USART_init()

Initializes GPIO port and pin number with default setting in USART communication.

```c++
void USART_init(USART_TypeDef* USARTx, int baud);
```

**Parameters**

* **USARTx:**  select USART nubmer
* **baud**:   Select USART baud-rate

**Example code**

```c++
USART_init(USART2, 38400);
```



### USART_getc()

Read USART communication data.

```c++
uint8_t USART_getc(USART_TypeDef * USARTx);	
```

**Parameters**

* **USARTx:**  Select the USART number to read the data.

**Example code**

```c++
mcu2Data = USART_getc(USART1);
pcData = USART_getc(USART2);
```



### is_USART_RXNE()

Check the status of USART. Received data is ready to be read or Data is not received.

```c++
uint32_t is_USART_RXNE(USART_TypeDef * USARTx);
```

**Parameters**

* **USARTx:**  Select the USART number

**Example code**

```c++
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
```


