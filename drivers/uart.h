/*
 * uart.h
 *
 * Created: 08.03.2019 19:54:36
 *  Author: ThePetrovich
 */ 

#ifndef UART_H_
#define UART_H_

#define UARTDRV_VER "1.1.0"
#define UARTDRV_TIMESTAMP __TIMESTAMP__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdarg.h>

#define TX0BUSY 0
#define TX1BUSY 0

#ifndef F_CPU
//#warning "F_CPU is not defined until uart.h"
#define F_CPU 8000000L
#endif

#ifndef UARTconfig
#define BAUD 9600							//UART baud rate
#define UBRRVAL ((F_CPU/(16*BAUD)) - 1)		//UBBR value
#define RX0EN 1								//RX0 enable
#define TX0EN 1								//TX0 enable
#define RX0IE 1								//RX0 byte received interrupt enable
#define TX0IE 0								//TX0 byte transmit interrupt enable
#define RX1EN 1								//RX0 enable
#define TX1EN 1								//TX0 enable
#define RX1IE 1								//RX0 byte received interrupt enable
#define TX1IE 0								//TX0 byte transmit interrupt enable
#endif

extern volatile char rx0_buffer[32];
extern volatile int rx0_pointer;
extern volatile char *tx0_data;
extern volatile int tx0_pointer;

extern volatile char rx1_buffer[128];
extern volatile int rx1_pointer;
extern volatile char *tx1_data;
extern volatile int tx1_pointer;

int uart0_init(unsigned int ubrr);
void uart0_transmit();
void uart0_putc(char c); //Does not use interrupts
void uart0_puts(char * msg); //Does not use interrupts
void uart0_printf(char * format, ...); //Величайший велосипед человечества
void rx0_buffer_flush();
void tx0_buffer_flush();

int uart1_init(unsigned int ubrr);
void uart1_transmit();
void rx1_buffer_flush();
void tx1_buffer_flush();

#endif /* UART_H_ */