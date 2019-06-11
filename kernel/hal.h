/*
 * hal.h
 *
 * Created: 25.05.2019 20:44:34
 *  Author: ThePetrovich
 */ 


#ifndef HAL_H_
#define HAL_H_

#include <avr/io.h>
#include <stdint.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>

#define disableInterrupts() cli()
#define enableInterrupts() sei()
#define statusReg SREG
#define nop() asm volatile ("NOP")

inline void switchBit(uint8_t *reg, uint8_t bit);
inline void setBit(uint8_t *reg, uint8_t bit);
inline void clearBit(uint8_t *reg, uint8_t bit);
inline volatile uint8_t checkBit(uint8_t *reg, uint8_t bit);
inline void writePin(uint8_t *port, uint8_t pin, uint8_t value);
inline volatile uint8_t readPin(uint8_t *port, uint8_t pin);

inline void switchBit(uint8_t *reg, uint8_t bit){
	*reg ^= (1 << bit);
}

inline void setBit(uint8_t *reg, uint8_t bit){
	*reg ^= (1 << bit);
}

inline void clearBit(uint8_t *reg, uint8_t bit){
	*reg &= ~(1 << bit);
}

inline volatile uint8_t checkBit(uint8_t *reg, uint8_t bit){
	return (*reg >> bit) & 1;
}

inline void writePin(uint8_t *port, uint8_t pin, uint8_t value){
	uint8_t nvalue = !!value;
	*port ^= (-1 * nvalue ^ *port) & (1 << pin);
}
//Actually does the same thing as checkBit(), but i'll keep it here just in case
inline volatile uint8_t readPin(uint8_t *port, uint8_t pin){
	return (*port >> pin) & 1;
}

#endif /* HAL_H_ */