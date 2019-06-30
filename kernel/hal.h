/*
 * hal.h
 *
 * Created: 25.05.2019 20:44:34
 *  Author: ThePetrovich
 */ 

#ifndef HAL_H_
#define HAL_H_

#ifndef F_CPU
#define F_CPU 8000000L
#endif

#define HAL_MOD_VER "0.1.2-staging"
#define HAL_MOD_TIMESTAMP __TIMESTAMP__

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

#define LED_WRK_PORT PORTC
#define LED_WRK_DDR DDRC
#define LED_WRK PC4

#define LED_KRN_PORT PORTC
#define LED_KRN_DDR DDRC
#define LED_KRN PC5

#define LED_TX_PORT PORTC
#define LED_TX_DDR DDRC
#define LED_TX PC6

#define LED_DBG_PORT PORTC
#define LED_DBG_DDR DDRC
#define LED_DBG PC7

#define HIGH 1
#define LOW 0
#define OUTPUT 1
#define INPUT 0

#define hal_disableInterrupts() cli()
#define hal_enableInterrupts() sei()
#define hal_statusReg SREG
#define hal_nop() asm volatile ("NOP")
#define delay(x) _delay_ms(x);

#define hal_setBit_m(x,y) x |= (1 << y)
#define hal_clearBit_m(x,y) x &= ~(1 << y)
#define hal_checkBit_m(x,y) ((0u == (x & (1 << y))) ? 0u : 1u)
#define hal_switchBit_m(x,y) (x ^= (1 << y))

inline void hal_switchBit(volatile uint8_t *reg, uint8_t bit);
inline void hal_setBit(volatile uint8_t *reg, uint8_t bit);
inline void hal_clearBit(volatile uint8_t *reg, uint8_t bit);
inline volatile uint8_t hal_checkBit(volatile uint8_t *reg, uint8_t bit);
inline void hal_writePin(volatile uint8_t *port, uint8_t pin, uint8_t value);
inline volatile uint8_t hal_readPin(volatile uint8_t *port, uint8_t pin);
inline void hal_setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t value);

inline void hal_switchBit(volatile uint8_t *reg, uint8_t bit){
	*reg ^= (1 << bit);
}

inline void hal_setBit(volatile uint8_t *reg, uint8_t bit){
	*reg |= (1 << bit);
}

inline void hal_clearBit(volatile uint8_t *reg, uint8_t bit){
	*reg &= ~(1 << bit);
}

inline volatile uint8_t hal_checkBit(volatile uint8_t *reg, uint8_t bit){
	return (*reg >> bit) & 1;
}

inline void hal_writePin(volatile uint8_t *port, uint8_t pin, uint8_t value){
	uint8_t nvalue = !!value;
	*port ^= (-1 * nvalue ^ *port) & (1 << pin);
}
//Actually does the same thing as checkBit(), but i'll keep it here just in case
inline volatile uint8_t hal_readPin(volatile uint8_t *port, uint8_t pin){
	return (*port >> pin) & 1;
}

inline void hal_setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t value){
	uint8_t nvalue = !!value;
	*ddr ^= (-1 * nvalue ^ *ddr) & (1 << pin);
}

void hal_setupPins();
#endif /* HAL_H_ */