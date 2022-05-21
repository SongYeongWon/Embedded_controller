#include "stm32f411xe.h"
#include "ecSysTick.h"
#include "ecADC.h"
#include "ecGPIO.h"
#include "ecTIM.h"
#include <stdint.h>


uint32_t result;

void ADC_init(GPIO_TypeDef *port, int pin, int trigmode){  //mode 0 : SW, 1 : TRGO
// 0. Match Port and Pin for ADC channel   
   int CHn = (uint32_t)ADC_pinmap(port, pin);         // ADC Channel <->Port/Pin mapping

// GPIO configuration ---------------------------------------------------------------------   
// 1. Initialize GPIO port and pin as ANALOG, no pull up / pull down
   GPIO_init(port, (uint32_t)pin, ANALOG);              // ANALOG = 3
   GPIO_pupdr(port, (uint32_t)pin, NO_PUPD);           // EC_NONE = 0

// ADC configuration   ---------------------------------------------------------------------         
// 1. Total time of conversion setting
   // Enable ADC pheripheral clock
   RCC->APB2ENR  |= RCC_APB2ENR_ADC1EN;       // Enable the clock of RCC_APB2ENR_ADC1EN
   
   // Configure ADC clock pre-scaler
   ADC->CCR &= ~(3UL<<16);               // 11: PCLK2 divided by 8   (21MHz)
   ADC->CCR |= 0UL<<16;
   
   // Configure ADC resolution 
   ADC1->CR1 &= ~(3UL<<24);           // 00: 12-bit resolution (15cycle+)
   
   // Configure channel sampling time of conversion.   
   // Software is allowed to write these bits only when ADSTART=0 and JADSTART=0   !!
   // ADC clock cycles @42MHz = 2us
   if(CHn < 10){ 
      ADC1->SMPR2  &= ~(7U<<3*CHn);            // sampling time conversion : 84
      ADC1->SMPR2  |= 4U<<3*CHn;               // sampling time conversion : 84           
   }
   else{
      ADC1->SMPR1  &= ~(7U<<3*(CHn%10));      
      ADC1->SMPR1  |= 4U<<3*(CHn%10); // how about change one line code?
   }
// 2. Regular / Injection Group 
   //Regular: SQRx, Injection: JSQx

// 3. Repetition: Single or Continuous conversion
   ADC1->CR2 &= ~(1UL<<1);                     // **default : Single conversion mode

// 4. Single Channel or Scan mode
   //  - Single Channel: scan mode, right alignment
   ADC1->CR1 |= 1UL<<8;                        // 1: Scan mode enable, more than 2 chs 
   ADC1->CR2 &= ~(1UL<<11); // 0: Right alignment, only 1 ch
   
   // Configure the sequence length
   ADC1->SQR1 &= ~(15UL<<20);                // 0000: 1 conversion in the regular channel conversion sequence
   
   // Configure the channel sequence 
   ADC1->SQR3 &= ~(0x1F<<0);                   // SQ1 clear bits
   ADC1->SQR3 |= (CHn & 0x1F);             // Choose the channel to convert firstly
      
   
// 5. Interrupt Enable
   // Enable EOC(conversion) interrupt. 
   ADC1->CR1 &= ~(1UL<<5);                // Interrupt reset
   ADC1->CR1 |= 1UL<<5;                    // Interrupt enable
   
   // Enable ADC_IRQn 
   NVIC_SetPriority(ADC_IRQn, 1);          // Set Priority to 2
   NVIC_EnableIRQ(ADC_IRQn);               // Enable interrupt form ACD1 peripheral   




/* -------------------------------------------------------------------------------------*/
//               HW TRIGGER MODE
/* -------------------------------------------------------------------------------------*/
   
   // TRGO Initialize : TIM3, 1msec, RISE edge
   if(trigmode==TRGO) ADC_TRGO(TIM3, 1, ADC_RISE);            
   
}

