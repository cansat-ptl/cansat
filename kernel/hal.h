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

#endif /* HAL_H_ */