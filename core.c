/*
 * yktcansat_staging.c
 *
 * Created: 07.03.2019 17:56:34
 *  Author: ThePetrovich
 */ 


#include "config.h"

extern volatile char tx0_buffer[128]; 
extern volatile unsigned char creg0;

int main(void)
{
	uart0_init(51);
	spi_init();
	tc72_init();
	//uart1_init(UBRRVAL);
	sei();
	sprintf((char*)&tx0_buffer, "TOLYA KOOSOK GOVNA\r\n");
    uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
	while(1){
		uint16_t temp = tc72_requestTemperatureRaw();
		float temp2 = tc72_calculateTemperature(temp);
		sprintf((char*)&tx0_buffer, "temperature = %f\r\n", temp2);
		uart0_transmit();
		while(creg0 & (1<<TX0BUSY)){
			;
		}
		_delay_ms(1000);
	}
}