void ADC_INJ_init(GPIO_TypeDef *port, int pin, int trigmode){  //mode 0 : SW, 1 : TRGO
// 0. Match Port and Pin for ADC channel   
   int CHn = (uint32_t)ADC_pinmap(port, pin);         // ADC Channel <->Port/Pin mapping

// GPIO configuration ---------------------------------------------------------------------   
// 1. Initialize GPIO port and pin as ANALOG, no pull up / pull down
   GPIO_init(port, (uint32_t)pin, ANALOG);              // ANALOG = 3
   GPIO_pupdr(port, (uint32_t)pin, NO_PUPD);           // EC_NONE = 0

// ADC configuration   ---------------------------------------------------------------------         
// 1. Total time of conversion setting
   // Enable ADC pheripheral clock
   RCC->APB2ENR  |= 1UL<<8;       // Enable the clock of RCC_APB2ENR_ADC1EN, ADC1ENABLE
   
   // Configure ADC clock pre-scaler
   ADC->CCR &= ~(3UL<<16);               
   ADC->CCR |= 3UL<<16;            // 11: PCLK2 divided by 8   (21MHz)
   
   // Configure ADC resolution 
   ADC1->CR1 &= ~(3UL<<24);           // 00: 12-bit resolution (15cycle+)
   
   // Configure channel sampling time of conversion.   
   // Software is allowed to write these bits only when ADSTART=0 and JADSTART=0   !!
   // ADC clock cycles @42MHz = 2us
   if(CHn < 10){ 
      ADC1->SMPR2  &= ~(7U<<3*CHn);         // sampling time conversion : 84
      ADC1->SMPR2  |= 4U<<3*CHn;            // sampling time conversion : 84           
   }
   else{
      ADC1->SMPR1  &= ~(7U<<3*(CHn%10));      
      ADC1->SMPR1  |= 4U<<3*(CHn%10);       // how about change one line code?
   }
// 2. Regular / Injection Group 
//Regular: SQRx, Injection: JSQx

// 3. Repetition: Single or Continuous conversion
   ADC1->CR2 &= ~(1UL<<1);                     // **default : Single conversion mode

// 4. Single Channel or Scan mode
   //  - Single Channel: scan mode, right alignment
   ADC1->CR1 |= 1UL<<8;                           // 1: Scan mode enable, more than 2 chs 
   ADC1->CR2 &= ~(1UL<<11);                      // 0: Right alignment, only 1 ch
   
   // Configure the sequence length
   ADC1->JSQR &= ~(3UL<<20);                   // 0000: 1 conversion in the injected channel conversion sequence
   
   // Configure the channel sequence 
   ADC1->JSQR &= ~(0x1F<<0);                   //    SQ1 clear bits
   ADC1->JSQR |= (CHn & 0x1F);             //    Choose the channel to convert firstly
                                                         //   CHn should be 1~4      
   
// 5. Interrupt Enable

   // Enable JEOC(conversion) interrupt. 
   ADC1->CR1 &= ~(1UL<<7);                // Interrupt reset
   ADC1->CR1 |= 1UL<<7;                    // Interrupt enable
   
   // Enable ADC_IRQn 
   NVIC_SetPriority(ADC_IRQn, 3);          // Set Priority to 2
   NVIC_EnableIRQ(ADC_IRQn);               // Enable interrupt form ACD1 peripheral   

/* -------------------------------------------------------------------------------------*/
//               HW TRIGGER MODE
/* -------------------------------------------------------------------------------------*/
   
   // TRGO Initialize : TIM3, 1msec, RISE edge
   if(trigmode==TRGO) ADC_INJ_TRGO(TIM4, 100, ADC_RISE);            
   
}

