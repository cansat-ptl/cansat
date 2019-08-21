/*
 * uart.h
 *
 * Created: 08.03.2019 19:54:36
 *  Author: ThePetrovich
 */ 

#ifndef UART_H_
#define UART_H_

#define UARTDRV_VER "1.1.3"
#define UARTDRV_TIMESTAMP __TIMESTAMP__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdarg.h>
#include "../driver_config.h"

#define TX0BUSY 0
#define TX1BUSY 0
#define TX0_IE 1
#define TX1_IE 1
#define RX0_IE 2
#define RX1_IE 2

extern volatile char rx0_buffer[32];
extern volatile int rx0_pointer;
extern volatile char *tx0_data;
extern volatile int tx0_pointer;

extern volatile char rx1_buffer[128];
extern volatile int rx1_pointer;
extern volatile char *tx1_data;
extern volatile int tx1_pointer;

/*------------------------------------------------------------
UART0 setup - uart0_init(unsigned int ubrr)
Sets up UART0 registers
Arguments: unsigned int ubrr - UBRR value, calculated using
		    formula in drivers/uart.h. Defines UART0 baud rate.
Returns: int - 0 (additional checks may be added later)
------------------------------------------------------------*/
int uart0_init(unsigned int ubrr);

/*------------------------------------------------------------
UART0 interrupted TX start - uart0_transmit(unsigned int ubrr)
Transmits data stored in tx0_buffer using uart0 interrupts.
Data should be put in buffer BEFORE the transmission starts.
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void uart0_transmit();

/*------------------------------------------------------------
UART0 uninterrupted character TX - uart0_putc(char c)
Transmits a single character through uart0. Does not require
interrupts.
Arguments: char c - character to be sent
Returns: nothing
------------------------------------------------------------*/
void uart0_putc(char c);

/*------------------------------------------------------------
UART0 uninterrupted string TX - uart0_puts(char * msg)
Transmits a string through uart0. Does not require
interrupts.
Arguments: char * msg - string pointer
Returns: nothing
------------------------------------------------------------*/
void uart0_puts(char * msg);

/*------------------------------------------------------------
UART0 receive buffer flush - rx0_buffer_flush()
Clears the uart0 receive buffer (rx0_buffer).
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void rx0_buffer_flush();

/*------------------------------------------------------------
UART0 transmit buffer flush - tx0_buffer_flush()
Clears the uart0 transmit buffer (rx0_buffer).
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void tx0_buffer_flush();

void rx0_enableInterrupt();
void rx0_disableInterrupt();
void tx0_enableInterrupt();
void tx0_disableInterrupt();

/*------------------------------------------------------------
UART1 setup - uart1_init(unsigned int ubrr)
Sets up UART1 registers
Arguments: unsigned int ubrr - UBRR value, calculated using
		    formula in drivers/uart.h. Defines UART0 baud rate.
Returns: int - 0 (additional checks may be added later)
------------------------------------------------------------*/
int uart1_init(unsigned int ubrr);

/*------------------------------------------------------------
UART1 interrupted TX start - uart1_transmit(unsigned int ubrr)
Transmits data stored in tx1_buffer using uart1 interrupts.
Data should be put in buffer BEFORE the transmission starts.
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void uart1_transmit();

/*------------------------------------------------------------
UART1 receive buffer flush - rx1_buffer_flush()
Clears the uart1 receive buffer (rx1_buffer).
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void rx1_buffer_flush();

/*------------------------------------------------------------
UART1 transmit buffer flush - tx1_buffer_flush()
Clears the uart1 transmit buffer (rx1_buffer).
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void tx1_buffer_flush();

void rx1_enableInterrupt();
void rx1_disableInterrupt();
void tx1_enableInterrupt();
void tx1_disableInterrupt();

#endif /* UART_H_ */