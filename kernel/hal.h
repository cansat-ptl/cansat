/*
 * hal.h
 *
 * Created: 25.05.2019 20:44:34
 *  Author: ThePetrovich
 */ 

#ifndef HAL_H_
#define HAL_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>

#define JUMPER_PORT PORTA
#define JUMPER_DDR DDRA
#define JUMPER_PIN PINA
#define JUMPER_IN PA0
#define JUMPER_OUT PA1

#define LED_BUILTIN_PORT PORTG
#define LED_BUILTIN_DDR DDRG
#define LED_BUILTIN PG3

#define LED_WRK_PORT PORTA
#define LED_WRK_DDR DDRA
#define LED_WRK PA4

#define LED_KRN_PORT PORTA
#define LED_KRN_DDR DDRA
#define LED_KRN PA5

#define LED_TX_PORT PORTA
#define LED_TX_DDR DDRA
#define LED_TX PA6

#define LED_DBG_PORT PORTA
#define LED_DBG_DDR DDRA
#define LED_DBG PA7

#define HIGH 1
#define LOW 0
#define OUTPUT 1
#define INPUT 0

#define disableInterrupts() cli()
#define enableInterrupts() sei()
#define statusReg SREG
#define nop() asm volatile ("NOP")
#define delay(x) _delay_ms(x);

#define setBit_m(x,y) x |= (1 << y)
#define clearBit_m(x,y) x &= ~(1<< y)
#define checkBit_m(x,y) ((0u == (x & (1<<y)))?0u:1u)
#define switchBit_m(x,y) (x ^= (1<<y))

inline void switchBit(volatile uint8_t *reg, uint8_t bit);
inline void setBit(volatile uint8_t *reg, uint8_t bit);
inline void clearBit(volatile uint8_t *reg, uint8_t bit);
inline volatile uint8_t checkBit(volatile uint8_t *reg, uint8_t bit);
inline void writePin(volatile uint8_t *port, uint8_t pin, uint8_t value);
inline volatile uint8_t readPin(volatile uint8_t *port, uint8_t pin);
inline void setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t value);

inline void switchBit(volatile uint8_t *reg, uint8_t bit){
	*reg ^= (1 << bit);
}

inline void setBit(volatile uint8_t *reg, uint8_t bit){
	*reg ^= (1 << bit);
}

inline void clearBit(volatile uint8_t *reg, uint8_t bit){
	*reg &= ~(1 << bit);
}

inline volatile uint8_t checkBit(volatile uint8_t *reg, uint8_t bit){
	return (*reg >> bit) & 1;
}

inline void writePin(volatile uint8_t *port, uint8_t pin, uint8_t value){
	uint8_t nvalue = !!value;
	*port ^= (-1 * nvalue ^ *port) & (1 << pin);
}
//Actually does the same thing as checkBit(), but i'll keep it here just in case
inline volatile uint8_t readPin(volatile uint8_t *port, uint8_t pin){
	return (*port >> pin) & 1;
}

inline void setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t value){
	uint8_t nvalue = !!value;
	*ddr ^= (-1 * nvalue ^ *ddr) & (1 << pin);
}

void setupPins();
#endif /* HAL_H_ */