void ADC_TRGO(TIM_TypeDef* TIMx, int msec, int edge){
   // set timer
   int timer = 0;
   if(TIMx==TIM2)          timer=2;
   else if(TIMx==TIM3) timer=3;   
   
   // Single conversion mode (disable continuous conversion)
   ADC1->CR2 &= ~(1UL<<1);        // Discontinuous conversion mode(need clear first)
   ADC1->CR2 |= 1UL<<10;              // Enable EOCS, Overrun detection enabled
   

   // HW Trigger configuration -------------------------------------------------------------
   
// 1. TIMx Trigger Output Config
   // Enable TIMx Clock
   TIM_init(TIMx, msec);
   TIMx->CR1 &= ~(1UL<<0);                //counter disable(to change setting)
   
   // Set PSC, ARR
  TIM_period_ms(TIMx, msec);                //(no need)
   
  // Master Mode Selection MMS[2:0]: Trigger output (TRGO)
  TIMx->CR2 &= ~(7UL<<4);                // reset MMS
  TIMx->CR2 |= 4UL<<4;                  // 100: Compare - OC1REF signal is used as trigger output (TRGO)
   
   // Output Compare Mode
  TIMx->CCMR1 &= ~(7UL<<4);                // OC1M : output compare 1 Mode 
  TIMx->CCMR1 |= 6UL<<4;                   // OC1M = 110 for compare 1 Mode ch1 
   
  // OC1 signal 
  TIMx->CCER |= 1UL<<0;                   // CC1E Capture enabled
   TIMx->CCR1  = (TIMx->ARR)/2;             // duty ratio 50%
   
  // Enable TIMx 
  TIMx->CR1 |= 1UL<<0;                         //counter enable

// 2. ADC HW Trigger Config.
   // Select Trigger Source           
   ADC1->CR2 &= ~ADC_CR2_EXTSEL;          // reset EXTSEL
   ADC1->CR2 |= (timer*2+2) << 24;          // TIMx TRGO event (ADC : TIM2, TIM3 TRGO)
   
   //Select Trigger Polarity
   ADC1->CR2 &= ~ADC_CR2_EXTEN;            // reset EXTEN, default
   if(edge==ADC_RISE)          ADC1->CR2 |= ADC_CR2_EXTEN_0;      // trigger detection rising edge
   else if(edge==ADC_FALL) ADC1->CR2 |= ADC_CR2_EXTEN_1;      // trigger detection falling edge
   else if(edge==ADC_BOTH) ADC1->CR2 |= ADC_CR2_EXTEN_Msk;   // trigger detection both edge

}

void ADC_INJ_TRGO(TIM_TypeDef* TIMx, int msec, int edge){
   // set timer
   int timer = 0;
   if(TIMx==TIM1)          timer   =   1;
   else if(TIMx==TIM2) timer   =   2;   
   else if(TIMx==TIM4) timer   =   4;
   else if(TIMx==TIM5) timer   =   5;
   
   // Single conversion mode (disable continuous conversion)
   ADC1->CR2 |= 1UL<<1;        // Discontinuous conversion mode(need clear first)
   ADC1->CR2 |= 1UL<<10;           // Enable EOCS
   

   // HW Trigger configuration -------------------------------------------------------------
   
// 1. TIMx Trigger Output Config
   // Enable TIMx Clock
   TIM_init(TIMx, msec);
   TIMx->CR1 &= ~(1UL<<0);                //counter disable(to change setting)
   
   // Set PSC, ARR
  //TIM_period_ms(TIMx, msec);                //(no need)
   
  // Master Mode Selection MMS[2:0]: Trigger output (TRGO)
  TIMx->CR2 &= ~(7UL<<4);                // reset MMS
  TIMx->CR2 |= 4UL<<4;                  // 100: Compare - OC1REF signal is used as trigger output (TRGO)
   
   // Output Compare Mode
  TIMx->CCMR1 &= ~(7UL<<4);                // OC1M : output compare 1 Mode 
  TIMx->CCMR1 |= 6UL<<4;                   // OC1M = 110 for compare 1 Mode ch1 
   
  // OC1 signal 
  TIMx->CCER |= 1UL<<0;                   // CC1E Capture enabled
   TIMx->CCR1  = (TIMx->ARR)/2;             // duty ratio 50%
   
  // Enable TIMx 
  TIMx->CR1 |= 1UL<<0;                         //counter enable

// 2. ADC HW Trigger Config.
   // Select Trigger Source           
   ADC1->CR2 &= ~ADC_CR2_JEXTSEL;          // reset JEXTSEL
   
   if(TIMx==TIM1)          ADC1->CR2 |= 1UL<<16;    // TIM1 TRGO event
   else if(TIMx==TIM2) ADC1->CR2 |= 3UL<<16;    // TIM2 TRGO event
   else if(TIMx==TIM4) ADC1->CR2 |= 9UL<<16;    // TIM4 TRGO event
   else if(TIMx==TIM5) ADC1->CR2 |= 11UL<<16;   // TIM5 TRGO event
   
   //Select Trigger Polarity
   ADC1->CR2 &= ~ADC_CR2_JEXTEN;                        // reset JEXTEN, default
   if(edge==ADC_RISE)          ADC1->CR2 |= 1UL<<20;      // trigger detection rising edge
   else if(edge==ADC_FALL) ADC1->CR2 |= 2UL<<20;      // trigger detection falling edge
   else if(edge==ADC_BOTH) ADC1->CR2 |= 3UL<<20;      // trigger detection both edge

}

