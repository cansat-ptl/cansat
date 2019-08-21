/*
 * uart.c
 *
 * Created: 07.03.2019 20:22:02
 *  Author: ThePetrovich
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "../uart.h"

#if UART0_USE_EXT_RX_ISR == 0
	volatile char rx0_buffer[32] = "";
	volatile int rx0_pointer = 0;
#endif
//#if UART0_USE_EXT_TX_ISR == 1
	volatile char tx0_buffer[128] = "";
	volatile int tx0_pointer = 0;
//#endif
volatile char *tx0_data;
volatile int tx0_size = 0;

volatile unsigned char creg0 = 0;
volatile unsigned char creg1 = 0;

#if UART1_USE_EXT_RX_ISR == 0
	volatile char rx1_buffer[128] = "";
	volatile int rx1_pointer = 0;
#endif
//#if UART1_USE_EXT_TX_ISR == 1
	volatile char tx1_buffer[128] = "";
	volatile int tx1_pointer = 0;
//#ednif
volatile char *tx1_data;
volatile int tx1_size = 0;

/*------------------------------------------------------------
Number conversion - convert(uint16_t num, int base)
Converts numbers in different notations to a string
Arguments: uint16_t num - input value
		   int base - notation base
Returns: char * - converted number in a string
------------------------------------------------------------*/
char * convert(uint16_t num, int base){
	static char alphabet[]= "0123456789ABCDEF";
	static char buffer[64];
	char *ptr;
	
	ptr = &buffer[63];
	*ptr = '\0';
	
	do {
		*--ptr = alphabet[num % base];
		num /= base;
	} while(num != 0);
	
	return(ptr);
}

/*------------------------------------------------------------
UART0 setup - uart0_init(unsigned int ubrr)
Sets up UART0 registers
Arguments: unsigned int ubrr - UBRR value, calculated using
		    formula in drivers/uart.h. Defines UART0 baud rate.
Returns: int - 0 (additional checks may be added later)
------------------------------------------------------------*/
int uart0_init(unsigned int ubrr){
	UBRR0H = 0;
	UBRR0L = 12;
	UCSR0B = (RX0EN<<RXEN)|(TX0EN<<TXEN)|(RX0IE<<RXCIE)|(TX0IE<<TXCIE)|(0<<UDRIE);
	UCSR0C = (0<<USBS)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(0<<UPM00)|(0<<UPM01)|(0<<UMSEL0);
	return 0;
}

/*------------------------------------------------------------
UART0 interrupted TX start - uart0_transmit(unsigned int ubrr)
Transmits data stored in tx0_buffer using uart0 interrupts.
Data should be put in buffer BEFORE the transmission starts.
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void uart0_transmit(){
	UDR0 = tx0_buffer[0];
	UCSR0B |= (1<<UDRIE);
	creg0 |= (1<<TX0BUSY);
}

/*------------------------------------------------------------
UART0 uninterrupted character TX - uart0_putc(char c)
Transmits a single character through uart0. Does not require
interrupts.
Arguments: char c - character to be sent
Returns: nothing
------------------------------------------------------------*/
void uart0_putc(char c){
	UDR0 = c;
	while(!(UCSR0A & (1<<UDRE)));
}

/*------------------------------------------------------------
UART0 uninterrupted string TX - uart0_puts(char * msg)
Transmits a string through uart0. Does not require
interrupts.
Arguments: char * msg - string pointer
Returns: nothing
------------------------------------------------------------*/
void uart0_puts(char * msg){
	int i = 0;
	while(msg[i] != '\0'){
		UDR0 = msg[i];
		while(!(UCSR0A & (1<<UDRE)));
		i++;
	}
}

/*------------------------------------------------------------
UART0 receive buffer flush - rx0_buffer_flush()
Clears the uart0 receive buffer (rx0_buffer).
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void rx0_buffer_flush(){
	rx0_buffer[0] = 0;
	rx0_pointer = 0;
}

/*------------------------------------------------------------
UART0 transmit buffer flush - tx0_buffer_flush()
Clears the uart0 transmit buffer (rx0_buffer).
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void tx0_buffer_flush(){
	tx0_pointer = 0;
	tx0_buffer[0] = '\x0';
}

void rx0_enableInterrupt(){
	UCSR0B |= (1 << RXCIE);
	creg0 |= ~(1 << RX0_IE);
}

void rx0_disableInterrupt(){
	UCSR0B &= ~(1 << RXCIE);
	creg0 &= ~(1 << RX0_IE);
}

void tx0_enableInterrupt(){
	UCSR0B |= (1 << UDRIE);
	creg0 |= ~(1 << TX0_IE);
}

void tx0_disableInterrupt(){
	UCSR0B &= ~(1 << UDRIE);
	creg0 &= ~(1 << TX0_IE);
}

/*------------------------------------------------------------
UART0 receive interrupt service routine
Puts received character into rx0_buffer.
Arguments: none
Returns: nothing
------------------------------------------------------------*/
#if UART0_USE_EXT_RX_ISR == 0
ISR(USART0_RX_vect){
	char data = UDR0;
	if(strlen((char*)&rx0_buffer) < 32){
		rx0_buffer[rx0_pointer] = data;
		rx0_pointer++;
	}
}
#endif

