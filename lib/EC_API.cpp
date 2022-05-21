#include "EC_API.h"

/* System CLOCK is HSI by default */
int EC_DigitalIn::read()
{
	val_t = GPIO_read(Port_t, pin_t);
	return val_t;
}

void EC_DigitalIn::pupdr(int _pupd) {
	GPIO_pupdr(Port_t, pin_t, _pupd);
}

void EC_DigitalOut::write(int _outVal)
{
	GPIO_write(Port_t, pin_t, _outVal);
}
void EC_DigitalOut::pupdr(int _pupd){
	GPIO_pupdr(Port_t, pin_t, _pupd);
}

void EC_DigitalOut::otype(int _type){
	GPIO_otype(Port_t, pin_t, _type);
}
void EC_DigitalOut::ospeed(int _speed){
	GPIO_ospeed(Port_t, pin_t, _speed);
}
void EC_Ticker::Delay_ms(uint32_t msec){
	delay_ms(msec);
}
uint32_t EC_Ticker::read_ms(void){
	return SysTick->VAL &= 0xffffff;
}
void EC_Ticker::reset(void){
	SysTick_reset();
}

