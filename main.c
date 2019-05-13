/*
 * yktcansat_staging.c
 *
 * Created: 07.03.2019 17:56:34
 *  Author: ThePetrovich
 */ 


#include "config.h"

extern volatile char tx0_buffer[128]; 
extern volatile unsigned char creg0;

static int uart0_putchar(char c, FILE *stream);
static FILE uart0stdout = FDEV_SETUP_STREAM(uart0_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart0_putchar(char c, FILE *stream){
	if (c == '\n')
		uart0_putchar('\r', stream);
	while(UCSR0A & (1<<UDRE)){
		;
	}
	UDR0 = c;
	return 0;
}

int main(void){
	uart0_init(51);
	//spi_init();
	//tc72_init();
	stdout = &uart0stdout;
	//uart1_init(UBRRVAL);
	/*sei();
	sprintf((char*)&tx0_buffer, "TOLYA KOOSOK GOVNA\r\n");
    uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
	while(1){
		uint16_t temp = tc72_requestTemperatureRaw();
		float temp2 = tc72_calculateTemperature(temp);
		sprintf((char*)&tx0_buffer, "temperature = %d\r\n", temp);
		uart0_transmit();
		while(creg0 & (1<<TX0BUSY)){
			;
		}
		_delay_ms(1000);
	}*/
	sei();
	sprintf((char*)&tx0_buffer, "[INIT]Main: starting up\r\n");
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
	sprintf((char*)&tx0_buffer, "[INIT]Main: using kernel version %s built %s\r\n", KERNEL_VER, KERNEL_TIMESTAMP);
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
	sprintf((char*)&tx0_buffer, "[INIT]Main: using UART driver version %s built %s\r\n", UARTDRV_VER, UARTDRV_TIMESTAMP);
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
	sprintf((char*)&tx0_buffer, "[INIT]Main: using SPI driver version %s built %s\r\n", SPIDRV_VER, SPIDRV_TIMESTAMP);
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
	kernelInit();
}