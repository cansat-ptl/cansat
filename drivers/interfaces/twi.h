/*
 * twi.h
 *
 * Created: 07.06.2019 23:15:11
 *  Author: ThePetrovich
 */
 
#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <util/twi.h>

#define TWIDRV_VER "0.0.1-a1"
#define TWIDRV_TIMESTAMP __TIMESTAMP__

//Fix compile error by supplying a default F_CPU value
#ifndef F_CPU
//#warning "F_CPU is not defined until twi.h"
#define F_CPU 8000000L
#endif
//TWI predefines
#define F_SCL 100000L //SCL frequency
#define TWI_PRESCALER 1 //SCL prescaler
#define TWBRVAL ((((F_CPU / F_SCL) / TWI_PRESCALER) - 16 ) / 2)
#define TWI_READ 1
#define TWI_WRITE 0
#define TW_OK 0xF8
//Errors
#define TWI_START_ERR 1
#define TWI_ACK_ERR 2
#define TWI_WRITE_ERR 3
#define TWI_READ_ERR 4

#define sendStart() TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)

void twi_init();
uint8_t twi_start(uint8_t address);
void twi_stop();
uint8_t twi_write(uint8_t data);
uint8_t twi_read_ack();
uint8_t twi_read_nack();
uint8_t twi_writeRegister(uint8_t address, uint8_t _register, uint8_t* data, uint16_t size);
uint8_t twi_readRegister(uint8_t address, uint8_t _register, uint8_t* data, uint16_t size);
uint8_t twi_transfer(uint8_t mode, uint8_t address, uint8_t* data, uint8_t size);

#endif /* TWI_H_ */