/*------------------------------------------------------------
UART0 transmit interrupt service routine
Pushes the next character in tx0_buffer to uart0 line
Arguments: none
Returns: nothing
------------------------------------------------------------*/
#if UART0_USE_EXT_UDRE_ISR == 0
ISR(USART0_UDRE_vect){
	tx0_pointer+=1;
	if(tx0_buffer[tx0_pointer] != '\x0'){
		UDR0 = tx0_buffer[tx0_pointer];
	}
	else {
		UCSR0B &= ~(1<<UDRIE);
		creg0 &= ~(1<<TX0BUSY);
		tx0_buffer_flush();
	}
}
#endif
/*------------------------------------------------------------
UART1 setup - uart1_init(unsigned int ubrr)
Sets up UART1 registers
Arguments: unsigned int ubrr - UBRR value, calculated using
		    formula in drivers/uart.h. Defines UART0 baud rate.
Returns: int - 0 (additional checks may be added later)
------------------------------------------------------------*/
int uart1_init(unsigned int ubrr){
	UBRR1H = 0;
	UBRR1L = 51;
	UCSR1B = (RX1EN<<RXEN)|(TX1EN<<TXEN)|(RX1IE<<RXCIE)|(TX1IE<<TXCIE)|(0<<UDRIE);
	UCSR1C = (0<<USBS)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(0<<UPM00)|(0<<UPM01)|(0<<UMSEL0);
	return 0;
}

/*------------------------------------------------------------
UART1 interrupted TX start - uart1_transmit(unsigned int ubrr)
Transmits data stored in tx1_buffer using uart1 interrupts.
Data should be put in buffer BEFORE the transmission starts.
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void uart1_transmit(){
	UDR1 = tx1_buffer[0];
	UCSR1B |= (1<<UDRIE);
	creg1 |= (1<<TX1BUSY);
}

/*------------------------------------------------------------
UART1 receive buffer flush - rx1_buffer_flush()
Clears the uart1 receive buffer (rx1_buffer).
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void rx1_buffer_flush(){
	rx1_buffer[0] = 0;
	rx1_pointer = 0;
}

/*------------------------------------------------------------
UART1 transmit buffer flush - tx1_buffer_flush()
Clears the uart1 transmit buffer (rx1_buffer).
Arguments: none
Returns: nothing
------------------------------------------------------------*/
void tx1_buffer_flush(){
	tx1_pointer = 0;
	tx1_buffer[0] = '\x0';
}

void rx1_enableInterrupt(){
	UCSR1B |= (1 << RXCIE);
	creg1 |= ~(1 << RX1_IE);
}

void rx1_disableInterrupt(){
	UCSR1B &= ~(1 << RXCIE);
	creg1 &= ~(1 << RX1_IE);
}

void tx1_enableInterrupt(){
	UCSR1B |= (1 << UDRIE);
	creg1 |= ~(1 << TX1_IE);
}

void tx1_disableInterrupt(){
	UCSR1B &= ~(1 << UDRIE);
	creg1 &= ~(1 << TX1_IE);
}

#if UART1_USE_EXT_RX_ISR == 0
ISR(USART1_RX_vect){
	char data = UDR1;
	if(strlen((char*)&rx1_buffer) < 32){
		rx1_buffer[rx1_pointer] = data;
		rx1_pointer++;
	}
}
#endif

/*------------------------------------------------------------
UART1 transmit interrupt service routine
Pushes the next character in tx0_buffer to uart0 line
Arguments: none
Returns: nothing
------------------------------------------------------------*/
#if UART1_USE_EXT_UDRE_ISR == 0
ISR(USART1_UDRE_vect){
	tx1_pointer+=1;
	if(tx1_buffer[tx1_pointer] != '\x0'){
		UDR1 = tx1_buffer[tx1_pointer];
	}
	else {
		UCSR1B &= ~(1<<UDRIE);
		creg1 &= ~(1<<TX1BUSY);
		tx1_buffer_flush();
	}
}
#endif
