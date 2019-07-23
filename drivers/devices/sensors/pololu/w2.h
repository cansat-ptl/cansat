#ifndef W2_H
#define W2_H
#define w2_start	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); w2_busy
#define w2_stop		TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN); 
#define w2_ack		TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN); w2_busy
#define w2_nack		TWCR = (1<<TWINT) | (1<<TWEN); w2_busy
#define w2_on		TWCR = (1<<TWINT) | (1<<TWEN)
#define w2_sb		TWCR = (1<<TWINT) | (1<<TWEN); w2_busy;
#define w2_off		TWCR = 0
#define w2_busy		while(!(TWCR & (1<<TWINT)))

#define khz400		2
#define khz100		32
#define w2_freq(a)	TWBR = a;

#include <avr/io.h>

void w2_init()
{
	w2_freq(khz400);
	TWSR = 0x00;
	PORTD |= (1<<PD0|1<<PD1);
	DDRD &= ~(1<<PD0) & (1<<PD1);
	w2_on;
}

void w2_write(unsigned char byte)
{
	TWDR = byte;
	w2_sb;
}

unsigned char w2_read(unsigned char ack)
{
	if(ack)
	{
		w2_ack;
	}
	else
	{
		w2_nack;
	}
	return TWDR;
}
#endif


