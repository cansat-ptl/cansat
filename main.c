/*
 * yktcansat_staging.c
 *
 * Created: 07.03.2019 17:56:34
 *  Author: Admin
 */ 


#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include "drivers/uart.h"
#include "drivers/spi.h"

#define F_CPU 8000000L
#define BAUD 9600
#define UBRRVAL ((F_CPU/(16*BAUD)) - 1)

extern volatile char tx0_buffer[128]; 
extern volatile unsigned char creg0;

int main(void)
{
	uart0_init(51);
	//uart1_init(UBRRVAL);
	sei();
	sprintf((char*)&tx0_buffer, "TOLYA KOOSOK GOVNA\r\n");
    uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
	sprintf((char*)&tx0_buffer, "HELLO FROM AVR\r\n");
	uart0_transmit();
	while(1){
		;
	}
}