void ADC_continue(int contmode){
   if(contmode==CONT){
      // Repetition: Continuous conversion
      ADC1->CR2 |= 1UL<<1;                  // Enable Continuous conversion mode   
      ADC1->CR1 &= ~ADC_CR1_SCAN;            // 0: Scan mode disable 
   }
   else                                                 //if(contmode==SINGLE)
      {
      // Repetition: Single conversion
      ADC1->CR2 &= ~ADC_CR2_CONT;         // Disable Continuous conversion mode   
      ADC1->CR1 |= ADC_CR1_SCAN;            // 1: Scan mode enable
   }
} 

void ADC_sequence(int length, int *seq){
   //length : how many use  // seq = channel that you want to use, ex) seq[2] = {3, 5};
   ADC1->SQR1 &= ~(0xF<<20);                   // reset length of conversions in the regular channel    
   ADC1->SQR1 |= (length-1)<<20;             // conversions in the regular channel conversion sequence
   
   for(int i = 0; i<length; i++){
      if (i<6){
         ADC1->SQR3 &= ~(0x1F<<i*5);            // SQn clear bits
         ADC1->SQR3 |= seq[i]<<i*5;            // Choose the channel to convert sequence
      }
      else if (i <12){
         ADC1->SQR2 &= ~(0x1F<<(i-6)*5);               // SQn clear bits
         ADC1->SQR2 |= seq[i]<<(i-6)*5;               // Choose the channel to convert sequence
      }
      else{
         ADC1->SQR1 &= ~(0x1F<<(i-12)*5);   // SQn clear bits
         ADC1->SQR1 |= seq[i]<<(i-12)*5;      // Choose the channel to convert sequence
      }
   }
}

void ADC_INJ_sequence(int length, int *seq){
   //length : how many use  // seq = channel that you want to use, ex) seq[2] = {3, 5};
   ADC1->JSQR &= ~(3UL<<20);       // reset length of conversions in the injected channel    
   ADC1->JSQR |= (length-1)<<20;   // conversions in the injected channel conversion sequence
   
   for(int i = 0; i<length; i++){
      ADC1->JSQR &= ~(0x1FUL<<((4-i)*5));   // Choose the channel to convert sequence
      ADC1->JSQR |= seq[i]<<((4-i)*5);   // Choose the channel to convert sequence
   }
   
   //ADC1->JSQR |= ((uint32_t)seq[0]<<0)|((uint32_t)seq[1]<<5);   // Choose the channel to convert sequence
}

void ADC_start(void){
   // Enable ADON, SW Trigger-------------------------------------------------------------------------------
   // Enable ADC 
   ADC1->CR2 |= ADC_CR2_ADON;   
   // ADC Start (SW, once)     
   ADC1->CR2 |= ADC_CR2_SWSTART;
   
}

void ADC_INJ_start(void){
   // Enable ADON, SW Trigger-------------------------------------------------------------------------------
   // Enable ADC 
   ADC1->CR2 |= ADC_CR2_ADON;   
   // ADC INJ Start (SW, once)     
   ADC1->CR2 |= ADC_CR2_JSWSTART;
   
}

uint32_t is_ADC_EOC(void){
   return ADC1->SR & ADC_SR_EOC;
}

uint32_t is_ADC_JEOC(void){
   return ADC1->SR & ADC_SR_JEOC;
}

void clear_ADC_JEOC(void){
   ADC1->SR &= ~ADC_SR_JEOC;
}

uint32_t is_ADC_OVR(void){
   return ADC1->SR & ADC_SR_OVR;
}

void clear_ADC_OVR(void){
   ADC1->SR &= ~ADC_SR_OVR;
}

uint32_t ADC_read(){
   return ADC1->DR;
}

uint32_t ADC_pinmap(GPIO_TypeDef *Port, int Pin){
   if(Port == GPIOA){
      if(Pin == 0)          return 0;
      else if(Pin == 1) return 1;
      else if(Pin == 4) return 4;
      else if(Pin == 5) return 5;
      else if(Pin == 6) return 6;
      else if(Pin == 7) return 7;
      else                      while(1);
   }
   else if(Port == GPIOB){
      if(Pin == 0)          return 8;
      else if(Pin == 1)   return 9;
      else                      while(1);
   }
   else if(Port == GPIOC){
      if(Pin == 0)         return 10;
      else if(Pin == 1)   return 11;
      else if(Pin == 2)   return 12;
      else if(Pin == 3)   return 13;
      else if(Pin == 4)   return 14;
      else if(Pin == 5)   return 15;
      else                     while(1);
   }
}